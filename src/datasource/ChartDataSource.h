/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

    virtual QVariant first() const;

    Q_SIGNAL void dataChanged();

protected:
    static bool variantCompare(const QVariant &lhs, const QVariant &rhs);
};

#endif // DATASOURCE_H
