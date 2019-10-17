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

#ifndef MODELSOURCE_H
#define MODELSOURCE_H

#include "ChartDataSource.h"

#include <QAbstractItemModel>

/**
 * A data source that reads data from a QAbstractItemModel.
 *
 *
 */
class ModelSource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY(int role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(QString roleName READ roleName WRITE setRoleName NOTIFY roleNameChanged)
    Q_PROPERTY(int column READ column WRITE setColumn NOTIFY columnChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(bool indexColumns READ indexColumns WRITE setIndexColumns NOTIFY indexColumnsChanged)

public:
    explicit ModelSource(QObject *parent = nullptr);

    int role() const;
    void setRole(int role);
    Q_SIGNAL void roleChanged();

    QString roleName() const;
    void setRoleName(const QString &name);
    Q_SIGNAL void roleNameChanged();

    int column() const;
    void setColumn(int column);
    Q_SIGNAL void columnChanged();

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);
    Q_SIGNAL void modelChanged();

    bool indexColumns() const;
    void setIndexColumns(bool index);
    Q_SIGNAL void indexColumnsChanged();

    virtual int itemCount() const override;
    virtual QVariant item(int index) const override;
    virtual QVariant minimum() const override;
    virtual QVariant maximum() const override;

private:
    mutable int m_role = -1;
    QString m_roleName;
    int m_column = 0;
    bool m_indexColumns = false;
    QAbstractItemModel *m_model = nullptr;
};

#endif // MODELSOURCE_H
