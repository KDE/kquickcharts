/*
 * This file is part of Quick Charts
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

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSurfaceFormat>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addOption({QStringLiteral("page"), QStringLiteral("The page to show."), QStringLiteral("page")});
    parser.addOption({QStringLiteral("api"),
                      QStringLiteral("The graphics API to use. Can be one of 'default', 'core45', 'compat45', 'compat21' or 'es'."),
                      QStringLiteral("api"),
                      QStringLiteral("default")});
    parser.addHelpOption();
    parser.process(app);

    QSurfaceFormat format;
    auto api = parser.value(QStringLiteral("api"));
    if (api == QStringLiteral("core45")) {
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setVersion(4, 5);
    } else if (api == QStringLiteral("compat45")) {
        format.setProfile(QSurfaceFormat::CompatibilityProfile);
        format.setVersion(4, 5);
    } else if (api == QStringLiteral("es")) {
        format.setRenderableType(QSurfaceFormat::OpenGLES);
    } else if (api == QStringLiteral("default") || api == QStringLiteral("compat20")) {
        format.setVersion(2, 1);
    } else {
        qWarning() << "Unknown API option" << api << "\n";
        parser.showHelp(1);
    }
    QSurfaceFormat::setDefaultFormat(format);

    QQmlApplicationEngine engine;

    if (parser.isSet(QStringLiteral("page"))) {
        engine.rootContext()->setContextProperty(QStringLiteral("__commandLinePage"), parser.value(QStringLiteral("page")));
    } else {
        engine.rootContext()->setContextProperty(QStringLiteral("__commandLinePage"), nullptr);
    }

    engine.load(QStringLiteral("qrc:/main.qml"));

    return app.exec();
}
