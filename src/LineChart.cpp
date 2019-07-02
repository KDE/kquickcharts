#include "LineChart.h"

#include <numeric>
#include <QPainterPath>
#include <QPainter>

#include "RangeGroup.h"
#include "datasource/ChartDataSource.h"
#include "scenegraph/LineGridNode.h"
#include "scenegraph/LineChartNode.h"

class LineChart::Private
{
public:
    Private(LineChart* qq) : q(qq) { }

    void updateLineNode(LineChartNode* node, const QColor& lineColor, ChartDataSource* valueSource);
    QVector<QVector2D> interpolate(const QVector<QVector2D> &points, qreal start, qreal end, qreal height);

    LineChart* q;

    bool stacked = false;
    bool smooth = false;
    qreal lineWidth = 1.0;
    qreal fillOpacity = 0.0;
    LineChart::Direction direction = Direction::ZeroAtStart;
    QVector<QVector2D> previousValues;

    bool rangeInvalid = true;
};

LineChart::LineChart(QQuickItem* parent)
    : XYChart(parent), d(new Private{this})
{
}

LineChart::~LineChart()
{
}

bool LineChart::stacked() const
{
    return d->stacked;
}

bool LineChart::smooth() const
{
    return d->smooth;
}

qreal LineChart::lineWidth() const
{
    return d->lineWidth;
}

qreal LineChart::fillOpacity() const
{
    return d->fillOpacity;
}

void LineChart::setStacked(bool stacked)
{
    if (d->stacked == stacked) {
        return;
    }

    d->stacked = stacked;
    d->rangeInvalid = true;
    update();
    Q_EMIT stackedChanged();
}

void LineChart::setSmooth(bool smooth)
{
    if (smooth == d->smooth)
        return;

    d->smooth = smooth;
    update();
    Q_EMIT smoothChanged();
}

void LineChart::setLineWidth(qreal width)
{
    if(qFuzzyCompare(d->lineWidth, width))
        return;

    d->lineWidth = width;
    update();
    Q_EMIT lineWidthChanged();
}

void LineChart::setFillOpacity(qreal opacity)
{
    if (qFuzzyCompare(d->fillOpacity, opacity))
        return;

    d->fillOpacity = opacity;
    update();
    Q_EMIT fillOpacityChanged();
}

QSGNode *LineChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if(!node) {
        node = new QSGNode();
    }

    if (d->rangeInvalid) {
        updateComputedRange();
        d->rangeInvalid = false;
    }

    if (d->stacked)
        d->previousValues.clear();

    const auto sources = valueSources();
    for (int i = 0; i < sources.size(); ++i) {
        if(i >= node->childCount())
            node->appendChildNode(new LineChartNode{});

        auto lineNode = static_cast<LineChartNode*>(node->childAtIndex(i));
        auto color = colorSource()->item(i).value<QColor>();
        d->updateLineNode(lineNode, color, sources.at(i));
    }

    while (node->childCount() > sources.size()) {
        node->removeChildNode(node->childAtIndex(node->childCount() - 1));
    }

    return node;
}

void LineChart::onDataChanged()
{
    d->rangeInvalid = true;
    update();
}

void LineChart::updateAutomaticXRange(ComputedRange& range)
{
    range.startX = 0;
    int maxX = -1;
    const auto sources = valueSources();
    for(auto valueSource : sources) {
        maxX = qMax(maxX, valueSource->itemCount());
    }
    range.endX = maxX;
}

void LineChart::updateAutomaticYRange(ComputedRange& range)
{
    auto minY = std::numeric_limits<float>::max();
    auto maxY = std::numeric_limits<float>::min();

    const auto sources = valueSources();
    if (!d->stacked) {
        for (auto valueSource : sources) {
            minY = qMin(minY, valueSource->minimum().toFloat());
            maxY = qMax(maxY, valueSource->maximum().toFloat());
        }
    } else {
        auto yDistance = 0.0;
        for (auto valueSource : sources) {
            minY = qMin(minY, valueSource->minimum().toFloat());
            yDistance += valueSource->maximum().toFloat();
        }
        maxY = minY + yDistance;
    }
    range.startY = std::min(0.0f, minY);
    range.endY = std::max(0.0f, maxY);
}

void LineChart::Private::updateLineNode(LineChartNode* node, const QColor& lineColor, ChartDataSource* valueSource)
{
    auto fillColor = lineColor;
    fillColor.setAlphaF(fillOpacity);

    node->setRect(q->boundingRect());
    node->setLineColor(lineColor);
    node->setFillColor(fillColor);
    node->setLineWidth(lineWidth);

    auto range = q->computedRange();

    float stepSize = q->width() / (range.distanceX - 1);
    QVector<QVector2D> values(range.distanceX);
    auto generator = [&, i = range.startX]() mutable -> QVector2D {
        auto result = QVector2D{
            direction == Direction::ZeroAtStart ? i * stepSize : float(q->boundingRect().right()) - i * stepSize,
            (valueSource->item(i).toFloat() - range.startY) / range.distanceY
        };
        i++;
        return result;
    };

    if(direction == Direction::ZeroAtStart) {
        std::generate_n(values.begin(), range.distanceX, generator);
    } else {
        std::generate_n(values.rbegin(), range.distanceX, generator);
    }

    if (stacked && !previousValues.isEmpty()) {
        if (values.size() != previousValues.size()) {
            qWarning() << "Value source" << valueSource->objectName() << "has a different number of elements from the previuous source. Ignoring stacking for this source.";
        } else {
            std::for_each(values.begin(), values.end(), [this, i = 0](QVector2D &point) mutable {
                point.setY(point.y() + previousValues.at(i++).y());
            });
        }
    }
    previousValues = values;

    if (smooth) {
        values = interpolate(values, 0.0, q->width(), q->height());
    }

    node->setValues(values);
}

QVector<QVector2D> LineChart::Private::interpolate(const QVector<QVector2D> &points, qreal start, qreal end, qreal height)
{
    QPainterPath path;
    if(points.size() < 4)
        return points;

    const QMatrix4x4 matrix( 0,    1,    0,     0,
                            -1/6., 1,    1/6.,  0,
                             0,    1/6., 1,    -1/6.,
                             0,    0,    1,     0);

    const qreal xDelta = (end - start) / (points.count() - 3);
    qreal x = start - xDelta;

    path.moveTo(start, points[0].y() * height);

    for (int i = 1; i < points.count() - 2; i++) {
        const QMatrix4x4 p(x,              points[i-1].y() * height, 0, 0,
                           x + xDelta * 1, points[i+0].y() * height, 0, 0,
                           x + xDelta * 2, points[i+1].y() * height, 0, 0,
                           x + xDelta * 3, points[i+2].y() * height, 0, 0);

        const QMatrix4x4 res = matrix * p;

        path.cubicTo(res(1, 0), res(1, 1),
                     res(2, 0), res(2, 1),
                     res(3, 0), res(3, 1));

        x += xDelta;
    }

    QVector<QVector2D> result;

    const auto polygons = path.toSubpathPolygons();
    for(const auto &polygon : polygons) {
        for(auto point : polygon) {
            result.append(QVector2D{float(point.x()), float(point.y() / q->height())});
        }
    }

    return result;
}
