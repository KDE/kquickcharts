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

#ifndef LEGENDMODEL_H
#define LEGENDMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

#include "../namespace.h"

BEGIN_NAMESPACE

class Chart;
class ChartDataSource;

/**
 * A model that extracts information from a chart that can be displayed as a legend.
 */
class LegendModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Chart *chart READ chart WRITE setChart NOTIFY chartChanged)
    Q_PROPERTY(int sourceIndex READ sourceIndex WRITE setSourceIndex NOTIFY sourceIndexChanged)

public:
    enum Roles { NameRole = Qt::UserRole, ColorRole, ValueRole };

    enum SourceIndex { UseSourceCount = -2 };
    Q_ENUM(SourceIndex)

    explicit LegendModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Chart *chart() const;
    Q_SLOT void setChart(Chart *newChart);
    Q_SIGNAL void chartChanged();

    int sourceIndex() const;
    Q_SLOT void setSourceIndex(int index);
    Q_SIGNAL void sourceIndexChanged();

private:
    void queueUpdate();
    void update();
    void updateData();

    struct LegendItem;

    Chart *m_chart = nullptr;
    int m_sourceIndex = UseSourceCount;
    bool m_updateQueued = false;
    std::vector<QMetaObject::Connection> m_connections;
    std::vector<LegendItem> m_items;
};

END_NAMESPACE

#endif // LEGENDMODEL_H
