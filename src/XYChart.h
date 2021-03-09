/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef XYCHART_H
#define XYCHART_H

#include "Chart.h"

class RangeGroup;

/**
 * A helper containing the calculated X and Y ranges of a chart.
 */
struct ComputedRange {
    int startX = 0;
    int endX = 0;
    int distanceX = 0;
    float startY = 0.0;
    float endY = 0.0;
    float distanceY = 0.0;
};

bool operator==(const ComputedRange &first, const ComputedRange &second);

/**
 * A base class for Charts that are based on an X/Y grid.
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
     * Whether the values of each value source should be stacked.
     *
     * When true, Y values will be added on top of each other. The precise
     * meaning of this property depends on the specific chart. The default is
     * false.
     */
    Q_PROPERTY(bool stacked READ stacked WRITE setStacked NOTIFY stackedChanged)

public:
    /**
     * The direction of values on the X axis.
     *
     * "Start" is defined as the starting direction of the chart, when using a
     * left-to-right language it will be the left side, with a right-to-left
     * language it will be right.
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

    /**
     * Get the complete, calculated range for this chart.
     */
    ComputedRange computedRange() const;
    /**
     * Emitted whenever the complete range is recalculated.
     */
    Q_SIGNAL void computedRangeChanged();

protected:
    /**
     * Re-calculate the chart's range.
     *
     * By default, this will make use of the xRange/yRange properties and
     * calculate a proper range. This method can be overridden by subclasses if
     * some special calculation is needed.
     */
    virtual void updateComputedRange();

    /**
     * Set the computed range.
     *
     * \param range The new range.
     */
    void setComputedRange(ComputedRange range);

private:
    RangeGroup *m_xRange = nullptr;
    RangeGroup *m_yRange = nullptr;
    Direction m_direction = Direction::ZeroAtStart;
    bool m_stacked = false;
    ComputedRange m_computedRange;
};

QDebug operator<<(QDebug debug, const ComputedRange &range);

#endif // XYCHART_H
