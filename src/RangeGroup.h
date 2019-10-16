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

#include <functional>

#include <QObject>

#include "namespace.h"

BEGIN_NAMESPACE

class ChartDataSource;

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
    /**
     * The minimum size of the range. Mostly relevant when automatic is true.
     */
    Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    /**
     * The amount with which the range increases. That is, the total range will be limited to a multiple of this value.
     */
    Q_PROPERTY(qreal increment READ increment WRITE setIncrement NOTIFY incrementChanged)

public:
    struct RangeResult {
        qreal start = 0.0;
        qreal end = 0.0;
        qreal distance = 0.0;
    };

    explicit RangeGroup(QObject *parent = nullptr);

    qreal from() const;
    void setFrom(qreal from);
    Q_SIGNAL void fromChanged();

    qreal to() const;
    void setTo(qreal to);
    Q_SIGNAL void toChanged();

    bool automatic() const;
    void setAutomatic(bool newAutomatic);
    Q_SIGNAL void automaticChanged();

    qreal distance() const;

    qreal minimum() const;
    void setMinimum(qreal newMinimum);
    Q_SIGNAL void minimumChanged();

    qreal increment() const;
    void setIncrement(qreal newIncrement);
    Q_SIGNAL void incrementChanged();

    bool isValid() const;

    Q_SIGNAL void rangeChanged();

    RangeResult calculateRange(const QVector<ChartDataSource*> &sources,
                               std::function<qreal(ChartDataSource*)> minimumCallback,
                               std::function<qreal(ChartDataSource*)> maximumCallback);

private:
    qreal m_from = 0.0;
    qreal m_to = 100.0;
    bool m_automatic = true;
    qreal m_minimum = std::numeric_limits<qreal>::min();
    qreal m_increment = 0.0;
};

END_NAMESPACE

#endif // RANGEGROUP_H
