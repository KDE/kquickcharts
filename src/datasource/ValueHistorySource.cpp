/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "ValueHistorySource.h"

#include "QmlDeprecated.h"

#if QUICKCHARTS_BUILD_DEPRECATED_SINCE(5, 78)

ValueHistorySource::ValueHistorySource(QObject *parent)
    : ChartDataSource(parent)
{
    QML_DEPRECATED("ValueHistorySource", "5.78", "Use HistoryProxySource instead")
}

int ValueHistorySource::itemCount() const
{
    return m_maximumHistory;
}

QVariant ValueHistorySource::item(int index) const
{
    if (index < 0 || index >= m_history.count()) {
        return QVariant{};
    }

    return m_history.at(index);
}

QVariant ValueHistorySource::minimum() const
{
    auto item = std::min_element(m_history.cbegin(), m_history.cend());
    if (item != m_history.cend()) {
        return *item;
    }

    return QVariant{};
}

QVariant ValueHistorySource::maximum() const
{
    auto item = std::max_element(m_history.cbegin(), m_history.cend());
    if (item != m_history.cend()) {
        return *item;
    }

    return QVariant{};
}

QVariant ValueHistorySource::value() const
{
    return m_value;
}

void ValueHistorySource::setValue(const QVariant &newValue)
{
    m_value = newValue;

    if (!m_updateTimer) {
        m_history.prepend(newValue);
        while (m_history.size() > m_maximumHistory) {
            m_history.removeLast();
        }

        Q_EMIT dataChanged();
    }
}

int ValueHistorySource::maximumHistory() const
{
    return m_maximumHistory;
}

void ValueHistorySource::setMaximumHistory(int newMaximumHistory)
{
    if (newMaximumHistory == m_maximumHistory) {
        return;
    }

    m_maximumHistory = newMaximumHistory;
    while (m_history.size() > m_maximumHistory) {
        m_history.removeLast();
    }
    Q_EMIT maximumHistoryChanged();
}

int ValueHistorySource::interval() const
{
    return m_updateTimer ? m_updateTimer->interval() : -1;
}

void ValueHistorySource::setInterval(int newInterval)
{
    if (m_updateTimer && newInterval == m_updateTimer->interval()) {
        return;
    }

    if (newInterval > 0) {
        if (!m_updateTimer) {
            m_updateTimer = std::make_unique<QTimer>();
            // See ModelHistorySource.cpp line 110
            m_updateTimer->setTimerType(Qt::PreciseTimer);
            connect(m_updateTimer.get(), &QTimer::timeout, this, [this]() {
                m_history.prepend(m_value);
                while (m_history.size() > m_maximumHistory) {
                    m_history.removeLast();
                }
                Q_EMIT dataChanged();
            });
        }
        m_updateTimer->setInterval(newInterval);
        m_updateTimer->start();
    } else {
        m_updateTimer.reset();
    }

    Q_EMIT intervalChanged();
}

void ValueHistorySource::clear()
{
    m_history.clear();
    Q_EMIT dataChanged();
}

#endif // QUICKCHARTS_BUILD_DEPRECATED_SINCE
