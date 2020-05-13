/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef MODELSOURCE_H
#define MODELSOURCE_H

#include "ChartDataSource.h"

#include <QAbstractItemModel>
#include <QPointer>

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
    QPointer<QAbstractItemModel> m_model;
};

#endif // MODELSOURCE_H
