#include "ChartsPlugin.h"

#include "PieChart.h"
#include "ModelSource.h"
#include "SingleValueSource.h"
#include "RangeGroup.h"
#include "LineChart.h"
#include "ArraySource.h"
#include "Axis.h"

void KQuickControlsAddonsChartsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.quickcharts"));

    Q_INIT_RESOURCE(shaders);

    qmlRegisterType<PieChart>(uri, 1, 0, "PieChart");
    qmlRegisterType<LineChart>(uri, 1, 0, "LineChart");

    qmlRegisterUncreatableType<ChartDataSource>(uri, 1, 0, "ChartDataSource", QStringLiteral("Just a base class"));
    qmlRegisterType<ModelSource>(uri, 1, 0, "ModelSource");
    qmlRegisterType<SingleValueSource>(uri, 1, 0, "SingleValueSource");
    qmlRegisterType<ArraySource>(uri, 1, 0, "ArraySource");

    qmlRegisterUncreatableType<RangeGroup>(uri, 1, 0, "Range", QStringLiteral("Used as a grouped property"));
}
