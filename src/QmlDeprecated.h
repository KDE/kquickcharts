/*
 * SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef DEPRECATED_H
#define DEPRECATED_H

#include "charts_deprecated_logging.h"

#include <QQmlContext>
#include <QQmlEngine>

/**
 * Tiny helper macro to mark a QML item as deprecated.
 *
 * This should be placed into the item's constructor and will output a
 * deprecation message when the item is instantiated.
 *
 * All parameters are expected to be strings.
 *
 * \param item The name of the item that is deprecated.
 * \param since The version since when the deprecation happened.
 * \param message An extra message to display, like which item to use instead.
 */
// clang-format off
#define QML_DEPRECATED(item, since, message) \
    QMetaObject::invokeMethod(this, [this]() {\
        qCWarning(DEPRECATED).nospace() << item <<  " is deprecated (since " << since << "): " << message;\
        if (!qmlContext(this)) return;\
        QString elidedName = QStringLiteral("...") + qmlContext(this)->baseUrl().toString().right(80);\
        qCWarning(DEPRECATED) << "Note: Instantiated from" << elidedName;\
    }, Qt::QueuedConnection);
// clang-format on

#endif // DEPRECATED_H
