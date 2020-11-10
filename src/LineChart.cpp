/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "LineChart.h"

#include <QPainter>
#include <QPainterPath>
#include <QQuickWindow>
#include <numeric>

#include "RangeGroup.h"
#include "datasource/ChartDataSource.h"
#include "scenegraph/LineChartNode.h"
#include "scenegraph/LineGridNode.h"

QVector<QVector2D> interpolate(const QVector<QVector2D> &points, qreal start, qreal end, qreal height);

QColor colorWithAlpha(const QColor &color, qreal opacity)
{
    auto result = color;
    result.setRedF(result.redF() * opacity);
    result.setGreenF(result.greenF() * opacity);
    result.setBlueF(result.blueF() * opacity);
    result.setAlphaF(opacity);
    return result;
}

LineChartAttached::LineChartAttached(QObject* parent)
    : QObject(parent)
{
}

QVariant LineChartAttached::value() const
{
    return m_value;
}

void LineChartAttached::setValue(const QVariant& value)
{
    if (value == m_value) {
        return;
    }

    m_value = value;
    Q_EMIT valueChanged();
}

QColor LineChartAttached::color() const
{
    return m_color;
}

void LineChartAttached::setColor(const QColor& color)
{
    if (color == m_color) {
        return;
    }

    m_color = color;
    Q_EMIT colorChanged();
}

QString LineChartAttached::name() const
{
    return m_name;
}

void LineChartAttached::setName(const QString & newName)
{
    if (newName == m_name) {
        return;
    }

    m_name = newName;
    Q_EMIT nameChanged();
}

QString LineChartAttached::shortName() const
{
    if (m_shortName.isEmpty()) {
        return m_name;
    } else {
        return m_shortName;
    }
}

void LineChartAttached::setShortName(const QString & newShortName)
{
    if (newShortName == m_shortName) {
        return;
    }

    m_shortName = newShortName;
    Q_EMIT shortNameChanged();
}

LineChart::LineChart(QQuickItem *parent)
    : XYChart(parent)
{
}

bool LineChart::smooth() const
{
    return m_smooth;
}

qreal LineChart::lineWidth() const
{
    return m_lineWidth;
}

qreal LineChart::fillOpacity() const
{
    return m_fillOpacity;
}

void LineChart::setSmooth(bool smooth)
{
    if (smooth == m_smooth) {
        return;
    }

    m_smooth = smooth;
    polish();
    Q_EMIT smoothChanged();
}

void LineChart::setLineWidth(qreal width)
{
    if (qFuzzyCompare(m_lineWidth, width)) {
        return;
    }

    m_lineWidth = width;
    update();
    Q_EMIT lineWidthChanged();
}

void LineChart::setFillOpacity(qreal opacity)
{
    if (qFuzzyCompare(m_fillOpacity, opacity)) {
        return;
    }

    m_fillOpacity = opacity;
    update();
    Q_EMIT fillOpacityChanged();
}

ChartDataSource *LineChart::fillColorSource() const
{
    return m_fillColorSource;
}

void LineChart::setFillColorSource(ChartDataSource *newFillColorSource)
{
    if (newFillColorSource == m_fillColorSource) {
        return;
    }

    m_fillColorSource = newFillColorSource;
    update();
    Q_EMIT fillColorSourceChanged();
}

QQmlComponent *LineChart::pointDelegate() const
{
    return m_pointDelegate;
}

void LineChart::setPointDelegate(QQmlComponent *newPointDelegate)
{
    if (newPointDelegate == m_pointDelegate) {
        return;
    }

    m_pointDelegate = newPointDelegate;
    for (auto entry : qAsConst(m_pointDelegates)) {
        qDeleteAll(entry);
    }
    m_pointDelegates.clear();
    polish();
    Q_EMIT pointDelegateChanged();
}

void LineChart::updatePolish()
{
    if (m_rangeInvalid) {
        updateComputedRange();
        m_rangeInvalid = false;
    }

    QVector<QVector2D> previousValues;

    const auto range = computedRange();
    const auto sources = valueSources();
    for (int i = 0; i < sources.size(); ++i) {
        auto valueSource = sources.at(i);

        float stepSize = width() / (range.distanceX - 1);
        QVector<QVector2D> values(range.distanceX);
        auto generator = [&, i = range.startX]() mutable -> QVector2D {
            float value = 0;
            if (range.distanceY != 0) {
                value = (valueSource->item(i).toFloat() - range.startY) / range.distanceY;
            }

            auto result = QVector2D{direction() == Direction::ZeroAtStart ? i * stepSize
                                    : float(boundingRect().right()) - i * stepSize, value};
            i++;
            return result;
        };

        if (direction() == Direction::ZeroAtStart) {
            std::generate_n(values.begin(), range.distanceX, generator);
        } else {
            std::generate_n(values.rbegin(), range.distanceX, generator);
        }

        if (stacked() && !previousValues.isEmpty()) {
            if (values.size() != previousValues.size()) {
                qWarning() << "Value source" << valueSource->objectName()
                        << "has a different number of elements from the previuous source. Ignoring stacking for this source.";
            } else {
                std::for_each(values.begin(), values.end(), [previousValues, i = 0](QVector2D &point) mutable {
                    point.setY(point.y() + previousValues.at(i++).y());
                });
            }
        }
        previousValues = values;

        if (m_pointDelegate) {
            auto& delegates = m_pointDelegates[valueSource];
            if (delegates.size() != values.size()) {
                qDeleteAll(delegates);
                createPointDelegates(values, i);
            } else {
                for (int item = 0; item < values.size(); ++item) {
                    auto delegate = delegates.at(item);
                    updatePointDelegate(delegate, values.at(item), valueSource->item(item), i);
                }
            }
        }

        if (m_smooth) {
            m_values[valueSource] = interpolate(values, height());
        } else {
            m_values[valueSource] = values;
        }
    }

    const auto pointKeys = m_pointDelegates.keys();
    for (auto key : pointKeys) {
        if (!sources.contains(key)) {
            qDeleteAll(m_pointDelegates[key]);
            m_pointDelegates.remove(key);
        }
    }

    update();
}

QSGNode *LineChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if (!node) {
        node = new QSGNode();
    }

    const auto sources = valueSources();
    for (int i = 0; i < sources.size(); ++i) {
        int childIndex = sources.size() - 1 - i;
        while (childIndex >= node->childCount()) {
            node->appendChildNode(new LineChartNode{});
        }
        auto lineNode = static_cast<LineChartNode *>(node->childAtIndex(childIndex));
        auto color = colorSource() ? colorSource()->item(i).value<QColor>() : Qt::black;
        auto fillColor = m_fillColorSource ? m_fillColorSource->item(i).value<QColor>() : colorWithAlpha(color, m_fillOpacity);
        updateLineNode(lineNode, color, fillColor, sources.at(i));
    }

    while (node->childCount() > sources.size()) {
        // removeChildNode unfortunately does not take care of deletion so we
        // need to handle this manually.
        auto lastNode = node->childAtIndex(node->childCount() - 1);
        node->removeChildNode(lastNode);
        delete lastNode;
    }

    return node;
}

void LineChart::onDataChanged()
{
    m_rangeInvalid = true;
    polish();
}

void LineChart::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
    XYChart::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry != oldGeometry) {
        polish();
    }
}

void LineChart::updateLineNode(LineChartNode *node, const QColor &lineColor, const QColor &fillColor, ChartDataSource *valueSource)
{
    if (window()) {
        node->setRect(boundingRect(), window()->devicePixelRatio());
    } else {
        node->setRect(boundingRect(), 1.0);
    }
    node->setLineColor(lineColor);
    node->setFillColor(fillColor);
    node->setLineWidth(m_lineWidth);

    auto values = m_values.value(valueSource);
    node->setValues(values);
}

QVector<QVector2D> interpolate(const QVector<QVector2D> &points, qreal start, qreal end, qreal height)
{
    QPainterPath path;
    if (points.size() < 4)
        return points;

    const auto sixth = 1.f / 6.f;

    const qreal xDelta = (end - start) / (points.count() - 3);
    qreal x = start - xDelta;

    path.moveTo(start, points[0].y() * height);

    for (int i = 1; i < points.count() - 2; i++) {
        // This code was:
        //
        // QMatrix4x4 matrix(   0,   1,   0,    0,
        //                   -1/6,   1, 1/6,    0,
        //                      0, 1/6,   1, -1/6,
        //                      0,   0,   1,    0);
        // QMatrix4x4 p(x + xDelta * 0, points[i - 1].y() * height, 0, 0,
        //              x + xDelta * 1, points[i + 0].y() * height, 0, 0,
        //              x + xDelta * 2, points[i + 1].y() * height, 0, 0,
        //              x + xDelta * 3, points[i + 2].y() * height, 0, 0)
        // QMatrix4x4 res = matrix * p;
        // path.cubicTo(res(1,0), res(1, 1), res(2, 0), res(2, 1), res(3, 0), res(3, 1))
        //
        // The below calculations calculate the used elements from the matrix directly, avoiding
        // most of an expensive matrix multiplication.

        auto p0 = points[i - 1].y() * height;
        auto p1 = points[i].y() * height;
        auto p2 = points[i + 1].y() * height;
        auto p3 = points[i + 2].y() * height;

        //res(1, 0) = (-1/6, 1, 1/6, 0) dot (x, x + xDelta, x + xDelta * 2, x + xDelta * 3)
        auto res10 = (3 * x + 4 * xDelta) / 3.f;
        //res(1, 1) = (-1/6, 1, 1/6, 0) dot (p[i-1].y, p[i].y, p[i+1].y, p[i+2].y)
        auto res11 = -sixth * p0 + p1 + sixth * p2;
        //res(2, 0) = (0, 1/6, 1, -1/6) dot (x, x + xDelta, x + xDelta * 2, x + xDelta * 3)
        auto res20 = (3 * x + 5 * xDelta) / 3.f;
        //res(2, 1) = (0, 1/6, 1, -1/6) dot (p[i-1].y, p[i].y, p[i+1].y, p[i+2].y)
        auto res21 = sixth * p1 + p2 + -sixth * p3;
        //res(3, 0) = (0, 0, 1, 0) dot (x, x + xDelta, x + xDelta * 2, x + xDelta * 3)
        auto res30 = x + 2 * xDelta;
        //res(3, 1) = (0, 0, 1, 0) dot (p[i-1].y, p[i].y, p[i+1].y, p[i+2].y)
        auto res31 = p2;

        path.cubicTo(res10, res11, res20, res21, res30, res31);

        x += xDelta;
    }

    QVector<QVector2D> result;

    const auto polygons = path.toSubpathPolygons();
    auto pointCount = std::accumulate(polygons.begin(), polygons.end(), 0, [](int current, const QPolygonF &polygon) {
        return current + polygon.size();
    });
    result.reserve(pointCount);

    for (const auto &polygon : polygons) {
        for (auto point : polygon) {
            result.append(QVector2D{float(point.x()), float(point.y() / height)});
        }
    }

    return result;
}

void LineChart::createPointDelegates(const QVector<QVector2D> &values, int sourceIndex)
{
    auto valueSource = valueSources().at(sourceIndex);

    QVector<QQuickItem*> delegates;
    for (int i = 0; i < values.size(); ++i) {
        auto delegate = qobject_cast<QQuickItem*>(m_pointDelegate->beginCreate(qmlContext(m_pointDelegate)));
        if (!delegate) {
            qWarning() << "Delegate creation for point" << i << "of value source" << valueSource->objectName()
                       << "failed, make sure pointDelegate is a QQuickItem";
            delegate = new QQuickItem(this);
        }

        delegate->setParent(this);
        delegate->setParentItem(this);
        updatePointDelegate(delegate, values.at(i), valueSource->item(i), sourceIndex);

        m_pointDelegate->completeCreate();

        delegates.append(delegate);
    }

    m_pointDelegates.insert(valueSource, delegates);
}

void LineChart::updatePointDelegate(QQuickItem *delegate, const QVector2D &position, const QVariant &value, int sourceIndex)
{
    auto pos = QPointF{position.x() - delegate->width() / 2, (1.0 - position.y()) * height() - delegate->height() / 2};
    delegate->setPosition(pos);

    auto attached = static_cast<LineChartAttached*>(qmlAttachedPropertiesObject<LineChart>(delegate, true));
    attached->setValue(value);
    attached->setColor(colorSource() ? colorSource()->item(sourceIndex).value<QColor>() : Qt::black);
    attached->setName(nameSource() ? nameSource()->item(sourceIndex).toString() : QString{});
    attached->setShortName(shortNameSource() ? shortNameSource()->item(sourceIndex).toString() : QString{});
}
