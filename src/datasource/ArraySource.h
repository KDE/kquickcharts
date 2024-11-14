/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef ARRAYSOURCE_H
#define ARRAYSOURCE_H

#include <QVariantList>

#include "ChartDataSource.h"

/*!
 * \qmltype ArraySource
 * \inherits ChartDataSource
 * \inqmlmodule org.kde.quickcharts
 *
 * \brief A data source that provides entries of an array as data.
 */
class QUICKCHARTS_EXPORT ArraySource : public ChartDataSource
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit ArraySource(QObject *parent = nullptr);

    /*!
     * \qmlproperty list<variant> ArraySource::array
     *
     * \brief The array to use to provide entries from.
     */
    Q_PROPERTY(QVariantList array READ array WRITE setArray NOTIFY dataChanged)
    QVariantList array() const;
    void setArray(const QVariantList &array);

    /*!
     * \qmlproperty bool ArraySource::wrap
     *
     * \brief Wraparound when indexing past the array's bounds.
     *
     * If true, when an item is requested at an index outside of the array's
     * bounds, wrap around to the start or end of the array and continue from
     * there. This ensures we always return an entry from the array.
     *
     * If false (the default), requesting an item at an index outside of the
     * array's'bounds, we return an empty item instead.
     */
    Q_PROPERTY(bool wrap READ wrap WRITE setWrap NOTIFY dataChanged)
    bool wrap() const;
    void setWrap(bool wrap);

    int itemCount() const override;
    QVariant item(int index) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

private:
    QVariantList m_array;
    bool m_wrap = false;
};

#endif // ARRAYSOURCE_H
