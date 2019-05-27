#include "PieChart.h"

#include <QAbstractItemModel>
#include <QDebug>

#include "scenegraph/PieChartNode_p.h"

#include "ChartDataSource.h"
#include "RangeGroup.h"

class PieChart::Private
{
public:
    RangeGroup *range = nullptr;
    qreal borderWidth = -1.0;

    ChartDataSource *valueSource = nullptr;
    ChartDataSource *colorSource = nullptr;

    QVector<qreal> sections;
    QVector<QColor> colors;

    QColor backgroundColor;
};

PieChart::PieChart(QQuickItem *parent)
    : QQuickItem(parent)
    , d(new Private)
{
    setFlag(QQuickItem::ItemHasContents, true);
    d->range = new RangeGroup{this};
    connect(d->range, &RangeGroup::rangeChanged, this, &PieChart::updateData);
}

PieChart::~PieChart()
{
}

RangeGroup *PieChart::range() const
{
    return d->range;
}

qreal PieChart::borderWidth() const
{
    return d->borderWidth;
}

ChartDataSource *PieChart::valueSource() const
{
    return d->valueSource;
}

ChartDataSource *PieChart::colorSource() const
{
    return d->colorSource;
}

void PieChart::setBorderWidth(qreal width)
{
    if (qFuzzyCompare(width, d->borderWidth))
        return;

    d->borderWidth = width;
    update();
    emit borderWidthChanged();
}

void PieChart::setValueSource(ChartDataSource *value)
{
    if (value == d->valueSource)
        return;

    if (d->valueSource)
        disconnect(d->valueSource, &ChartDataSource::dataChanged, this, &PieChart::updateData);

    d->valueSource = value;

    if (d->valueSource)
        connect(d->valueSource, &ChartDataSource::dataChanged, this, &PieChart::updateData);

    updateData();
    emit valueSourceChanged();
}

void PieChart::setColorSource(ChartDataSource *color)
{
    if (color == d->colorSource)
        return;

    if (d->colorSource)
        disconnect(d->colorSource, &ChartDataSource::dataChanged, this, &PieChart::updateData);

    d->colorSource = color;

    if (d->colorSource)
        connect(d->colorSource, &ChartDataSource::dataChanged, this, &PieChart::updateData);

    updateData();
    emit colorSourceChanged();
}

QColor PieChart::backgroundColor() const
{
    return d->backgroundColor;
}

void PieChart::setBackgroundColor(const QColor &color)
{
    if (color == d->backgroundColor) {
        return;
    }
    d->backgroundColor = color;
    update();
    emit backgroundColorChanged();
}

QSGNode *PieChart::updatePaintNode(QSGNode *node, UpdatePaintNodeData *data)
{
    Q_UNUSED(data);
    PieChartNode *n = static_cast<PieChartNode *>(node);
    if (!n) {
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

    if (!d->valueSource || !d->colorSource)
        return;

    if (!d->range->isValid())
        return;

    qreal threshold = !d->range->automatic() ? d->range->from() : 0.0;
    qreal total = 0.0;
    QVector<qreal> data;
    for (int i = 0; i < d->valueSource->itemCount(); ++i) {
        auto value = d->valueSource->item(i).toReal();
        auto limited = value - threshold;
        if (limited > 0.0) {
            data << limited;
            total += limited;

            auto color = d->colorSource->item(i).value<QColor>();
            d->colors << color;
        }
        threshold = qMax(0.0, threshold - value);
    }

    if (qFuzzyCompare(total, 0.0))
        return;

    qreal max = total;

    if (!d->range->automatic() && d->range->distance() >= total) {
        max = d->range->distance();
    }

    for (auto value : data) {
        d->sections << value / max;
    }

    if (!qFuzzyCompare(max, total) && d->backgroundColor.isValid()) {
        d->sections << (max - total) / max;
        d->colors << d->backgroundColor;
    }

    update();
}
