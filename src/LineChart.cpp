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

        node->appendChildNode(new LineChartNode{});
    }

    LineGridNode *n = static_cast<LineGridNode *>(node->childAtIndex(0));
    n->setRect(boundingRect());
    n->setDrawX(d->xAxis->drawLines());
    n->setDrawY(d->yAxis->drawLines());
    n->setXSpacing((d->xAxis->stepSize() / d->xAxis->range()->distance()) * width());
    n->setYSpacing((d->yAxis->stepSize() / d->yAxis->range()->distance()) * height());

    n->update();

    LineChartNode *cn = static_cast<LineChartNode*>(node->childAtIndex(1));
    auto lineColor = d->lineColorSource->item(0).value<QColor>();
    cn->setRect(boundingRect());
    cn->setLineColor(lineColor);
    lineColor.setAlphaF(0.5);
    cn->setFillColor(lineColor);
    cn->setLineWidth(2.0);

    QVector<qreal> values;
    auto valueSource = d->valueSources.at(0);
    for(int i = 0; i < valueSource->itemCount(); i++) {
        values << (valueSource->item(i).toReal() - d->yAxis->range()->from()) / d->yAxis->range()->distance();
    }
    cn->setValues(values);

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
