/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "LineChart.h"

#include <QPainter>
#include <QPainterPath>
#include <numeric>

#include "RangeGroup.h"
#include "datasource/ChartDataSource.h"
#include "scenegraph/LineChartNode.h"
#include "scenegraph/LineGridNode.h"

QVector<QVector2D> interpolate(const QVector<QVector2D> &points, qreal start, qreal end, qreal height);

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
    update();
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

QSGNode *LineChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if (!node) {
        node = new QSGNode();
    }

    if (m_rangeInvalid) {
        updateComputedRange();
        m_rangeInvalid = false;
    }

    if (stacked()) {
        m_previousValues.clear();
    }

    const auto sources = valueSources();
    for (int i = 0; i < sources.size(); ++i) {
        int childIndex = sources.size() - 1 - i;
        while (childIndex >= node->childCount()) {
            node->appendChildNode(new LineChartNode{});
        }
        auto lineNode = static_cast<LineChartNode *>(node->childAtIndex(childIndex));
        auto color = colorSource() ? colorSource()->item(i).value<QColor>() : Qt::black;
        updateLineNode(lineNode, color, sources.at(i));
    }

    while (node->childCount() > sources.size()) {
        node->removeChildNode(node->childAtIndex(node->childCount() - 1));
    }

    return node;
}

void LineChart::onDataChanged()
{
    m_rangeInvalid = true;
    update();
}

void LineChart::updateLineNode(LineChartNode *node, const QColor &lineColor, ChartDataSource *valueSource)
{
    auto fillColor = lineColor;
    fillColor.setRedF(fillColor.redF() * m_fillOpacity);
    fillColor.setGreenF(fillColor.greenF() * m_fillOpacity);
    fillColor.setBlueF(fillColor.blueF() * m_fillOpacity);
    fillColor.setAlphaF(m_fillOpacity);

    node->setRect(boundingRect());
    node->setLineColor(lineColor);
    node->setFillColor(fillColor);
    node->setLineWidth(m_lineWidth);

    auto range = computedRange();

    float stepSize = width() / (range.distanceX - 1);
    QVector<QVector2D> values(range.distanceX);
    auto generator = [&, i = range.startX]() mutable -> QVector2D {
        float value = 0;
        if (range.distanceY != 0) {
            value = (valueSource->item(i).toFloat() - range.startY) / range.distanceY;
        }

        auto result = QVector2D{direction() == Direction::ZeroAtStart ? i * stepSize : float(boundingRect().right()) - i * stepSize,
                                value};
        i++;
        return result;
    };

    if (direction() == Direction::ZeroAtStart) {
        std::generate_n(values.begin(), range.distanceX, generator);
    } else {
        std::generate_n(values.rbegin(), range.distanceX, generator);
    }

    if (stacked() && !m_previousValues.isEmpty()) {
        if (values.size() != m_previousValues.size()) {
            qWarning() << "Value source" << valueSource->objectName()
                       << "has a different number of elements from the previuous source. Ignoring stacking for this source.";
        } else {
            std::for_each(
                values.begin(), values.end(), [this, i = 0](QVector2D &point) mutable { point.setY(point.y() + m_previousValues.at(i++).y()); });
        }
    }
    m_previousValues = values;

    if (m_smooth) {
        values = interpolate(values, 0.0, width(), height());
    }

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
