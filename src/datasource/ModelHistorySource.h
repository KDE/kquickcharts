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

#ifndef MODELHISTORYSOURCE_H
#define MODELHISTORYSOURCE_H

#include <memory>
#include <QTimer>
#include "ModelSource.h"

/**
 * A data source that watches a QAbstractItemModel cell and provides the history of that cell as data.
 */
class ModelHistorySource : public ModelSource
{
    Q_OBJECT
    // The row to read data from. The items of this source will be the values of
    // that row, using the column and role from ModelSource.
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    // The maximum amount of history to keep.
    Q_PROPERTY(int maximumHistory READ maximumHistory WRITE setMaximumHistory NOTIFY maximumHistoryChanged)
    // The update interval. If not set or set to a value < 0, a new item will be
    // added whenever the underlying model changes. Otherwise, the source will
    // sample the underlying model every interval milliseconds and add a new item
    // with whatever value the model has at that point - even if it did not change.
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)

public:
    explicit ModelHistorySource(QObject *parent = nullptr);

    virtual int itemCount() const override;
    virtual QVariant item(int index) const override;
    virtual QVariant minimum() const override;
    virtual QVariant maximum() const override;

    int row() const;
    void setRow(int row);
    Q_SIGNAL void rowChanged();

    int maximumHistory() const;
    void setMaximumHistory(int maximumHistory);
    Q_SIGNAL void maximumHistoryChanged();

    int interval() const;
    void setInterval(int newInterval);
    Q_SIGNAL void intervalChanged();

    Q_INVOKABLE void clear();

private:
    void onModelChanged();
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

    int m_row;
    int m_maximumHistory;
    QVariantList m_history;
    std::unique_ptr<QTimer> m_updateTimer;
};

#endif // MODELHISTORYSOURCE_H
