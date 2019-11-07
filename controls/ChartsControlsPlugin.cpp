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

#include "ChartsControlsPlugin.h"

#include <QDebug>
#include <QFile>
#include <QQmlEngine>
#include <QQuickStyle>
#include <QUrl>

ChartsControlsPlugin::ChartsControlsPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{
}

void ChartsControlsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.quickcharts.controls"));

    m_styleName = QQuickStyle::name();

    qmlRegisterSingletonType(componentUrl(QStringLiteral("Theme.qml")), uri, 1, 0, "Theme");
    qmlRegisterType(componentUrl(QStringLiteral("Legend.qml")), uri, 1, 0, "Legend");
    qmlRegisterType(componentUrl(QStringLiteral("LegendDelegate.qml")), uri, 1, 0, "LegendDelegate");
    qmlRegisterType(componentUrl(QStringLiteral("LineChartControl.qml")), uri, 1, 0, "LineChartControl");
    qmlRegisterType(componentUrl(QStringLiteral("PieChartControl.qml")), uri, 1, 0, "PieChartControl");
}

QUrl ChartsControlsPlugin::componentUrl(const QString &fileName)
{
    auto url = baseUrl();
    url.setPath(url.path() % QLatin1Char('/'));

    auto styled = url.resolved(QUrl{QStringLiteral("styles/") % m_styleName % QLatin1Char('/') % fileName});
    if (QFile::exists(styled.toLocalFile())) {
        return styled;
    }

    return url.resolved(QUrl{fileName});
}
