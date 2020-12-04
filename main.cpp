#include "imge.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDebug>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(Imge::tr("[file]"), Imge::tr("Image file to open."));
    commandLineParser.process(QCoreApplication::arguments());
    QFileInfo file{commandLineParser.positionalArguments().front()};

    if (!commandLineParser.positionalArguments().isEmpty() && file.isFile()) {
        Imge w;
        QGuiApplication::setApplicationDisplayName(Imge::tr(file.fileName().toStdString().c_str()));
        w.setWindowState(Qt::WindowFullScreen);
        w.show();
        a.exec();
    }
    return 0;
}
