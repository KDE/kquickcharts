#include "XYChart.h"

#include "RangeGroup.h"

XYChart::XYChart(QQuickItem* parent)
    : QQuickItem(parent)
{
    m_xRange = new RangeGroup{this};
    connect(m_xRange, &RangeGroup::rangeChanged, this, &XYChart::updateComputedRange);
    m_yRange = new RangeGroup{this};
    connect(m_yRange, &RangeGroup::rangeChanged, this, &XYChart::updateComputedRange);
}

XYChart::~XYChart()
{
}

RangeGroup* XYChart::xRange() const
{
    return m_xRange;
}

RangeGroup* XYChart::yRange() const
{
    return m_yRange;
}

XYChart::Direction XYChart::direction() const
{
    return m_direction;
}

void XYChart::setDirection(XYChart::Direction newDirection)
{
    if (newDirection == m_direction) {
        return;
    }

    m_direction = newDirection;
    Q_EMIT directionChanged();
}

const ComputedRange XYChart::computedRange() const
{
    return m_computedRange;
}

void XYChart::updateComputedRange()
{
    ComputedRange result;

    if (m_xRange->automatic()) {
        updateAutomaticXRange(result);
    } else {
        result.startX = m_xRange->from();
        result.endX = m_xRange->to();
    }
    result.distanceX = result.endX - result.startX;

    if(m_yRange->automatic()) {
        updateAutomaticYRange(result);
    } else {
        result.startY = m_yRange->from();
        result.endY = m_yRange->to();
    }
    result.distanceY = result.endY - result.startY;

    m_computedRange = result;
    Q_EMIT computedRangeChanged();
}
