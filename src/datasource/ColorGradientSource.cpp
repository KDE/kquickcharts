/*
 * This file is part of Quick Charts
 * Copyright 2019 Marco Martin <mart@kde.org>
 * Copyright 2019 David Edmundson <davidedmundson@kde.org>
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

#include "ColorGradientSource.h"

#include <QVariant>

#include <QDebug>

ColorGradientSource::ColorGradientSource(QObject *parent)
    : ChartDataSource(parent)
{
}

int ColorGradientSource::itemCount() const
{
    return m_itemCount;
}

QVariant ColorGradientSource::item(int index) const
{
    if (index < 0 || index >= m_colors.size()) {
        return QVariant{};
    }

    return m_colors.at(index);
}

QVariant ColorGradientSource::minimum() const
{
    return QVariant{};
}

QVariant ColorGradientSource::maximum() const
{
    return QVariant{};
}

QColor ColorGradientSource::baseColor() const
{
    return m_baseColor;
}

void ColorGradientSource::setBaseColor(const QColor &newBaseColor)
{
    if (newBaseColor == m_baseColor) {
        return;
    }

    m_baseColor = newBaseColor;
    regenerateColors();
    Q_EMIT baseColorChanged();
}

void ColorGradientSource::setItemCount(int newItemCount)
{
    if (newItemCount == m_itemCount) {
        return;
    }

    m_itemCount = newItemCount;
    regenerateColors();
    Q_EMIT itemCountChanged();
}

QVariantList ColorGradientSource::colors() const
{
    QVariantList colorsVariant;
    colorsVariant.reserve(m_colors.count());
    for (const QColor &color : qAsConst(m_colors)) {
        colorsVariant.append(color);
    }
    return colorsVariant;
}

void ColorGradientSource::regenerateColors()
{
    if (!m_baseColor.isValid() || m_itemCount <= 0) {
        return;
    }

    m_colors.clear();

    for (int i = 0; i < m_itemCount; ++i) {
        auto newHue = m_baseColor.hsvHueF() + i * (1.0 / m_itemCount);
        newHue = newHue - int(newHue);
        m_colors.append(QColor::fromHsvF(newHue, m_baseColor.saturationF(), m_baseColor.valueF(), m_baseColor.alphaF()));
    }

    Q_EMIT dataChanged();
}
