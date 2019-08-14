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

#include "ModelHistorySource.h"

#include <QVariantList>
#include <QAbstractItemModel>
#include <QDebug>

ModelHistorySource::ModelHistorySource(QObject* parent)
    : ModelSource(parent)
{
    connect(this, &ModelHistorySource::modelChanged, this, &ModelHistorySource::onModelChanged);
}

ModelHistorySource::~ModelHistorySource()
{
}

int ModelHistorySource::itemCount() const
{
    return m_history.size();
}

QVariant ModelHistorySource::item(int index) const
{
    if(index < 0 || index >= m_history.size())
        return QVariant{};

    return m_history.at(index);
}

QVariant ModelHistorySource::minimum() const
{
    if(m_history.isEmpty())
        return QVariant{};

    return *std::min_element(m_history.begin(), m_history.end());
}

QVariant ModelHistorySource::maximum() const
{
    if(m_history.isEmpty())
        return QVariant{};

    return *std::max_element(m_history.begin(), m_history.end());
}

int ModelHistorySource::row() const
{
    return m_row;
}

void ModelHistorySource::setRow(int row)
{
    if (m_row == row) {
        return;
    }

    m_row = row;
    Q_EMIT rowChanged();
}

int ModelHistorySource::maximumHistory() const
{
    return m_maximumHistory;
}

void ModelHistorySource::setMaximumHistory(int maximumHistory)
{
    if (m_maximumHistory == maximumHistory) {
        return;
    }

    m_maximumHistory = maximumHistory;
    Q_EMIT maximumHistoryChanged();
}

void ModelHistorySource::onModelChanged()
{
    if(model())
        connect(model(), &QAbstractItemModel::dataChanged, this, &ModelHistorySource::onDataChanged);
}

void ModelHistorySource::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    if (!roles.isEmpty() && !roles.contains(role()))
        return;

    if (topLeft.row() > m_row || bottomRight.row() < m_row)
        return;

    if (topLeft.column() > column() || bottomRight.column() < column())
        return;

    auto entry = model()->data(model()->index(m_row, column()), role());
    if (!entry.isValid())
        return;

    m_history.prepend(entry);
    while(m_history.size() > m_maximumHistory)
        m_history.pop_back();

    Q_EMIT dataChanged();
}
