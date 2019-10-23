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

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>

/**
 * Abstract base class for data sources.
 */
class ChartDataSource : public QObject
{
    Q_OBJECT

public:
    explicit ChartDataSource(QObject *parent = nullptr);
    virtual ~ChartDataSource() = default;

    virtual int itemCount() const = 0;
    virtual QVariant item(int index) const = 0;
    virtual QVariant minimum() const = 0;
    virtual QVariant maximum() const = 0;

    Q_SIGNAL void dataChanged();
};

#endif // DATASOURCE_H
