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

#include "ChartsPlugin.h"

#include "BarChart.h"
#include "Chart.h"
#include "LineChart.h"
#include "PieChart.h"
#include "RangeGroup.h"
#include "XYChart.h"

#include "decorations/AxisLabels.h"
#include "decorations/GridLines.h"
#include "decorations/LegendModel.h"

#include "datasource/ArraySource.h"
#include "datasource/ChartAxisSource.h"
#include "datasource/ColorGradientSource.h"
#include "datasource/ModelHistorySource.h"
#include "datasource/ModelSource.h"
#include "datasource/SingleValueSource.h"
#include "datasource/ValueHistorySource.h"

QuickChartsPlugin::QuickChartsPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{
}

void QuickChartsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.quickcharts"));

    Q_INIT_RESOURCE(shaders);

    qmlRegisterType<PieChart>(uri, 1, 0, "PieChart");
    qmlRegisterType<LineChart>(uri, 1, 0, "LineChart");
    qmlRegisterType<BarChart>(uri, 1, 0, "BarChart");
    qmlRegisterUncreatableType<XYChart>(uri, 1, 0, "XYChart", QStringLiteral("Just a base class"));
    qmlRegisterUncreatableType<Chart>(uri, 1, 0, "Chart", QStringLiteral("Just a base class"));

    qmlRegisterUncreatableType<ChartDataSource>(uri, 1, 0, "ChartDataSource", QStringLiteral("Just a base class"));
    qmlRegisterType<ModelSource>(uri, 1, 0, "ModelSource");
    qmlRegisterType<SingleValueSource>(uri, 1, 0, "SingleValueSource");
    qmlRegisterType<ArraySource>(uri, 1, 0, "ArraySource");
    qmlRegisterType<ModelHistorySource>(uri, 1, 0, "ModelHistorySource");
    qmlRegisterType<ChartAxisSource>(uri, 1, 0, "ChartAxisSource");
    qmlRegisterType<ValueHistorySource>(uri, 1, 0, "ValueHistorySource");
    qmlRegisterType<ColorGradientSource>(uri, 1, 0, "ColorGradientSource");

    qmlRegisterUncreatableType<RangeGroup>(uri, 1, 0, "Range", QStringLiteral("Used as a grouped property"));

    qmlRegisterType<GridLines>(uri, 1, 0, "GridLines");
    qmlRegisterUncreatableType<LinePropertiesGroup>(uri, 1, 0, "LinePropertiesGroup", QStringLiteral("Used as a grouped property"));
    qmlRegisterType<AxisLabels>(uri, 1, 0, "AxisLabels");
    qmlRegisterUncreatableType<AxisLabelsAttached>(uri, 1, 0, "AxisLabelsAttached", QStringLiteral("Attached property"));
    qmlRegisterType<LegendModel>(uri, 1, 0, "LegendModel");
}
