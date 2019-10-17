/*
 * This file is part of Quick Charts.
 * Copyright 2019 Marco Martin <mart@kde.org>
 * Copyright 2019 David Edmundson <davidedmundson@kde.org>
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

#ifndef COLORGRADIENTSOURCE_H
#define COLORGRADIENTSOURCE_H

#include <QColor>
#include <QVector>

#include "ChartDataSource.h"

/**
 * A data source that provides a hue-shifted color as data.
 */
class ColorGradientSource : public ChartDataSource
{
    Q_OBJECT
    Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor NOTIFY baseColorChanged)
    Q_PROPERTY(int itemCount READ itemCount WRITE setItemCount NOTIFY itemCountChanged)
    Q_PROPERTY(QVariantList colors READ colors NOTIFY dataChanged)

public:
    explicit ColorGradientSource(QObject *parent = nullptr);

    int itemCount() const override;
    QVariant item(int index) const override;
    QVariant minimum() const override;
    QVariant maximum() const override;

    QColor baseColor() const;
    void setBaseColor(const QColor &newBaseColor);
    Q_SIGNAL void baseColorChanged();

    void setItemCount(int newItemCount);
    Q_SIGNAL void itemCountChanged();

    QVariantList colors() const;

private:
    void regenerateColors();

    QColor m_baseColor = Qt::blue;
    int m_itemCount = 0;
    QVector<QColor> m_colors;
};

#endif // COLORGRADIENTSOURCE_H
