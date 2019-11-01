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

#ifndef VALUEHISTORYSOURCE_H
#define VALUEHISTORYSOURCE_H

#include <memory>
#include <QVariant>
#include <QVector>
#include <QTimer>

#include "ChartDataSource.h"

/**
 * A data source that provides a history of a single value.
 */
class ValueHistorySource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY dataChanged)
    Q_PROPERTY(int maximumHistory READ maximumHistory WRITE setMaximumHistory NOTIFY maximumHistoryChanged)
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)

public:
    explicit ValueHistorySource(QObject *parent = nullptr);

    int itemCount() const override;
    QVariant item(int index) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

    QVariant value() const;
    void setValue(const QVariant &value);

    int maximumHistory() const;
    void setMaximumHistory(int maximumHistory);
    Q_SIGNAL void maximumHistoryChanged();

    int interval() const;
    void setInterval(int newInterval);
    Q_SIGNAL void intervalChanged();

    Q_INVOKABLE void clear();

private:
    QVariant m_value;
    int m_maximumHistory = 10;
    std::unique_ptr<QTimer> m_updateTimer;
    QVector<QVariant> m_history;
};

#endif // VALUEHISTORYSOURCE_H
