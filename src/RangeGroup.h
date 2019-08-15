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

#ifndef RANGEGROUP_H
#define RANGEGROUP_H

#include <QObject>

#include "namespace.h"

BEGIN_NAMESPACE

/**
 * An object that can be used as a grouped property to provide a range for charts.
 */
class RangeGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(qreal to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(bool automatic READ automatic WRITE setAutomatic NOTIFY automaticChanged)
    Q_PROPERTY(qreal distance READ distance NOTIFY rangeChanged)

public:
    explicit RangeGroup(QObject* parent = nullptr);

    qreal from() const;
    qreal to() const;
    bool automatic() const;

    bool isValid() const;
    qreal distance() const;

public Q_SLOTS:
    void setFrom(qreal from);
    void setTo(qreal to);
    void setAutomatic(bool automatic);

Q_SIGNALS:
    void fromChanged();
    void toChanged();
    void automaticChanged();
    void rangeChanged();
    void distanceChanged();

private:
    qreal m_from = 0.0;
    qreal m_to = 100.0;
    bool m_automatic = true;
};

END_NAMESPACE

#endif // RANGEGROUP_H
