/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
