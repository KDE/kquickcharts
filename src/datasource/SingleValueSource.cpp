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

#include "SingleValueSource.h"

SingleValueSource::SingleValueSource ( QObject* parent )
    : ChartDataSource(parent)
{

}

SingleValueSource::~SingleValueSource()
{
}

QVariant SingleValueSource::item ( int index ) const
{
    Q_UNUSED(index);
    return m_value;
}

int SingleValueSource::itemCount() const
{
    return 1;
}

QVariant SingleValueSource::minimum() const
{
    return m_value;
}

QVariant SingleValueSource::maximum() const
{
    return m_value;
}

QVariant SingleValueSource::value() const
{
    return m_value;
}

void SingleValueSource::setValue ( const QVariant& value )
{
    if ( m_value == value ) {
        return;
    }

    m_value = value;
    Q_EMIT dataChanged();
}
