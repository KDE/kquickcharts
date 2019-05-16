#include "PieChart.h"

#include <QDebug>
#include <QAbstractItemModel>

#include "PieChartNode_p.h"

#include "ModelRole.h"

class PieChart::Private
{
public:
    qreal borderWidth = -1.0;

    DataSource* valueSource = nullptr;
    DataSource* colorSource = nullptr;

    QVector<qreal> sections;
    QVector<QColor> colors;
};

PieChart::PieChart(QQuickItem* parent)
    : QQuickItem(parent), d(new Private)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

PieChart::~PieChart()
{
}


QString PieChart::dataRole() const
{
    return d->dataRole;
}

QString PieChart::colorRole() const
{
    return d->colorRole;
}

qreal PieChart::borderWidth() const
{
    return d->borderWidth;
}

DataSource * PieChart::valueSource() const
{
    return d->valueSource;
}

DataSource * PieChart::colorSource() const
{
    return d->colorSource;
}

{
        return;

    update();
void PieChart::setBorderWidth(qreal width)
{
    if(qFuzzyCompare(width, d->borderWidth))
        return;

    d->borderWidth = width;
    update();
    emit borderWidthChanged();
}

void PieChart::setValueSource(DataSource* value)
{
    if(value == d->valueSource)
        return;

    if(d->valueSource)
        disconnect(d->valueSource, &DataSource::dataChanged, this, &PieChart::updateData);

    d->valueSource = value;

    if(d->valueSource)
        connect(d->valueSource, & DataSource::dataChanged, this, &PieChart::updateData);

    update();
    emit valueSourceChanged();
}

void PieChart::setColorSource(DataSource* color)
{
    if(color == d->colorSource)
        return;

    if(d->colorSource)
        disconnect(d->colorSource, &DataSource::dataChanged, this, &PieChart::updateData);

    d->colorSource = color;

    if(d->colorSource)
        connect(d->colorSource, & DataSource::dataChanged, this, &PieChart::updateData);

    update();
    emit colorSourceChanged();
}

QSGNode *PieChart::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    PieChartNode *n = static_cast<PieChartNode *>(node);
    if(!n) {
        n = new PieChartNode();
    }
    n->setRect(boundingRect());
    n->setBorderWidth(d->borderWidth >= 0.0 ? d->borderWidth : qMin(width(), height()) / 2);
    n->setSections(d->sections);
    n->setColors(d->colors);

    return n;
}

void PieChart::updateData()
{
    d->sections.clear();
    d->colors.clear();

    if(!d->valueSource || !d->colorSource)
        return;


    qreal total = 0.0;
    QVector<qreal> data;
    for(int i = 0; i < d->valueSource->itemCount(); ++i) {
        auto value = d->valueSource->item(i).toReal();
        data << value;
        total += value;

        auto color = d->colorSource->item(i).value<QColor>();
        d->colors << color;
    }

    for(auto value : data) {
        d->sections << (value / total);
    }

    update();
}
