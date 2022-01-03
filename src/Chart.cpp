/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "Chart.h"
#include "datasource/ChartDataSource.h"

Chart::Chart(QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    connect(this, &Chart::dataChanged, this, &Chart::onDataChanged);
}

ChartDataSource *Chart::nameSource() const
{
    return m_nameSource;
}

void Chart::setNameSource(ChartDataSource *nameSource)
{
    if (m_nameSource == nameSource) {
        return;
    }

    m_nameSource = nameSource;
    Q_EMIT dataChanged();
    Q_EMIT nameSourceChanged();
}

ChartDataSource *Chart::shortNameSource() const
{
    return m_shortNameSource;
}

void Chart::setShortNameSource(ChartDataSource *shortNameSource)
{
    if (m_shortNameSource == shortNameSource) {
        return;
    }

    m_shortNameSource = shortNameSource;
    Q_EMIT dataChanged();
    Q_EMIT shortNameSourceChanged();
}

ChartDataSource *Chart::colorSource() const
{
    return m_colorSource;
}

void Chart::setColorSource(ChartDataSource *colorSource)
{
    if (m_colorSource == colorSource) {
        return;
    }

    if (m_colorSource) {
        disconnect(m_colorSource, &ChartDataSource::dataChanged, this, &Chart::dataChanged);
    }

    m_colorSource = colorSource;

    if (m_colorSource) {
        connect(m_colorSource, &ChartDataSource::dataChanged, this, &Chart::dataChanged);
    }

    Q_EMIT dataChanged();
    Q_EMIT colorSourceChanged();
}

Chart::DataSourcesProperty Chart::valueSourcesProperty()
{
    return DataSourcesProperty{this, this, &Chart::appendSource, &Chart::sourceCount, &Chart::source, &Chart::clearSources};
}

QVector<ChartDataSource *> Chart::valueSources() const
{
    return m_valueSources;
}

void Chart::insertValueSource(int index, ChartDataSource *source)
{
    if (index < 0) {
        return;
    }

    m_valueSources.insert(index, source);
    connect(source, &QObject::destroyed, this, qOverload<QObject *>(&Chart::removeValueSource));
    connect(source, &ChartDataSource::dataChanged, this, &Chart::dataChanged);

    Q_EMIT dataChanged();
    Q_EMIT valueSourcesChanged();
}

void Chart::removeValueSource(int index)
{
    if (index < 0 || index >= m_valueSources.count()) {
        return;
    }

    m_valueSources.at(index)->disconnect(this);
    m_valueSources.remove(index);

    Q_EMIT dataChanged();
    Q_EMIT valueSourcesChanged();
}

void Chart::removeValueSource(QObject *source)
{
    auto itr = std::find_if(m_valueSources.begin(), m_valueSources.end(), [source](QObject *dataSource) {
        return dataSource == source;
    });

    if (itr != m_valueSources.end()) {
        (*itr)->disconnect(this);
        m_valueSources.erase(itr);
    }

    Q_EMIT dataChanged();
    Q_EMIT valueSourcesChanged();
}

Chart::IndexingMode Chart::indexingMode() const
{
    return m_indexingMode;
}

void Chart::setIndexingMode(IndexingMode newIndexingMode)
{
    if (newIndexingMode == m_indexingMode) {
        return;
    }

    m_indexingMode = newIndexingMode;
    Q_EMIT dataChanged();
    Q_EMIT indexingModeChanged();
}

void Chart::componentComplete()
{
    QQuickItem::componentComplete();
    Q_EMIT dataChanged();
}

void Chart::appendSource(Chart::DataSourcesProperty *list, ChartDataSource *source)
{
    auto chart = reinterpret_cast<Chart *>(list->data);
    chart->insertValueSource(chart->valueSources().size(), source);
}
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
int Chart::sourceCount(Chart::DataSourcesProperty *list)
#else
qsizetype Chart::sourceCount(Chart::DataSourcesProperty *list)
#endif
{
    return reinterpret_cast<Chart *>(list->data)->m_valueSources.count();
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
ChartDataSource *Chart::source(Chart::DataSourcesProperty *list, int index)
#else
ChartDataSource *Chart::source(Chart::DataSourcesProperty *list, qsizetype index)
#endif
{
    return reinterpret_cast<Chart *>(list->data)->m_valueSources.at(index);
}

void Chart::clearSources(Chart::DataSourcesProperty *list)
{
    auto chart = reinterpret_cast<Chart *>(list->data);
    std::for_each(chart->m_valueSources.cbegin(), chart->m_valueSources.cend(), [chart](ChartDataSource *source) {
        source->disconnect(chart);
    });
    chart->m_valueSources.clear();
    Q_EMIT chart->dataChanged();
}
