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

#ifndef LEGENDMODEL_H
#define LEGENDMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

class Chart;
class ChartDataSource;

struct LegendItem
{
    QString name;
    QColor color;
    QVariant value;
};

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
    void setChart(Chart *newChart);
    Q_SIGNAL void chartChanged();

    int sourceIndex() const;
    void setSourceIndex(int index);
    Q_SIGNAL void sourceIndexChanged();

private:
    void queueUpdate();
    void update();
    void updateData();
    int countItems();

    Chart *m_chart = nullptr;
    int m_sourceIndex = UseSourceCount;
    bool m_updateQueued = false;
    std::vector<QMetaObject::Connection> m_connections;
    std::vector<LegendItem> m_items;
};

#endif // LEGENDMODEL_H
