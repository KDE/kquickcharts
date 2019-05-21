#include "LineChart.h"

#include "ChartDataSource.h"
#include "Axis.h"
#include "RangeGroup.h"

#include "scenegraph/LineGridNode.h"

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
                                             d.get(),
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
    emit xAxisChanged();
}

void LineChart::setYAxis(Axis* axis)
{
    if(axis == d->yAxis)
        return;

    d->yAxis = axis;
    emit yAxisChanged();
}

void LineChart::setLineColorSource(ChartDataSource* source)
{
    if(source == d->lineColorSource)
        return;

    d->lineColorSource = source;
    emit lineColorSourceChanged();
}

void LineChart::setStacked(bool stacked)
{
    if (d->stacked == stacked) {
        return;
    }

    d->stacked = stacked;
    emit stackedChanged();
}

void LineChart::setSmooth(bool smooth)
{
    if (smooth == d->smooth)
        return;

    d->smooth = smooth;
    emit smoothChanged();
}

QSGNode *LineChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data);

    if(!node) {
        node = new QSGNode();
        node->appendChildNode(new LineGridNode());
    }

    LineGridNode *n = static_cast<LineGridNode *>(node->childAtIndex(0));
    n->setRect(boundingRect());
    n->setDrawX(d->xAxis->drawLines());
    n->setDrawY(d->yAxis->drawLines());
    n->setXSpacing((d->xAxis->stepSize() / d->xAxis->range()->distance()) * width());
    n->setYSpacing((d->yAxis->stepSize() / d->yAxis->range()->distance()) * height());

    n->update();

    return node;
}

void LineChart::Private::appendSource(LineChart::DataSourcesProperty *list, ChartDataSource* source)
{
    reinterpret_cast<LineChart::Private*>(list->data)->valueSources.append(source);
}

int LineChart::Private::sourceCount(LineChart::DataSourcesProperty *list)
{
    return reinterpret_cast<LineChart::Private*>(list->data)->valueSources.count();
}

ChartDataSource * LineChart::Private::source(LineChart::DataSourcesProperty* list, int index)
{
    return reinterpret_cast<LineChart::Private*>(list->data)->valueSources.at(index);
}

void LineChart::Private::clearSources(LineChart::DataSourcesProperty* list)
{
    reinterpret_cast<LineChart::Private*>(list->data)->valueSources.clear();
}
