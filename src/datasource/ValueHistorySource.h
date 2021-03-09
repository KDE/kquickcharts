/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef VALUEHISTORYSOURCE_H
#define VALUEHISTORYSOURCE_H

#include <QTimer>
#include <QVariant>
#include <QVector>
#include <memory>

#include "ChartDataSource.h"
#include "quickcharts_export.h"

#if QUICKCHARTS_BUILD_DEPRECATED_SINCE(5, 78)

/**
 * A data source that provides a history of a single value.
 *
 * \deprecated Since 5.78, use HistoryProxySource instead with a
 * SingleValueSource as inner source.
 */
class ValueHistorySource : public ChartDataSource
{
    Q_OBJECT
    /**
     * The value to track.
     *
     * Depending on the value of \property interval , changing this will add a
     * new item to the source.
     */
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY dataChanged)
    /**
     * The maximum amount of history to keep.
     *
     * The default is 10.
     */
    Q_PROPERTY(int maximumHistory READ maximumHistory WRITE setMaximumHistory NOTIFY maximumHistoryChanged)
    /**
     * The interval, in milliseconds, with which to query the value.
     *
     * If not set or set to a value < 0, a new item will be added whenever value
     * changes. Otherwise, the source will sample the value every interval
     * milliseconds and add a new item with whatever value it has at that point
     * - even if it did not change.
     *
     * The default is 0.
     */
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

#endif // QUICKCHARTS_BUILD_DEPRECATED_SINCE

#endif // VALUEHISTORYSOURCE_H
