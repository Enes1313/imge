#include <QApplication>
#include <QCommandLineParser>
#include "imge.h"

/*
 * Bu kaynak dosyada, programa gelecek dosya parametresi alınmaktadır.
 * Dosya parametresi tabii olarak dosya yoludur.
 * Eğer parametre varsa, ilk parametre Imge sınıfına gonderilir.
 * Imge sınıfı nesne oluşturur, eğer dosya yolunda image dosyası yoksa,
 * Geçersiz resim yazısı içerikli siyah kare resim ekranda gösterilmelidir.
 * Şuan bu özellik olmadığı gibi bu resmin oluşturulması Image sınıfının,
 * gösterilmesi ise Imge sınıfının sorumluluğundadır.
 *
 * Bu dosyada commandline ile yapılacak değişiklikler için,
 * https://doc.qt.io/qt-5/qcommandlineparser.html kaynağına başvurunuz.
 *
 * Bu dosyada ileri bir zamanda, program açıkken tekrar çalıştırılması durumunda yeni bir
 * örnek açılmadan yeni dosya yolundaki dosyanın açılmış pencerede gösterilmesi için
 * değişiklik yapılabilir. Belki bunun için ayrı bir sınıfta var olabilir. Kısaca bu program
 * hafıza da en fazla 1 tane olmalıdır. Screenshot özelliğine dikkat edilmeli ona göre
 * gerekiyorsa pencere küçültülmeli.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // for Qt options
    QCommandLineParser commandLineParser;

    commandLineParser.addHelpOption();  // (-h, --help and -? on Windows) --help-all, Qt-specific options
    commandLineParser.addPositionalArgument(Imge::tr("[file]"), Imge::tr("Image file to open.")); // Kullanım
    commandLineParser.process(QCoreApplication::arguments());

    if (!commandLineParser.positionalArguments().isEmpty())
    {
        /*
         * Parametre olara komut satırındaki bu programın çağrılmasından sonraki ilk parametre geçiliyor.
         */
        Imge imge{commandLineParser.positionalArguments().front()};
        /*
         * Ekrana gösterilmesi bu show komutu ile sağlanıyor.
         */
        imge.show();
        /*
         * Imge imge; yerel değiken olduğundan return a.exec() i buraya da koyuyoruz.
         */
        return a.exec();
    }
    else
    {
        // TODO: print message with qDebug or qInfo (qDebug ile qInfo farkına bakınız.)
    }

    return a.exec();
}
