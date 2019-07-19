#ifndef CHARTSCONTROLSPLUGIN_H
#define CHARTSCONTROLSPLUGIN_H

#include <QQmlExtensionPlugin>

class ChartsControlsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    explicit ChartsControlsPlugin(QObject *parent = nullptr);
    void registerTypes(const char *uri) override;

private:
    QUrl componentUrl(const QString &fileName);
    QUrl styledComponentUrl(const QString &fileName);
    QString m_styleName;
};

#endif // CHARTSCONTROLSPLUGIN_H
