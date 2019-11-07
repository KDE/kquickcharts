/*
 * This file is part of KQuickCharts
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

#ifndef ARRAYSOURCE_H
#define ARRAYSOURCE_H

#include <QVariantList>

#include "ChartDataSource.h"

/**
 * A data source that provides entries of an array as data.
 */
class ArraySource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY(QVariantList array READ array WRITE setArray NOTIFY dataChanged)
    Q_PROPERTY(bool wrap READ wrap WRITE setWrap NOTIFY dataChanged)

public:
    /**
     * Constructor
     *
     * @param parent TODO
     */
    explicit ArraySource(QObject *parent = nullptr);

    virtual int itemCount() const override;
    virtual QVariant item(int index) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

    QVariantList array() const;
    void setArray(const QVariantList &array);

    bool wrap() const;
    void setWrap(bool wrap);

private:
    QVariantList m_array;
    bool m_wrap = false;
};

#endif // ARRAYSOURCE_H
