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

#include "ModelSource.h"

/**
 * A data source that watches a QAbstractItemModel cell and provides the history of that cell as data.
 */
class ModelHistorySource : public ModelSource
{
    Q_OBJECT
    Q_PROPERTY(int row READ row WRITE setRow NOTIFY rowChanged)
    Q_PROPERTY(int maximumHistory READ maximumHistory WRITE setMaximumHistory NOTIFY maximumHistoryChanged)

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

    Q_INVOKABLE void clear();

private:
    void onModelChanged();
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

    int m_row;
    int m_maximumHistory;
    QVariantList m_history;
};

#endif // MODELHISTORYSOURCE_H
