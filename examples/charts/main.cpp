
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QQmlContext>
#include <QDebug>
#include <QSurfaceFormat>

#include <KDeclarative/KDeclarative>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    KDeclarative::KDeclarative::setupQmlJsDebugger();

    QCommandLineParser parser;
    parser.addOption({QStringLiteral("page"), QStringLiteral("The page to show."), QStringLiteral("page")});
    parser.addOption({
        QStringLiteral("api"),
        QStringLiteral("The graphics API to use. Can be one of 'default', 'core45', 'compat45', 'compat21' or 'es'."),
        QStringLiteral("api"),
        QStringLiteral("default")
    });
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

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(&engine);
    kdeclarative.setupContext();

    if (parser.isSet(QStringLiteral("page"))) {
        engine.rootContext()->setContextProperty(QStringLiteral("__commandLinePage"), parser.value(QStringLiteral("page")));
    } else {
        engine.rootContext()->setContextProperty(QStringLiteral("__commandLinePage"), nullptr);
    }

    engine.load(QStringLiteral("qrc:/main.qml"));

    return app.exec();
}
