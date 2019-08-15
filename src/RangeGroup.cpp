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

#include "RangeGroup.h"

RangeGroup::RangeGroup(QObject *parent)
    : QObject(parent)
{
    connect(this, &RangeGroup::fromChanged, this, &RangeGroup::rangeChanged);
    connect(this, &RangeGroup::toChanged, this, &RangeGroup::rangeChanged);
    connect(this, &RangeGroup::automaticChanged, this, &RangeGroup::rangeChanged);
}

qreal RangeGroup::from() const
{
    return m_from;
}

qreal RangeGroup::to() const
{
    return m_to;
}

bool RangeGroup::automatic() const
{
    return m_automatic;
}

bool RangeGroup::isValid() const
{
    return m_automatic || (m_to > m_from);
}

qreal RangeGroup::distance() const
{
    return m_to - m_from;
}

void RangeGroup::setFrom(qreal from)
{
    if (qFuzzyCompare(m_from, from)) {
        return;
    }

    m_from = from;
    Q_EMIT fromChanged();
}

void RangeGroup::setTo(qreal to)
{
    if (qFuzzyCompare(m_to, to)) {
        return;
    }

    m_to = to;
    Q_EMIT toChanged();
}

void RangeGroup::setAutomatic(bool automatic)
{
    if (m_automatic == automatic) {
        return;
    }

    m_automatic = automatic;
    Q_EMIT automaticChanged();
}
