#include <QApplication>
#include <QCommandLineParser>
#include "imge.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser commandLineParser;

    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(Imge::tr("[file]"), Imge::tr("Image file to open."));
    commandLineParser.process(QCoreApplication::arguments());

    if (!commandLineParser.positionalArguments().isEmpty())
    {
        Imge imge{commandLineParser.positionalArguments().front()};
        imge.show();
        return a.exec();
    }

    // TODO: print error message with qDebug or qInfo

    return a.exec();
}
