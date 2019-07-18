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

#include "BarChart.h"

#include <QSGNode>
#include <QDebug>

#include "datasource/ChartDataSource.h"
#include "scenegraph/BarChartNode.h"

BarChart::BarChart(QQuickItem* parent)
    : XYChart(parent)
{
}

BarChart::~BarChart()
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


QSGNode *BarChart::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData*)
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

            for (const auto items : qAsConst(m_values)) {
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

            for (const auto items : qAsConst(m_values)) {
                for (auto entry : items) {
                    values << QPair<QVector2D, QColor>{QVector2D{x, float(entry.first)}, entry.second};
                    x += itemSpacing;
                }
            }
        }
    } else {
        auto itemSpacing = width() / m_values.size();
        if (stacked()) {
            auto x = float(itemSpacing / 2 - m_barWidth / 2);

            for (const auto items : qAsConst(m_values)) {
                for (int i = items.count() - 1; i >= 0; --i) {
                    auto entry = items.at(i);
                    values << QPair<QVector2D, QColor>{QVector2D{x, float(entry.first)}, entry.second};
                }
                x += itemSpacing;
            }
        } else {
            auto totalWidth = m_barWidth * valueSources().count() + m_spacing * (valueSources().count() - 1);

            auto x = float(itemSpacing / 2 - totalWidth / 2);

            for (const auto items : qAsConst(m_values)) {
                for (int i = 0; i < items.count(); ++i) {
                    auto entry = items.at(i);
                    values << QPair<QVector2D, QColor>{QVector2D{float(x + i * (m_barWidth + m_spacing)), float(entry.first)}, entry.second};
                }
                x += itemSpacing;
            }
        }
    }

    auto barNode = static_cast<BarChartNode*>(node);
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

    m_values.fill(QVector<QPair<qreal, QColor>>{}, range.distanceX);

    auto generator = [&, i = range.startX]() mutable -> QVector<QPair<qreal, QColor>> {
        QVector<QPair<qreal, QColor>> values;
        for (int j = 0; j < sources.count(); ++j) {
            auto value = (sources.at(j)->item(i).toReal() - range.startY) / range.distanceY;
            values << QPair<qreal, QColor>(value, colors->item(j).value<QColor>());
        }

        if (stacked()) {
            auto previous = 0.0;
            for (auto &value : values) {
                value.first += previous;
                previous = value.first;
            }
        }

        i++;
        return values;
    };

    if(direction() == Direction::ZeroAtStart) {
        std::generate_n(m_values.begin(), range.distanceX, generator);
    } else {
        std::generate_n(m_values.rbegin(), range.distanceX, generator);
    }

    update();
}
