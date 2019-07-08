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

    auto barNode = static_cast<BarChartNode*>(node);
    barNode->setRect(boundingRect());
    barNode->setValues(m_values);
    barNode->setSpacing(stacked() ? 0.0 : m_barWidth + m_spacing);
    barNode->setBarWidth(m_barWidth);
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
//     auto colorIndex = 0;

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
/*
    m_values = allValues;*/

/*
    for (int i = range.startX; i < range.endX; ++i) {
        QVector<QPair<qreal, QColor>> values;
        for (int j = 0; j < sources.count(); ++j) {
            values << QPair<qreal, QColor>(sources.at(j)->item(i).toReal(), colors->item(j).value<QColor>());
        }
    }


    for (auto source : sources) {
        auto color = colorSource()->item(colorIndex++).value<QColor>();
        QVector<QPair<qreal, QColor>> values;
        for (int i = 0; i < source->itemCount(); ++i) {
            values << QPair<qreal, QColor>(source->item(i).toReal(), color);
        }

        if (stacked()) {
            auto previousValue = 0.0;
            for (auto& value : values) {
                value.first += previousValue;
                previousValue = value.first;
            }
        }

        m_values << values;
    }*/

    update();
}
