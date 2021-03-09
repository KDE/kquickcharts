/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef LEGENDMODEL_H
#define LEGENDMODEL_H

#include <vector>

#include <QAbstractListModel>
#include <QColor>

class Chart;
class ChartDataSource;

struct LegendItem {
    QString name;
    QString shortName;
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
    enum Roles { NameRole = Qt::UserRole, ShortNameRole, ColorRole, ValueRole };

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
    void queueDataChange();
    void update();
    void updateData();
    int countItems();
    QVariant getValueForItem(int item);

    Chart *m_chart = nullptr;
    int m_sourceIndex = UseSourceCount;
    bool m_updateQueued = false;
    bool m_dataChangeQueued = false;
    std::vector<QMetaObject::Connection> m_connections;
    std::vector<LegendItem> m_items;
};

#endif // LEGENDMODEL_H
