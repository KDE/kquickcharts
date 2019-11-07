/*
 * This file is part of KQuickCharts
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

#include "AxisLabels.h"

#include <QDebug>

#include "datasource/ChartDataSource.h"
#include <QQmlContext>

AxisLabelsAttached::AxisLabelsAttached(QObject *parent)
    : QObject(parent)
{
}

int AxisLabelsAttached::index() const
{
    return m_index;
}

void AxisLabelsAttached::setIndex(int newIndex)
{
    if (newIndex == m_index) {
        return;
    }

    m_index = newIndex;
    Q_EMIT indexChanged();
}

QString AxisLabelsAttached::label() const
{
    return m_label;
}

void AxisLabelsAttached::setLabel(const QString &newLabel)
{
    if (newLabel == m_label) {
        return;
    }

    m_label = newLabel;
    Q_EMIT labelChanged();
}

AxisLabels::AxisLabels(QQuickItem *parent)
    : QQuickItem(parent)
{
}

AxisLabels::Direction AxisLabels::direction() const
{
    return m_direction;
}

void AxisLabels::setDirection(AxisLabels::Direction newDirection)
{
    if (newDirection == m_direction) {
        return;
    }

    m_direction = newDirection;
    scheduleLayout();
    Q_EMIT directionChanged();
}

QQmlComponent *AxisLabels::delegate() const
{
    return m_delegate;
}

void AxisLabels::setDelegate(QQmlComponent *newDelegate)
{
    if (newDelegate == m_delegate) {
        return;
    }

    m_delegate = newDelegate;
    updateLabels();
    Q_EMIT delegateChanged();
}

ChartDataSource *AxisLabels::source() const
{
    return m_source;
}

void AxisLabels::setSource(ChartDataSource *newSource)
{
    if (newSource == m_source) {
        return;
    }

    if (m_source) {
        m_source->disconnect(this);
    }

    m_source = newSource;

    if (m_source) {
        connect(m_source, &ChartDataSource::dataChanged, this, [this]() { updateLabels(); });
    }

    updateLabels();
    Q_EMIT sourceChanged();
}

Qt::Alignment AxisLabels::alignment() const
{
    return m_alignment;
}

void AxisLabels::setAlignment(Qt::Alignment newAlignment)
{
    if (newAlignment == m_alignment) {
        return;
    }

    m_alignment = newAlignment;
    scheduleLayout();
    Q_EMIT alignmentChanged();
}

bool AxisLabels::constrainToBounds() const
{
    return m_constrainToBounds;
}

void AxisLabels::setConstrainToBounds(bool newConstrainToBounds)
{
    if (newConstrainToBounds == m_constrainToBounds) {
        return;
    }

    m_constrainToBounds = newConstrainToBounds;
    scheduleLayout();
    Q_EMIT constrainToBoundsChanged();
}

void AxisLabels::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry != oldGeometry) {
        scheduleLayout();
    }
}

void AxisLabels::scheduleLayout()
{
    if (!m_layoutScheduled) {
        QMetaObject::invokeMethod(
            this,
            [this]() { layout(); m_layoutScheduled = false; },
            Qt::QueuedConnection);
        m_layoutScheduled = true;
    }
}

bool AxisLabels::isHorizontal()
{
    return m_direction == Direction::HorizontalLeftRight || m_direction == Direction::HorizontalRightLeft;
}

void AxisLabels::updateLabels()
{
    qDeleteAll(m_labels);
    m_labels.clear();

    if (!m_delegate || !m_source) {
        return;
    }

    for (int i = 0; i < m_source->itemCount(); ++i) {
        auto label = m_source->item(i).toString();

        auto context = new QQmlContext(qmlContext(this));
        auto item = qobject_cast<QQuickItem *>(m_delegate->beginCreate(context));
        if (!item) {
            qWarning() << "Failed to create label instance for label" << label;
            continue;
        }

        QObject::connect(item, &QQuickItem::xChanged, this, [this]() { scheduleLayout(); });
        QObject::connect(item, &QQuickItem::yChanged, this, [this]() { scheduleLayout(); });
        QObject::connect(item, &QQuickItem::widthChanged, this, [this]() { scheduleLayout(); });
        QObject::connect(item, &QQuickItem::heightChanged, this, [this]() { scheduleLayout(); });

        context->setParent(item);
        item->setParentItem(this);

        auto attached = static_cast<AxisLabelsAttached *>(qmlAttachedPropertiesObject<AxisLabels>(item, true));
        attached->setIndex(i);
        attached->setLabel(label);

        m_delegate->completeCreate();
        m_labels << item;
    }

    scheduleLayout();
}

void AxisLabels::layout()
{
    auto maxWidth = 0.0;
    auto totalWidth = 0.0;
    auto maxHeight = 0.0;
    auto totalHeight = 0.0;

    for (auto label : qAsConst(m_labels)) {
        maxWidth = std::max(maxWidth, label->width());
        maxHeight = std::max(maxHeight, label->height());
        totalWidth += label->width();
        totalHeight += label->height();
    }

    auto impWidth = isHorizontal() ? totalWidth : maxWidth;
    auto impHeight = isHorizontal() ? maxHeight : totalHeight;

    if (qFuzzyCompare(impWidth, width()) && qFuzzyCompare(impHeight, height())) {
        return;
    }

    setImplicitWidth(impWidth);
    setImplicitHeight(impHeight);

    auto spacing = (isHorizontal() ? width() : height()) / (m_labels.size() - 1);
    auto i = 0;
    auto layoutWidth = isHorizontal() ? 0.0 : width();
    auto layoutHeight = isHorizontal() ? height() : 0.0;

    for (auto label : qAsConst(m_labels)) {
        auto x = 0.0;
        auto y = 0.0;

        switch (m_direction) {
        case Direction::HorizontalLeftRight:
            x = i * spacing;
            break;
        case Direction::HorizontalRightLeft:
            x = width() - i * spacing;
            break;
        case Direction::VerticalTopBottom:
            y = i * spacing;
            break;
        case Direction::VerticalBottomTop:
            y = height() - i * spacing;
            break;
        }

        if (m_alignment & Qt::AlignHCenter) {
            x += (layoutWidth - label->width()) / 2;
        } else if (m_alignment & Qt::AlignRight) {
            x += layoutWidth - label->width();
        }

        if (m_alignment & Qt::AlignVCenter) {
            y += (layoutHeight - label->height()) / 2;
        } else if (m_alignment & Qt::AlignBottom) {
            y += layoutHeight - label->height();
        }

        if (m_constrainToBounds) {
            x = std::max(x, 0.0);
            x = x + label->width() > width() ? width() - label->width() : x;
            y = std::max(y, 0.0);
            y = y + label->height() > height() ? height() - label->height() : y;
        }

        label->setX(x);
        label->setY(y);
        i++;
    }
}
