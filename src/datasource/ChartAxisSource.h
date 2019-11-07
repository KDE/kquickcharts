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

#ifndef CHARTAXISSOURCE_H
#define CHARTAXISSOURCE_H

#include "ChartDataSource.h"

class XYChart;

/**
 * A data source that provides values from a chart's axis as data.
 */
class ChartAxisSource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY(XYChart *chart READ chart WRITE setChart NOTIFY chartChanged)
    Q_PROPERTY(ChartAxisSource::Axis axis READ axis WRITE setAxis NOTIFY axisChanged)
    Q_PROPERTY(int itemCount READ itemCount WRITE setItemCount NOTIFY itemCountChanged)

public:
    enum class Axis { XAxis, YAxis };
    Q_ENUM(Axis)

    /**
     * Constructor
     *
     * @param parent TODO
     */
    ChartAxisSource(QObject *parent = nullptr);

    virtual QVariant item(int index) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

    XYChart *chart() const;
    Q_SLOT void setChart(XYChart *newChart);
    Q_SIGNAL void chartChanged();

    ChartAxisSource::Axis axis() const;
    Q_SLOT void setAxis(ChartAxisSource::Axis newAxis);
    Q_SIGNAL void axisChanged();

    virtual int itemCount() const override;
    Q_SLOT void setItemCount(int newItemCount);
    Q_SIGNAL void itemCountChanged();

private:
    XYChart *m_chart = nullptr;
    Axis m_axis = Axis::XAxis;
    int m_itemCount = 2;
};

#endif // ARRAYSOURCE_H
