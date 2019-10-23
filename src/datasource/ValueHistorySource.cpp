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

#include "ValueHistorySource.h"

#include <QDebug>

ValueHistorySource::ValueHistorySource(QObject *parent)
    : ChartDataSource(parent)
{
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
    if (item != m_history.cend())
        return *item;

    return QVariant{};
}

QVariant ValueHistorySource::maximum() const
{
    auto item = std::max_element(m_history.cbegin(), m_history.cend());
    if (item != m_history.cend())
        return *item;

    return QVariant{};
}

QVariant ValueHistorySource::value() const
{
    return m_value;
}

void ValueHistorySource::setValue(const QVariant &newValue)
{
    m_value = newValue;

    m_history.prepend(newValue);
    while (m_history.size() > m_maximumHistory) {
        m_history.removeLast();
    }

    Q_EMIT dataChanged();
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

void ValueHistorySource::clear()
{
    m_history.clear();
    Q_EMIT dataChanged();
}
