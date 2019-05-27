#include "LineChart.h"

#include <numeric>

#include "ChartDataSource.h"
#include "Axis.h"
#include "RangeGroup.h"

#include "scenegraph/LineGridNode.h"
#include "scenegraph/LineChartNode.h"

struct ChartRange {
    int startX = 0;
    int endX = 0;
    qreal startY = 0.0;
    qreal endY = 0.0;
    qreal distance = 0.0;
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

    LineChart* q;

    RangeGroup *xRange = nullptr;
    RangeGroup *yRange = nullptr;
    QVector<ChartDataSource*> valueSources;
    ChartDataSource *lineColorSource = nullptr;
    bool stacked = false;
    bool smooth = false;
    qreal lineWidth = 1.0;
    qreal fillOpacity = 0.0;

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

QSGNode *LineChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if(!node) {
        node = new QSGNode();
    }

    d->updateRange();

    for (int i = 0; i < d->valueSources.size(); ++i) {
        if(i >= node->childCount())
            node->appendChildNode(new LineChartNode{});

        auto lineNode = static_cast<LineChartNode*>(node->childAtIndex(i));
        auto color = d->lineColorSource->item(i).value<QColor>();
        d->updateLineNode(lineNode, color, d->valueSources.at(i));
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

    if(yRange->automatic()) {
        qreal minY = std::numeric_limits<qreal>::max();
        qreal maxY = std::numeric_limits<qreal>::min();

        for(auto valueSource : valueSources) {
            minY = qMin(minY, valueSource->minimum().toReal());
            maxY = qMax(maxY, valueSource->maximum().toReal());
        }
        computedRange.startY = minY;
        computedRange.endY = maxY;
    } else {
        computedRange.startY = yRange->from();
        computedRange.endY = yRange->to();
    }
    computedRange.distance = computedRange.endY - computedRange.startY;
}

void LineChart::Private::updateLineNode(LineChartNode* node, const QColor& lineColor, ChartDataSource* valueSource)
{
    auto fillColor = lineColor;
    fillColor.setAlphaF(fillOpacity);

    node->setRect(q->boundingRect());
    node->setLineColor(lineColor);
    node->setFillColor(fillColor);
    node->setLineWidth(lineWidth);

    QVector<qreal> values;
    for(int i = computedRange.startX; i < computedRange.endX; i++) {
        values << valueSource->item(i).toReal();
    }

    for(auto& value : values) {
        value = (value - computedRange.startY) / computedRange.distance;
    }
    node->setValues(values);
}