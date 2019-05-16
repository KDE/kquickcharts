#include "ChartsPlugin.h"

#include "PieChart.h"
#include "ModelRole.h"

void KQuickControlsAddonsChartsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString::fromLatin1(uri) == QLatin1String("org.kde.quickcharts"));

    Q_INIT_RESOURCE(shaders);

    qmlRegisterType<PieChart>(uri, 1, 0, "PieChart");
    qmlRegisterType<ModelRole>(uri, 1, 0, "ModelRole");
}
