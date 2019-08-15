/*
 * This file is part of Quick Charts.
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHART_H
#define CHART_H

#include <QQuickItem>

#include "namespace.h"

BEGIN_NAMESPACE

class ChartDataSource;

/**
 * Abstract base class for all charts.
 */
class Chart : public QQuickItem
{
    Q_OBJECT
    /**
     * The data source to use for names of chart items.
     */
    Q_PROPERTY(ChartDataSource* nameSource READ nameSource WRITE setNameSource NOTIFY nameSourceChanged)
    /**
     * The data source to use for colors of chart items.
     */
    Q_PROPERTY(ChartDataSource* colorSource READ colorSource WRITE setColorSource NOTIFY colorSourceChanged)
    /**
     * The data sources providing the data this chart needs to render.
     */
    Q_PROPERTY(QQmlListProperty<ChartDataSource> valueSources READ valueSourcesProperty NOTIFY valueSourcesChanged)

public:
    using DataSourcesProperty = QQmlListProperty<ChartDataSource>;

    explicit Chart(QQuickItem *parent = nullptr);
    ~Chart() override = default;

    ChartDataSource *nameSource() const;
    Q_SLOT void setNameSource(ChartDataSource *nameSource);
    Q_SIGNAL void nameSourceChanged();

    ChartDataSource *colorSource() const;
    Q_SLOT void setColorSource(ChartDataSource *colorSource);
    Q_SIGNAL void colorSourceChanged();

    DataSourcesProperty valueSourcesProperty();
    QVector<ChartDataSource *> valueSources() const;
    Q_SIGNAL void valueSourcesChanged();
    Q_SLOT void insertValueSource(int index, ChartDataSource *source);
    Q_SLOT void removeValueSource(int index);
    Q_SLOT void removeValueSource(QObject *source);

protected:
    /**
     * Called when the data of a value source changes.
     *
     * This method should be reimplemented by subclasses. It is called whenever
     * the data of one of the value sources changes. Subclasses should use this
     * to make sure that they update whatever internal state they use for
     * rendering, then call update() to schedule rendering the item.
     */
    Q_SLOT virtual void onDataChanged() = 0;
    void componentComplete() override;

private:
    static void appendSource(DataSourcesProperty *list, ChartDataSource *source);
    static int sourceCount(DataSourcesProperty *list);
    static ChartDataSource *source(DataSourcesProperty *list, int index);
    static void clearSources(DataSourcesProperty *list);

    ChartDataSource *m_nameSource = nullptr;
    ChartDataSource *m_colorSource = nullptr;
    QVector<ChartDataSource *> m_valueSources;
};

END_NAMESPACE

#endif // CHART_H
