#include "ChartAxisSource.h"

#include <QVariant>

ChartAxisSource::ChartAxisSource(QObject* parent)
    : ChartDataSource(parent)
{
    connect(this, &ChartAxisSource::itemCountChanged, this, &ChartAxisSource::dataChanged);
    connect(this, &ChartAxisSource::chartChanged, this, &ChartAxisSource::dataChanged);
    connect(this, &ChartAxisSource::axisChanged, this, &ChartAxisSource::dataChanged);
}

ChartAxisSource::~ChartAxisSource()
{
}

QVariant ChartAxisSource::item(int index) const
{
    if (index < 0 || index > m_itemCount)
        return QVariant{};

    return index;
}

QVariant ChartAxisSource::minimum() const
{
    return QVariant{};
}

QVariant ChartAxisSource::maximum() const
{
    return QVariant{};
}

QQuickItem * ChartAxisSource::chart() const
{
    return m_chart;
}

void ChartAxisSource::setChart(QQuickItem * newChart)
{
    if (newChart == m_chart) {
        return;
    }

    m_chart = newChart;
    Q_EMIT chartChanged();
}

ChartAxisSource::Axis ChartAxisSource::axis() const
{
    return m_axis;
}

void ChartAxisSource::setAxis(ChartAxisSource::Axis newAxis)
{
    if (newAxis == m_axis) {
        return;
    }

    m_axis = newAxis;
    Q_EMIT axisChanged();
}

int ChartAxisSource::itemCount() const
{
    return m_itemCount;
}

void ChartAxisSource::setItemCount(int newItemCount)
{
    if (newItemCount == m_itemCount) {
        return;
    }

    m_itemCount = newItemCount;
    Q_EMIT itemCountChanged();
}
