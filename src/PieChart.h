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

#ifndef PIECHART_H
#define PIECHART_H

#include <memory>

#include "Chart.h"

class ChartDataSource;
class RangeGroup;

/**
 * A pie chart.
 */
class PieChart : public Chart
{
    Q_OBJECT

    /**
     * The range of values to display in this PieChart. When set to "automatic", the values will be divide across the
     * entire chart. .
     */
    Q_PROPERTY(RangeGroup *range READ range CONSTANT)
    /**
     * Whether to use a filled Pie or not. If true, the first Pie rendered will be rendered as a filled circle.
     */
    Q_PROPERTY(bool filled READ filled WRITE setFilled NOTIFY filledChanged)
    /**
     * The thickness of an individual pie. If filled is set, this is ignored for the first pie.
     */
    Q_PROPERTY(qreal thickness READ thickness WRITE setThickness NOTIFY thicknessChanged)
    /**
     * The amount of spacing between stacked pies.
     */
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    /**
     * Sets a colour to use to fill remaining space on the arc.
     */
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    /**
     * The starting angle of the arc used for the entire pie.
     */
    Q_PROPERTY(qreal fromAngle READ fromAngle WRITE setFromAngle NOTIFY fromAngleChanged)
    /**
     * The end angle of the arc used for the entire pie.
     */
    Q_PROPERTY(qreal toAngle READ toAngle WRITE setToAngle NOTIFY toAngleChanged)
    /**
     * Smooth the ends of sections.
     */
    Q_PROPERTY(bool smoothEnds READ smoothEnds WRITE setSmoothEnds NOTIFY smoothEndsChanged)

public:
    /**
     * Default constructor
     */
    explicit PieChart(QQuickItem *parent = nullptr);

    RangeGroup *range() const;

    bool filled() const;
    void setFilled(bool newFilled);
    Q_SIGNAL void filledChanged();

    qreal thickness() const;
    void setThickness(qreal newThickness);
    Q_SIGNAL void thicknessChanged();

    qreal spacing() const;
    void setSpacing(qreal newSpacing);
    Q_SIGNAL void spacingChanged();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);
    Q_SIGNAL void backgroundColorChanged();

    qreal fromAngle() const;
    void setFromAngle(qreal newFromAngle);
    Q_SIGNAL void fromAngleChanged();

    qreal toAngle() const;
    void setToAngle(qreal newToAngle);
    Q_SIGNAL void toAngleChanged();

    bool smoothEnds() const;
    void setSmoothEnds(bool newSmoothEnds);
    Q_SIGNAL void smoothEndsChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;
    void onDataChanged() override;

private:
    std::unique_ptr<RangeGroup> m_range;
    bool m_filled = false;
    qreal m_thickness = 10.0;
    qreal m_spacing = 0.0;
    QColor m_backgroundColor = Qt::transparent;
    qreal m_fromAngle = 0.0;
    qreal m_toAngle = 360.0;
    bool m_smoothEnds = false;

    QVector<QVector<qreal>> m_sections;
    QVector<QVector<QColor>> m_colors;
};

#endif // PIECHART_H
