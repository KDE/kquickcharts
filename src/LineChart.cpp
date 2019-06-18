#include "LineChart.h"

#include <numeric>
#include <QPainterPath>
#include <QPainter>

#include "RangeGroup.h"
#include "datasource/ChartDataSource.h"
#include "scenegraph/LineGridNode.h"
#include "scenegraph/LineChartNode.h"

struct ChartRange {
    int startX = 0;
    int endX = 0;
    int distanceX = 0;
    float startY = 0.0;
    float endY = 0.0;
    float distanceY = 0.0;
};

class LineChart::Private
{
public:
    Private(LineChart* qq) : q(qq) { }

    static void appendSource(DataSourcesProperty *list, ChartDataSource *source);
    static int sourceCount(DataSourcesProperty *list);
    static ChartDataSource* source(DataSourcesProperty *list, int index);
    static void clearSources(DataSourcesProperty *list);

    void updateRange();
    void updateLineNode(LineChartNode* node, const QColor& lineColor, ChartDataSource* valueSource);
    QVector<QVector2D> interpolate(const QVector<QVector2D> &points, qreal start, qreal end, qreal height);

    LineChart* q;

    RangeGroup *xRange = nullptr;
    RangeGroup *yRange = nullptr;
    QVector<ChartDataSource*> valueSources;
    ChartDataSource *lineColorSource = nullptr;
    bool stacked = false;
    bool smooth = false;
    qreal lineWidth = 1.0;
    qreal fillOpacity = 0.0;
    LineChart::Direction direction = Direction::ZeroAtStart;
    QVector<QVector2D> previousValues;

    ChartRange computedRange;
};

LineChart::LineChart(QQuickItem* parent)
    : QQuickItem(parent), d(new Private{this})
{
    setFlag(ItemHasContents, true);

    d->xRange = new RangeGroup{this};
    connect(d->xRange, &RangeGroup::rangeChanged, this, &LineChart::update);
    d->yRange = new RangeGroup{this};
    connect(d->yRange, &RangeGroup::rangeChanged, this, &LineChart::update);
}

LineChart::~LineChart()
{
}

RangeGroup * LineChart::xRange() const
{
    return d->xRange;
}

RangeGroup * LineChart::yRange() const
{
    return d->yRange;
}

ChartDataSource * LineChart::lineColorSource() const
{
    return d->lineColorSource;
}

QQmlListProperty<ChartDataSource> LineChart::valueSources()
{
    return QQmlListProperty<ChartDataSource>(this,
                                             this,
                                             &Private::appendSource,
                                             &Private::sourceCount,
                                             &Private::source,
                                             &Private::clearSources);
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

LineChart::Direction LineChart::direction() const
{
    return d->direction;
}

void LineChart::setLineColorSource(ChartDataSource* source)
{
    if(source == d->lineColorSource)
        return;

    d->lineColorSource = source;
    update();
    emit lineColorSourceChanged();
}

void LineChart::setStacked(bool stacked)
{
    if (d->stacked == stacked) {
        return;
    }

    d->stacked = stacked;
    update();
    emit stackedChanged();
}

void LineChart::setSmooth(bool smooth)
{
    if (smooth == d->smooth)
        return;

    d->smooth = smooth;
    update();
    emit smoothChanged();
}

void LineChart::setLineWidth(qreal width)
{
    if(qFuzzyCompare(d->lineWidth, width))
        return;

    d->lineWidth = width;
    update();
    emit lineWidthChanged();
}

void LineChart::setFillOpacity(qreal opacity)
{
    if (qFuzzyCompare(d->fillOpacity, opacity))
        return;

    d->fillOpacity = opacity;
    update();
    emit fillOpacityChanged();
}

void LineChart::setDirection(LineChart::Direction dir)
{
    if(d->direction == dir)
        return;

    d->direction = dir;
    update();
    emit directionChanged();
}

void LineChart::insertValueSource(int position, ChartDataSource *source)
{
    // Avoid duplicates
    if (d->valueSources.contains(source)) {
        return;
    }

    d->valueSources.insert(position, source);
    connect(source, &QObject::destroyed, this, [this, source]() {
        removeValueSource(source);
    });
    QObject::connect(source, &ChartDataSource::dataChanged, this, &LineChart::update);
    update();
    emit valueSourcesChanged();
}

void LineChart::removeValueSource(ChartDataSource *source)
{
    int i = d->valueSources.indexOf(source);

    if (i < 0) {
        return;
    }

    d->valueSources.removeAll(source);
    
    emit valueSourcesChanged();
}

QSGNode *LineChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if(!node) {
        node = new QSGNode();
    }

    d->updateRange();

    if (d->stacked)
        d->previousValues.clear();

    for (int i = 0; i < d->valueSources.size(); ++i) {
        if(i >= node->childCount())
            node->appendChildNode(new LineChartNode{});

        auto lineNode = static_cast<LineChartNode*>(node->childAtIndex(i));
        auto color = d->lineColorSource->item(i).value<QColor>();
        d->updateLineNode(lineNode, color, d->valueSources.at(i));
    }

    while (node->childCount() > d->valueSources.size()) {
        node->removeChildNode(node->childAtIndex(node->childCount() - 1));
    }

    return node;
}

void LineChart::Private::appendSource(LineChart::DataSourcesProperty *list, ChartDataSource* source)
{
    auto chart = reinterpret_cast<LineChart*>(list->data);
    chart->d->valueSources.append(source);
    QObject::connect(source, &ChartDataSource::dataChanged, chart, &LineChart::update);
    chart->update();
}

int LineChart::Private::sourceCount(LineChart::DataSourcesProperty *list)
{
    return reinterpret_cast<LineChart*>(list->data)->d->valueSources.count();
}

ChartDataSource * LineChart::Private::source(LineChart::DataSourcesProperty* list, int index)
{
    return reinterpret_cast<LineChart*>(list->data)->d->valueSources.at(index);
}

void LineChart::Private::clearSources(LineChart::DataSourcesProperty* list)
{
    auto chart = reinterpret_cast<LineChart*>(list->data);
    std::for_each(chart->d->valueSources.begin(), chart->d->valueSources.end(), [chart](ChartDataSource* source) {
        source->disconnect(chart);
    });
    chart->d->valueSources.clear();
    chart->update();
}

void LineChart::Private::updateRange()
{
    if(xRange->automatic()) {
        computedRange.startX = 0;
        int maxX = -1;
        for(auto valueSource : valueSources) {
            maxX = qMax(maxX, valueSource->itemCount());
        }
        computedRange.endX = maxX;
    } else {
        computedRange.startX = xRange->from();
        computedRange.endX = xRange->to();
    }
    computedRange.distanceX = computedRange.endX - computedRange.startX;

    if(yRange->automatic()) {
        auto minY = std::numeric_limits<float>::max();
        auto maxY = std::numeric_limits<float>::min();

        if (!stacked) {
            for (auto valueSource : qAsConst(valueSources)) {
                minY = qMin(minY, valueSource->minimum().toFloat());
                maxY = qMax(maxY, valueSource->maximum().toFloat());
            }
        } else {
            auto yDistance = 0.0;
            for (auto valueSource : qAsConst(valueSources)) {
                minY = qMin(minY, valueSource->minimum().toFloat());
                yDistance += valueSource->maximum().toFloat();
            }
            maxY = minY + yDistance;
        }
        computedRange.startY = minY;
        computedRange.endY = maxY;
    } else {
        computedRange.startY = yRange->from();
        computedRange.endY = yRange->to();
    }
    computedRange.distanceY = computedRange.endY - computedRange.startY;
}

void LineChart::Private::updateLineNode(LineChartNode* node, const QColor& lineColor, ChartDataSource* valueSource)
{
    auto fillColor = lineColor;
    fillColor.setAlphaF(fillOpacity);

    node->setRect(q->boundingRect());
    node->setLineColor(lineColor);
    node->setFillColor(fillColor);
    node->setLineWidth(lineWidth);

    float stepSize = q->width() / (computedRange.distanceX - 1);
    QVector<QVector2D> values(computedRange.distanceX);
    auto generator = [&, i = computedRange.startX]() mutable -> QVector2D {
        return QVector2D{i * stepSize, (valueSource->item(i++).toFloat() - computedRange.startY) / computedRange.distanceY};
    };

    if(direction == Direction::ZeroAtStart) {
        std::generate_n(values.begin(), computedRange.distanceX, generator);
    } else {
        std::generate_n(values.rbegin(), computedRange.distanceX, generator);
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
    for(const auto polygon : polygons) {
        for(auto point : polygon) {
            result.append(QVector2D{float(point.x()), float(point.y() / q->height())});
        }
    }

    return result;
}
