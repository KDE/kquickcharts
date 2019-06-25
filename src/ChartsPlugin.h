#ifndef CHARTSPLUGIN_H
#define CHARTSPLUGIN_H

#include <QQmlExtensionPlugin>

class QuickChartsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    explicit QuickChartsPlugin(QObject *parent = nullptr);
    void registerTypes(const char *uri) override;
};

#endif // CHARTSPLUGIN_H
