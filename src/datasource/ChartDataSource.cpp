/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "ChartDataSource.h"

#include <QColor>
#include <QVariant>

ChartDataSource::ChartDataSource(QObject *parent)
    : QObject(parent)
{
}

QVariant ChartDataSource::first() const
{
    return item(0);
}

bool ChartDataSource::variantCompare(const QVariant &lhs, const QVariant &rhs)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return lhs < rhs;
#else
    return QVariant::compare(lhs, rhs) == QPartialOrdering::Less;
#endif
}
