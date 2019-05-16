
#include <QApplication>
#include <QQmlApplicationEngine>

#include <KDeclarative/KDeclarative>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    KDeclarative::KDeclarative::setupQmlJsDebugger();

    QQmlApplicationEngine engine;

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(&engine);
    kdeclarative.setupContext();

    engine.load(QStringLiteral("qrc:/main.qml"));

    return app.exec();
}
