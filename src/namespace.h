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

#ifndef NAMESPACE_H
#define NAMESPACE_H

/*
 * We only really want to place things in a namespace for Doxygen's sake, since
 * it will list classes by namespace. For the actual code, we do not really care
 * about using a namespace as everything is done through QML module loading
 * anyway. In addition, Qt/moc can be a pain when dealing with namespaced code.
 * So this macro is defined only when doxygen is running.
 */

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define BEGIN_NAMESPACE
#define END_NAMESPACE
#else
#define BEGIN_NAMESPACE namespace org { namespace kde { namespace quickcharts {
#define END_NAMESPACE } } }
#endif

#endif // NAMESPACE_H
