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

#ifndef SINGLEVALUESOURCE_H
#define SINGLEVALUESOURCE_H

#include <QVariant>

#include "ChartDataSource.h"

BEGIN_NAMESPACE

/**
 * A data source that provides a single value as data.
 */
class SingleValueSource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY ( QVariant value READ value WRITE setValue NOTIFY dataChanged )

public:
    explicit SingleValueSource ( QObject* parent = nullptr );
    ~SingleValueSource();

    virtual int itemCount() const override;
    virtual QVariant item ( int index ) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

    QVariant value() const;

public Q_SLOTS:
    void setValue ( const QVariant& value );

private:
    QVariant m_value;
};

END_NAMESPACE

#endif // SINGLEVALUESOURCE_H
