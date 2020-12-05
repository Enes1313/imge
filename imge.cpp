#include "imge.h"
#include "ui_imge.h"
#include <QWindow>
#include <QScreen>
#include <QBitmap>
#include <QDebug>
#include <QDesktopWidget>

//TODO: Fare tekerleği ile labelin pozisyonunun değişmesi.
//TODO: Pencere full iken Fare bir kez tıklandığında, pencerenin küçüklmesi.
//TODO: Pencere küçük iken fare ile iki kez tıklandığında pencerenin full olması.
//TODO: Küçük pencerede arkaplan rengi gri olacak.
//TODO: Büyük pecnereye tekrar gelindiğinde yeni arkaplandaki yeni ekran görüntüsü alınması.

Imge::Imge(const QString& filePath, QWidget* parent) : QMainWindow{parent}, image{filePath}, ui{new Ui::Imge}
{
    ui->setupUi(this);

    /*
     *  Background image, TODO: siyahlaştırma.
     */

    QScreen* screen = QGuiApplication::primaryScreen();
    /*if (const QWindow* window = windowHandle())
        screen = window->screen();*/
    QPixmap bkgnd{screen->grabWindow(0)};
    //bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    /*
    auto active_window = qApp->activeWindow();
    if (active_window) //could be null if your app doesn't have focus
    {
        QPixmap pixmap(active_window->size());
        active_window->render(&pixmap);
        ui->label->setPixmap(pixmap);
    }
    */

    palette.setBrush(QPalette::Background, bkgnd);
    setPalette(palette);

    /*
     *  read image and show, TODO: image sınıfına yaptırılmalı.
     */

    int x, y;
    QPixmap pix{QPixmap::fromImage(image.getImage())};
    QRect screenGeometry = QApplication::desktop()->screenGeometry();

    if (pix.width() < screenGeometry.width())
        x = (screenGeometry.width() - pix.width()) / 2;
    else
    {
        x = screenGeometry.width() / 4;
        pix = pix.scaledToWidth(screenGeometry.width() / 2);
    }

    if (pix.height() < screenGeometry.height())
        y = (screenGeometry.height() - pix.height()) / 2;
    else
    {
        y = screenGeometry.height() / 4;
        pix = pix.scaledToHeight(screenGeometry.height() / 2);
    }

    ui->label->setGeometry(x, y, pix.width(), pix.height());
    ui->label->setPixmap(pix);
    ui->label->raise();

    /*
     * set full screen
     */
    QGuiApplication::setApplicationDisplayName(image.getFileName().toStdString().c_str());
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowState(Qt::WindowMaximized);
}

Imge::~Imge()
{
    delete ui;
}

