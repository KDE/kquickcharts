/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2019  <copyright holder> <email>
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

#include "Chart.h"
#include "datasource/ChartDataSource.h"

Chart::Chart(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

Chart::~Chart()
{
}

ChartDataSource* Chart::nameSource() const
{
    return m_nameSource;
}

void Chart::setNameSource(ChartDataSource* nameSource)
{
    if (m_nameSource == nameSource) {
        return;
    }

    m_nameSource = nameSource;
    Q_EMIT nameSourceChanged();
}

ChartDataSource* Chart::colorSource() const
{
    return m_colorSource;
}

void Chart::setColorSource(ChartDataSource* colorSource)
{
    if (m_colorSource == colorSource) {
        return;
    }

    m_colorSource = colorSource;
    Q_EMIT colorSourceChanged();
}

Chart::DataSourcesProperty Chart::valueSourcesProperty()
{
    return DataSourcesProperty{
        this,
        this,
        &Chart::appendSource,
        &Chart::sourceCount,
        &Chart::source,
        &Chart::clearSources
    };
}

QVector<ChartDataSource *> Chart::valueSources() const
{
    return m_valueSources;
}

void Chart::insertValueSource(int index, ChartDataSource* source)
{
    if (index < 0) {
        return;
    }

    m_valueSources.insert(index, source);
    connect(source, &QObject::destroyed, this, qOverload<QObject*>(&Chart::removeValueSource));
    connect(source, &ChartDataSource::dataChanged, this, &Chart::onDataChanged);

    onDataChanged();
    Q_EMIT valueSourcesChanged();
}

void Chart::removeValueSource(int index)
{
    if (index < 0 || index >= m_valueSources.count()) {
        return;
    }

    m_valueSources.at(index)->disconnect(this);
    m_valueSources.remove(index);

    onDataChanged();
    Q_EMIT valueSourcesChanged();
}

void Chart::removeValueSource(QObject* source)
{
    removeValueSource(m_valueSources.indexOf(qobject_cast<ChartDataSource *>(source)));
}

void Chart::componentComplete()
{
    QQuickItem::componentComplete();
    onDataChanged();
}

void Chart::appendSource(Chart::DataSourcesProperty* list, ChartDataSource* source)
{
    auto chart = reinterpret_cast<Chart*>(list->data);
    chart->m_valueSources.append(source);
    QObject::connect(source, &ChartDataSource::dataChanged, chart, &Chart::onDataChanged);
    chart->onDataChanged();
}

int Chart::sourceCount(Chart::DataSourcesProperty* list)
{
    return reinterpret_cast<Chart*>(list->data)->m_valueSources.count();
}

ChartDataSource * Chart::source(Chart::DataSourcesProperty* list, int index)
{
    return reinterpret_cast<Chart*>(list->data)->m_valueSources.at(index);
}

void Chart::clearSources(Chart::DataSourcesProperty* list)
{
    auto chart = reinterpret_cast<Chart*>(list->data);
    std::for_each(chart->m_valueSources.cbegin(), chart->m_valueSources.cend(), [chart](ChartDataSource* source) {
        source->disconnect(chart);
    });
    chart->m_valueSources.clear();
    chart->onDataChanged();
}
