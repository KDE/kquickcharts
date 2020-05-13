/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "BarChart.h"

#include <QDebug>
#include <QSGNode>

#include "datasource/ChartDataSource.h"
#include "scenegraph/BarChartNode.h"
#include "RangeGroup.h"

BarChart::BarChart(QQuickItem *parent)
    : XYChart(parent)
{
}

qreal BarChart::spacing() const
{
    return m_spacing;
}

void BarChart::setSpacing(qreal newSpacing)
{
    if (newSpacing == m_spacing) {
        return;
    }

    m_spacing = newSpacing;
    update();
    Q_EMIT spacingChanged();
}

qreal BarChart::barWidth() const
{
    return m_barWidth;
}

void BarChart::setBarWidth(qreal newBarWidth)
{
    if (newBarWidth == m_barWidth) {
        return;
    }

    m_barWidth = newBarWidth;
    update();
    Q_EMIT barWidthChanged();
}

QSGNode *BarChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    if (!node) {
        node = new BarChartNode{};
    }

    QVector<QPair<QVector2D, QColor>> values;

    // TODO: Find some way to clean this up and simplify it, since this is pretty ugly.
    auto w = m_barWidth;
    if (w < 0.0) {
        if (stacked()) {
            auto totalItemCount = m_values.size();
            w = width() / totalItemCount - m_spacing;

            auto x = float(m_spacing / 2);
            auto itemSpacing = w + m_spacing;

            for (const auto &items : qAsConst(m_values)) {
                for (int i = items.count() - 1; i >= 0; --i) {
                    auto entry = items.at(i);
                    values << QPair<QVector2D, QColor>{QVector2D{x, float(entry.first)}, entry.second};
                }
                x += itemSpacing;
            }
        } else {
            auto totalItemCount = m_values.size() * valueSources().count();

            w = width() / totalItemCount - m_spacing;

            auto x = float(m_spacing / 2);
            auto itemSpacing = w + m_spacing;

            for (const auto &items : qAsConst(m_values)) {
                for (const auto &entry : items) {
                    values << QPair<QVector2D, QColor>{QVector2D{x, float(entry.first)}, entry.second};
                    x += itemSpacing;
                }
            }
        }
    } else {
        auto itemSpacing = width() / m_values.size();
        if (stacked()) {
            auto x = float(itemSpacing / 2 - m_barWidth / 2);

            for (const auto &items : qAsConst(m_values)) {
                for (int i = items.count() - 1; i >= 0; --i) {
                    auto entry = items.at(i);
                    values << QPair<QVector2D, QColor>{QVector2D{x, float(entry.first)}, entry.second};
                }
                x += itemSpacing;
            }
        } else {
            auto totalWidth = m_barWidth * valueSources().count() + m_spacing * (valueSources().count() - 1);

            auto x = float(itemSpacing / 2 - totalWidth / 2);

            for (const auto &items : qAsConst(m_values)) {
                for (int i = 0; i < items.count(); ++i) {
                    auto entry = items.at(i);
                    values << QPair<QVector2D, QColor>{QVector2D{float(x + i * (m_barWidth + m_spacing)), float(entry.first)}, entry.second};
                }
                x += itemSpacing;
            }
        }
    }

    auto barNode = static_cast<BarChartNode *>(node);
    barNode->setRect(boundingRect());
    barNode->setValues(values);
    barNode->setBarWidth(w);

    barNode->update();

    return barNode;
}

void BarChart::onDataChanged()
{
    m_values.clear();

    updateComputedRange();

    const auto range = computedRange();
    const auto sources = valueSources();
    auto colors = colorSource();
    auto indexMode = indexingMode();
    auto colorIndex = 0;

    m_values.fill(QVector<QPair<qreal, QColor>>{}, range.distanceX);

    auto generator = [&, i = range.startX]() mutable -> QVector<QPair<qreal, QColor>> {
        QVector<QPair<qreal, QColor>> values;

        for (int j = 0; j < sources.count(); ++j) {
            auto value = (sources.at(j)->item(i).toReal() - range.startY) / range.distanceY;
            values << QPair<qreal, QColor>(value, colors->item(colorIndex).value<QColor>());

            if (indexMode != Chart::IndexSourceValues) {
                colorIndex++;
            }
        }

        if (stacked()) {
            auto previous = 0.0;
            for (auto &value : values) {
                value.first += previous;
                previous = value.first;
            }
        }

        if (indexMode == Chart::IndexSourceValues) {
            colorIndex++;
        } else if (indexMode == Chart::IndexEachSource) {
            colorIndex = 0;
        }

        i++;
        return values;
    };

    if (direction() == Direction::ZeroAtStart) {
        std::generate_n(m_values.begin(), range.distanceX, generator);
    } else {
        std::generate_n(m_values.rbegin(), range.distanceX, generator);
    }

    update();
}
