/*
 * SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "ChartDataSource.h"

#include <QVariant>

/*!
 * \qmltype MapProxySource
 * \inherits ChartDataSource
 * \inqmlmodule org.kde.quickcharts
 *
 * \brief A source that uses the values of another source to produce values from a map.
 *
 * This source reads values from another source, then uses those as an index to
 * a map of different values and returns the appropriate value from that map.
 * This source's itemCount matches that of the other source.
 */
class QUICKCHARTS_EXPORT MapProxySource : public ChartDataSource
{
    Q_OBJECT
    QML_ELEMENT

public:
    MapProxySource(QObject *parent = nullptr);

    /*!
     * \qmlproperty ChartDataSource MapProxySource::source
     * \brief A ChartDataSource that is used as map indexes.
     */
    Q_PROPERTY(ChartDataSource *source READ source WRITE setSource NOTIFY sourceChanged)
    ChartDataSource *source() const;
    void setSource(ChartDataSource *newSource);
    Q_SIGNAL void sourceChanged();

    /*!
     * \qmlproperty QVariantMap MapProxySource::map
     * \brief The map to index for values.
     */
    Q_PROPERTY(QVariantMap map READ map WRITE setMap NOTIFY mapChanged)
    QVariantMap map() const;
    void setMap(const QVariantMap &newMap);
    Q_SIGNAL void mapChanged();

    int itemCount() const override;
    QVariant item(int index) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

private:
    ChartDataSource *m_source = nullptr;
    QVariantMap m_map;
};
