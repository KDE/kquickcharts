/*
 * This file is part of KQuickCharts
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

#include "XYChart.h"

#include "RangeGroup.h"
#include "datasource/ChartDataSource.h"

bool operator==(const ComputedRange &first, const ComputedRange &second)
{
    return first.startX == second.startX && first.endX == second.endX && qFuzzyCompare(first.startY, second.startY)
        && qFuzzyCompare(first.endY, second.endY);
}

XYChart::XYChart(QQuickItem *parent)
    : Chart(parent)
{
    m_xRange = new RangeGroup{this};
    connect(m_xRange, &RangeGroup::rangeChanged, this, &XYChart::updateComputedRange);
    m_yRange = new RangeGroup{this};
    connect(m_yRange, &RangeGroup::rangeChanged, this, &XYChart::updateComputedRange);
}

RangeGroup *XYChart::xRange() const
{
    return m_xRange;
}

RangeGroup *XYChart::yRange() const
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
    onDataChanged();
    Q_EMIT directionChanged();
}

bool XYChart::stacked() const
{
    return m_stacked;
}

void XYChart::setStacked(bool newStacked)
{
    if (newStacked == m_stacked) {
        return;
    }

    m_stacked = newStacked;
    onDataChanged();
    Q_EMIT stackedChanged();
}

ComputedRange XYChart::computedRange() const
{
    return m_computedRange;
}

void XYChart::updateComputedRange()
{
    if (valueSources().count() == 0) {
        return;
    }

    ComputedRange result;

    auto xRange = m_xRange->calculateRange(valueSources(),
                                           [](ChartDataSource *) { return 0; },
                                           [](ChartDataSource *source) { return source->itemCount(); });
    result.startX = xRange.start;
    result.endX = xRange.end;
    result.distanceX = xRange.distance;

    auto maximumY = [this, xRange](ChartDataSource *source) {
        if (!m_stacked) {
            return source->maximum().toDouble();
        } else {
            qreal max = std::numeric_limits<qreal>::min();
            for (int i = xRange.start; i < xRange.end; ++i) {
                qreal yDistance = 0.0;
                for (auto source : valueSources()) {
                    yDistance += source->item(i).toDouble();
                }
                max = std::max(max, yDistance);
            }
            return max;
        }
    };

    auto yRange = m_yRange->calculateRange(valueSources(),
                                           [](ChartDataSource *source) { return std::min(0.0, source->minimum().toDouble()); },
                                           maximumY);
    result.startY = yRange.start;
    result.endY = yRange.end;
    result.distanceY = yRange.distance;

    setComputedRange(result);
}

void XYChart::setComputedRange(ComputedRange range)
{
    if (range == m_computedRange) {
        return;
    }

    m_computedRange = range;
    Q_EMIT computedRangeChanged();
}

QDebug operator<<(QDebug debug, const ComputedRange &range)
{
    debug << "Range: startX" << range.startX << "endX" << range.endX << "distance" << range.distanceX << "startY" << range.startY << "endY"
          << range.endY << "distance" << range.distanceY;
    return debug;
}
