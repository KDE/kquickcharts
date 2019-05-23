#include "LineChart.h"

#include "ChartDataSource.h"
#include "Axis.h"
#include "RangeGroup.h"

#include "scenegraph/LineGridNode.h"
#include "scenegraph/LineChartNode.h"

class LineChart::Private
{
public:
    static void appendSource(DataSourcesProperty *list, ChartDataSource *source);
    static int sourceCount(DataSourcesProperty *list);
    static ChartDataSource* source(DataSourcesProperty *list, int index);
    static void clearSources(DataSourcesProperty *list);

    Axis *xAxis = nullptr;
    Axis *yAxis = nullptr;
    QVector<ChartDataSource*> valueSources;
    ChartDataSource *lineColorSource = nullptr;
    bool stacked = false;
    bool smooth = false;
};

LineChart::LineChart(QQuickItem* parent)
    : QQuickItem(parent), d(new Private)
{
    setFlag(ItemHasContents, true);
}

LineChart::~LineChart()
{
}

Axis * LineChart::xAxis() const
{
    return d->xAxis;
}

Axis * LineChart::yAxis() const
{
    return d->yAxis;
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

void LineChart::setXAxis(Axis* axis)
{
    if(axis == d->xAxis)
        return;

    d->xAxis = axis;
    update();
    emit xAxisChanged();
}

void LineChart::setYAxis(Axis* axis)
{
    if(axis == d->yAxis)
        return;

    d->yAxis = axis;
    update();
    emit yAxisChanged();
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

QSGNode *LineChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if(!node) {
        node = new QSGNode();
        node->appendChildNode(new LineGridNode());

        auto opacityNode = new QSGOpacityNode{};
        for(int i = 0; i < d->valueSources.size(); ++i) {
            opacityNode->appendChildNode(new LineChartNode{});
        }

        node->appendChildNode(opacityNode);

        auto lineNode = new QSGNode{};
        for(int i = 0; i < d->valueSources.size(); ++i) {
            lineNode->appendChildNode(new LineChartNode{});
        }
        node->appendChildNode(lineNode);
    }

    LineGridNode *n = static_cast<LineGridNode *>(node->childAtIndex(0));
    n->setRect(boundingRect());
    n->setDrawX(d->xAxis->drawLines());
    n->setDrawY(d->yAxis->drawLines());
    n->setXSpacing((d->xAxis->stepSize() / d->xAxis->range()->distance()) * width());
    n->setYSpacing((d->yAxis->stepSize() / d->yAxis->range()->distance()) * height());
    n->update();

    auto opacityNode = static_cast<QSGOpacityNode*>(node->childAtIndex(1));
    opacityNode->setOpacity(0.2);
    for(int i = 0; i < d->valueSources.size(); ++i) {
        LineChartNode *cn = static_cast<LineChartNode*>(opacityNode->childAtIndex(i));
        auto lineColor = d->lineColorSource->item(i).value<QColor>();
        cn->setRect(boundingRect());
        cn->setLineColor(QColor(0, 0, 0, 0));
        cn->setFillColor(lineColor);
        cn->setLineWidth(0.0);

        QVector<qreal> values;
        auto valueSource = d->valueSources.at(i);
        for(int i = 0; i < valueSource->itemCount(); i++) {
            values << (valueSource->item(i).toReal() - d->yAxis->range()->from()) / d->yAxis->range()->distance();
        }
        cn->setValues(values);
    }

    auto lineNode = node->childAtIndex(2);
    for(int i = 0; i < d->valueSources.size(); ++i) {
        LineChartNode *cn = static_cast<LineChartNode*>(lineNode->childAtIndex(i));
        auto lineColor = d->lineColorSource->item(i).value<QColor>();
        cn->setRect(boundingRect());
        cn->setLineColor(lineColor);
        cn->setFillColor(QColor(0, 0, 0, 0));
        cn->setLineWidth(2.0);

        QVector<qreal> values;
        auto valueSource = d->valueSources.at(i);
        for(int i = 0; i < valueSource->itemCount(); i++) {
            values << (valueSource->item(i).toReal() - d->yAxis->range()->from()) / d->yAxis->range()->distance();
        }
        cn->setValues(values);
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
