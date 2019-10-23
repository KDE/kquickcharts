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

#include "ArraySource.h"

ArraySource::ArraySource(QObject *parent)
    : ChartDataSource(parent)
{
}

int ArraySource::itemCount() const
{
    return m_array.count();
}

QVariant ArraySource::item(int index) const
{
    if (!m_wrap && (index < 0 || index > m_array.count() - 1))
        return QVariant{};

    return m_array.at(index % m_array.count());
}

QVariant ArraySource::minimum() const
{
    return *std::min_element(m_array.begin(), m_array.end());
}

QVariant ArraySource::maximum() const
{
    return *std::max_element(m_array.begin(), m_array.end());
}

QVariantList ArraySource::array() const
{
    return m_array;
}

bool ArraySource::wrap() const
{
    return m_wrap;
}

void ArraySource::setArray(const QVariantList &array)
{
    if (m_array == array) {
        return;
    }

    m_array = array;
    Q_EMIT dataChanged();
}

void ArraySource::setWrap(bool wrap)
{
    if (m_wrap == wrap) {
        return;
    }

    m_wrap = wrap;
    Q_EMIT dataChanged();
}
