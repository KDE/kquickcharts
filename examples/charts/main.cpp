
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QQmlContext>

#include <KDeclarative/KDeclarative>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    KDeclarative::KDeclarative::setupQmlJsDebugger();

    QCommandLineParser parser;
    parser.addOption({QStringLiteral("page"), QStringLiteral("The page to show."), QStringLiteral("page")});
    parser.addHelpOption();
    parser.process(app);

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
