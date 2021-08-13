/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "ModelHistorySource.h"

#include <QAbstractItemModel>
#include <QDebug>
#include <QTimer>
#include <QVariantList>

#include "QmlDeprecated.h"

#if QUICKCHARTS_BUILD_DEPRECATED_SINCE(5, 78)

ModelHistorySource::ModelHistorySource(QObject *parent)
    : ModelSource(parent)
{
    QML_DEPRECATED("ModelHistorySource", "5.78", "Use HistoryProxySource instead.")

    connect(this, &ModelHistorySource::modelChanged, this, &ModelHistorySource::onModelChanged);
}

int ModelHistorySource::itemCount() const
{
    return m_history.size();
}

QVariant ModelHistorySource::item(int index) const
{
    if (index < 0 || index >= m_history.size()) {
        return {};
    }

    return m_history.at(index);
}

QVariant ModelHistorySource::minimum() const
{
    if (m_history.isEmpty()) {
        return {};
    }

    auto minProperty = model()->property("minimum");
    auto maxProperty = model()->property("maximum");
    if (maxProperty.isValid() && maxProperty != minProperty) {
        return maxProperty;
    }

    return *std::min_element(m_history.begin(), m_history.end());
}

QVariant ModelHistorySource::maximum() const
{
    if (m_history.isEmpty()) {
        return {};
    }

    auto minProperty = model()->property("minimum");
    auto maxProperty = model()->property("maximum");
    if (maxProperty.isValid() && maxProperty != minProperty) {
        return maxProperty;
    }

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

int ModelHistorySource::interval() const
{
    return m_updateTimer ? m_updateTimer->interval() : -1;
}

void ModelHistorySource::setInterval(int newInterval)
{
    if (m_updateTimer && newInterval == m_updateTimer->interval()) {
        return;
    }

    if (newInterval > 0) {
        if (!m_updateTimer) {
            m_updateTimer = std::make_unique<QTimer>();
            // We need precise timers to avoid missing updates when dealing with semi-constantly
            // updating model. That is, if the model updates at 500ms and we also update at that
            // rate, a drift of 2ms can cause us to miss updates.
            m_updateTimer->setTimerType(Qt::PreciseTimer);
            connect(m_updateTimer.get(), &QTimer::timeout, this, [this]() {
                if (!model()) {
                    return;
                }

                auto index = model()->index(m_row, column());
                onDataChanged(index, index, {role()});
            });
            if (model()) {
                disconnect(model(), &QAbstractItemModel::dataChanged, this, &ModelHistorySource::onDataChanged);
            }
        }
        m_updateTimer->setInterval(newInterval);
        m_updateTimer->start();
    } else {
        m_updateTimer.reset();
        onModelChanged();
    }

    Q_EMIT intervalChanged();
}

void ModelHistorySource::clear()
{
    m_history.clear();
    Q_EMIT dataChanged();
}

void ModelHistorySource::onModelChanged()
{
    if (model() && !m_updateTimer) {
        connect(model(), &QAbstractItemModel::dataChanged, this, &ModelHistorySource::onDataChanged);
    }
}

void ModelHistorySource::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!model()) {
        return;
    }

    if (!roles.isEmpty() && !roles.contains(role())) {
        return;
    }

    if (topLeft.row() > m_row || bottomRight.row() < m_row) {
        return;
    }

    if (topLeft.column() > column() || bottomRight.column() < column()) {
        return;
    }

    auto entry = model()->data(model()->index(m_row, column()), role());

    m_history.prepend(entry);
    while (m_history.size() > m_maximumHistory) {
        m_history.pop_back();
    }

    Q_EMIT dataChanged();
}

#endif // QUICKCHARTS_BUILD_DEPRECATED_SINCE
