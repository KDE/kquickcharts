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

/*!
 * \qmltype XYChart
 * \inherits Chart
 * \inqmlmodule org.kde.quickcharts
 *
 * \brief A base class for Charts that are based on an X/Y grid.
 */
class QUICKCHARTS_EXPORT XYChart : public Chart
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Base Class")

public:
    /*!
     * \enum XYChart::Direction
     *
     * The direction of values on the X axis.
     *
     * "Start" is defined as the starting direction of the chart, when using a
     * left-to-right language it will be the left side, with a right-to-left
     * language it will be right.
     *
     * \value ZeroAtStart
     *        Zero is at the beginning of the chart, values run from begin to end.
     * \value ZeroAtEnd
     *        Zero is at the end of the chart, values run from end to begin.
     */
    enum class Direction {
        ZeroAtStart,
        ZeroAtEnd
    };
    Q_ENUM(Direction)

    explicit XYChart(QQuickItem *parent = nullptr);
    ~XYChart() override = default;

    /*!
     * \qmlproperty real XYChart::xRange.from
     * \qmlproperty real XYChart::xRange.to
     * \qmlproperty bool XYChart::xRange.automatic
     * \qmlproperty real XYChart::xRange.distance
     * \qmlproperty real XYChart::xRange.minimum
     * \qmlproperty real XYChart::xRange.increment
     *
     * \brief The range of values on the X axis.
     *
     * from: The start of this range. The default is 0.
     *
     * to: The end of this range. The default is 100.
     *
     * automatic: Whether to determine the range based on values of a chart. If true (the default), from and to are ignored and instead calculated from the
     * minimum and maximum values of a chart's valueSources.
     *
     * distance: The distance between from and to.
     *
     * minimum: The minimum size of the range. This is mostly relevant when automatic is true. Setting this value will ensure that the range will never be
     * smaller than this value. The default is std::numeric_limits<qreal>::min, which means minimum is disabled.
     *
     * increment: The amount with which the range increases. The total range will be limited to a multiple of this value. This is mostly useful when automatic
     * is true. The default is 0.0, which means do not limit the range increment.
     */
    Q_PROPERTY(RangeGroup *xRange READ xRange CONSTANT)
    virtual RangeGroup *xRange() const;
    /*!
     * \qmlproperty real XYChart::yRange.from
     * \qmlproperty real XYChart::yRange.to
     * \qmlproperty bool XYChart::yRange.automatic
     * \qmlproperty real XYChart::yRange.distance
     * \qmlproperty real XYChart::yRange.minimum
     * \qmlproperty real XYChart::yRange.increment
     *
     * \brief The range of values on the Y axis.
     *
     * from: The start of this range. The default is 0.
     *
     * to: The end of this range. The default is 100.
     *
     * automatic: Whether to determine the range based on values of a chart. If true (the default), from and to are ignored and instead calculated from the
     * minimum and maximum values of a chart's valueSources.
     *
     * distance: The distance between from and to.
     *
     * minimum: The minimum size of the range. This is mostly relevant when automatic is true. Setting this value will ensure that the range will never be
     * smaller than this value. The default is std::numeric_limits<qreal>::min, which means minimum is disabled.
     *
     * increment: The amount with which the range increases. The total range will be limited to a multiple of this value. This is mostly useful when automatic
     * is true. The default is 0.0, which means do not limit the range increment.
     */
    Q_PROPERTY(RangeGroup *yRange READ yRange CONSTANT)
    virtual RangeGroup *yRange() const;
    /*!
     * \qmlproperty enumeration XYChart::direction
     * \qmlenumeratorsfrom XYChart::Direction
     * \brief Which direction this chart's X axis runs.
     */
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    virtual XYChart::Direction direction() const;
    virtual void setDirection(XYChart::Direction newDirection);
    Q_SIGNAL void directionChanged();
    /*!
     * \qmlproperty bool XYChart::stacked
     * \brief Whether the values of each value source should be stacked.
     *
     * When true, Y values will be added on top of each other. The precise
     * meaning of this property depends on the specific chart. The default is
     * false.
     */
    Q_PROPERTY(bool stacked READ stacked WRITE setStacked NOTIFY stackedChanged)
    bool stacked() const;
    void setStacked(bool newStacked);
    Q_SIGNAL void stackedChanged();

    /*
     * Get the complete, calculated range for this chart.
     */
    ComputedRange computedRange() const;
    /*
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
