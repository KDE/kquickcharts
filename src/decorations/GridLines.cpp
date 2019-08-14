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

#include "GridLines.h"

#include "XYChart.h"
#include "scenegraph/LineGridNode.h"

LinePropertiesGroup::LinePropertiesGroup(GridLines* parent)
    : QObject(parent)
{
    m_parent = parent;
}

bool LinePropertiesGroup::visible() const
{
    return m_visible;
}

void LinePropertiesGroup::setVisible(bool newVisible)
{
    if (newVisible == m_visible) {
        return;
    }

    m_visible = newVisible;
    Q_EMIT propertiesChanged();
}


QColor LinePropertiesGroup::color() const
{
    return m_color;
}

void LinePropertiesGroup::setColor(const QColor & newColor)
{
    if (newColor == m_color) {
        return;
    }

    m_color = newColor;
    Q_EMIT propertiesChanged();
}

float LinePropertiesGroup::lineWidth() const
{
    return m_lineWidth;
}

void LinePropertiesGroup::setLineWidth(float newLineWidth)
{
    if (newLineWidth == m_lineWidth) {
        return;
    }

    m_lineWidth = newLineWidth;
    Q_EMIT propertiesChanged();
}

int LinePropertiesGroup::frequency() const
{
    return m_frequency;
}

void LinePropertiesGroup::setFrequency(int newFrequency)
{
    if (newFrequency == m_frequency) {
        return;
    }

    m_frequency = newFrequency;
    Q_EMIT propertiesChanged();
}

int LinePropertiesGroup::count() const
{
    return m_count;
}

void LinePropertiesGroup::setCount(int newCount)
{
    if (newCount == m_count) {
        return;
    }

    m_count = newCount;
    Q_EMIT propertiesChanged();
}

class GridLines::Private
{
public:
    GridLines::Direction direction = Direction::Horizontal;
    XYChart *chart = nullptr;
    float spacing = 10.0;

    std::unique_ptr<LinePropertiesGroup> major;
    std::unique_ptr<LinePropertiesGroup> minor;
};

GridLines::GridLines(QQuickItem* parent)
    : QQuickItem(parent), d(new Private)
{
    setFlag(QQuickItem::ItemHasContents);

    d->major = std::make_unique<LinePropertiesGroup>(this);
    connect(d->major.get(), &LinePropertiesGroup::propertiesChanged, this, &GridLines::update);
    d->minor = std::make_unique<LinePropertiesGroup>(this);
    connect(d->minor.get(), &LinePropertiesGroup::propertiesChanged, this, &GridLines::update);
}

GridLines::~GridLines()
{
}

GridLines::Direction GridLines::direction() const
{
    return d->direction;
}

void GridLines::setDirection(GridLines::Direction newDirection)
{
    if (newDirection == d->direction) {
        return;
    }

    d->direction = newDirection;
    update();
    Q_EMIT directionChanged();
}

XYChart * GridLines::chart() const
{
    return d->chart;
}

void GridLines::setChart(XYChart * newChart)
{
    if (newChart == d->chart) {
        return;
    }

    if (d->chart) {
        disconnect(d->chart, &XYChart::computedRangeChanged, this, &GridLines::update);
    }

    d->chart = newChart;

    if (d->chart) {
        connect(d->chart, &XYChart::computedRangeChanged, this, &GridLines::update);
    }

    update();
    Q_EMIT chartChanged();
}


float GridLines::spacing() const
{
    return d->spacing;
}

void GridLines::setSpacing(float newSpacing)
{
    if (newSpacing == d->spacing || d->chart != nullptr) {
        return;
    }

    d->spacing = newSpacing;
    update();
    Q_EMIT spacingChanged();
}

LinePropertiesGroup * GridLines::major() const
{
    return d->major.get();
}

LinePropertiesGroup * GridLines::minor() const
{
    return d->minor.get();
}

QSGNode * GridLines::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData*)
{
    if (!node) {
        node = new QSGNode{};
        node->appendChildNode(new LineGridNode{});
        node->appendChildNode(new LineGridNode{});
    }

    if (d->chart) {
        if (d->direction == Direction::Horizontal) {
            d->spacing = width() / (d->chart->computedRange().distanceX - 1);
        } else {
            d->spacing = height() / (d->chart->computedRange().distanceY);
        }
    }

    updateLines(static_cast<LineGridNode*>(node->childAtIndex(0)), d->minor.get());
    updateLines(static_cast<LineGridNode*>(node->childAtIndex(1)), d->major.get());

    return node;
}

void GridLines::updateLines(LineGridNode *node, LinePropertiesGroup *properties)
{
    node->setVisible(properties->visible());
    node->setRect(boundingRect());
    node->setVertical(d->direction == Direction::Vertical);
    node->setColor(properties->color());
    node->setLineWidth(properties->lineWidth());
    if (properties->count() > 0) {
        node->setSpacing(d->direction == Direction::Horizontal ? width() / (properties->count() + 1) : height() / (properties->count() + 1));
    } else {
        node->setSpacing(d->spacing * properties->frequency());
    }
    node->update();
}
