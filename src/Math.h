/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef CHARTS_MATH_H
#define CHARTS_MATH_H

#include <cmath>

#include <QVector2D>

/**
 * Convenience header containing several often used mathematics operations.
 */
namespace Math
{

static const qreal pi = std::acos(-1.0);

inline double squared(double input)
{
    return input * input;
}

inline double degToRad(double deg)
{
    return (deg / 180.0) * pi;
}

inline double radToDeg(double rad)
{
    return (rad / pi) * 180.0;
}

inline QVector2D rotated(const QVector2D vector, qreal angle)
{
    auto newX = vector.x() * std::cos(angle) - vector.y() * std::sin(angle);
    auto newY = vector.x() * std::sin(angle) + vector.y() * std::cos(angle);
    return QVector2D(newX, newY);
}

}

#endif
