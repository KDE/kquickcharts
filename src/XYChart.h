/*
 * This file is part of Quick Charts
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

#ifndef XYCHART_H
#define XYCHART_H

#include "Chart.h"

class RangeGroup;

/**
 * A helper containing the calculated X and Y ranges of a chart.
 */
struct ComputedRange
{
    int startX = 0;
    int endX = 0;
    int distanceX = 0;
    float startY = 0.0;
    float endY = 0.0;
    float distanceY = 0.0;
};

bool operator==(const ComputedRange &first, const ComputedRange &second);

/**
 * Base class for Charts that are based on an X/Y grid.
 */
class XYChart : public Chart
{
    Q_OBJECT
    /**
     * The range of values on the X axis.
     */
    Q_PROPERTY(RangeGroup *xRange READ xRange CONSTANT)
    /**
     * The range of values on the Y axis.
     */
    Q_PROPERTY(RangeGroup *yRange READ yRange CONSTANT)
    /**
     * Which direction this chart's X axis runs.
     */
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    /**
     * Whether the values of each value source should be stacked or treated separately.
     */
    Q_PROPERTY(bool stacked READ stacked WRITE setStacked NOTIFY stackedChanged)

public:
    /**
     * The direction of values on the X axis.
     */
    enum class Direction {
        ZeroAtStart, ///< Zero is at the beginning of the chart, values run from begin to end.
        ZeroAtEnd ///< Zero is at the end of the chart, values run from end to begin.
    };
    Q_ENUM(Direction)

    /**
     * Constructor
     *
     * @param parent The QObject parent.
     */
    explicit XYChart(QQuickItem *parent = nullptr);

    /**
     * Destructor
     */
    ~XYChart() override = default;

    virtual RangeGroup *xRange() const;
    virtual RangeGroup *yRange() const;

    virtual XYChart::Direction direction() const;
    virtual void setDirection(XYChart::Direction newDirection);
    Q_SIGNAL void directionChanged();

    bool stacked() const;
    void setStacked(bool newStacked);
    Q_SIGNAL void stackedChanged();

    ComputedRange computedRange() const;
    Q_SIGNAL void computedRangeChanged();

protected:
    virtual void updateComputedRange();

private:
    RangeGroup *m_xRange = nullptr;
    RangeGroup *m_yRange = nullptr;
    Direction m_direction = Direction::ZeroAtStart;
    bool m_stacked = false;
    ComputedRange m_computedRange;
};

QDebug operator<<(QDebug debug, const ComputedRange &range);

#endif // XYCHART_H
