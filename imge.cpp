#include "imge.h"
#include "ui_imge.h"
#include <QWindow>
#include <QScreen>
#include <QBitmap>
#include <QDebug>
#include <QDesktopWidget>
#include <QMouseEvent>

//TODO: Fare tekerleği ile labelin pozisyonunun değişmesi.
//TODO: Pencere full iken Fare bir kez tıklandığında, pencerenin küçüklmesi.
//TODO: Pencere küçük iken fare ile iki kez tıklandığında pencerenin full olması.
//TODO: Küçük pencerede arkaplan rengi gri olacak.
//TODO: Büyük pecnereye tekrar gelindiğinde yeni arkaplandaki yeni ekran görüntüsü alınması.

Imge::Imge(const QString& filePath, QWidget* parent) : QMainWindow{parent}, image{filePath}, ui{new Ui::Imge}
{
    ui->setupUi(this);

    takeAScreenShot();

    int x, y;
    QPixmap pix{QPixmap::fromImage(image.getImage())};
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

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

    ui->label->setGeometry(QRect{x, y, pix.width(), pix.height()});
    ui->label->setPixmap(pix);
    ui->label->raise();

    setWindowFlag(Qt::FramelessWindowHint, true);
    setWindowState(Qt::WindowMaximized);

    QGuiApplication::setApplicationDisplayName(image.getFileName().toStdString().c_str());
}

Imge::~Imge()
{
    delete ui;
}

void Imge::takeAScreenShot()
{
    QScreen* screen = QGuiApplication::primaryScreen();

    if (const QWindow* window = windowHandle())
        screen = window->screen();

    if (!screen)
        return; //throw "";

    background = screen->grabWindow(0);
}

void Imge::mouseDoubleClickEvent(QMouseEvent *event)
{
    if ((event->button() != Qt::LeftButton) || (windowState() & Qt::WindowMaximized))
        return;

    grabMouse();

    hide();

    setWindowFlag(Qt::FramelessWindowHint, true);
    setWindowState(Qt::WindowMaximized);

    takeAScreenShot();
    update();

    show();
}

void Imge::mousePressEvent(QMouseEvent* event)
{
    if (!((event->button() == Qt::RightButton) || (event->button() == Qt::LeftButton)) || !(windowState() & Qt::WindowMaximized))
        return;

    grabMouse();

    hide();

    setWindowFlag(Qt::FramelessWindowHint, false);
    setWindowState(Qt::WindowNoState);

    /*
        sg = QDesktopWidget().availableGeometry()
        fg = window_object.frameGeometry
        wg = window_object.geometry
    */

    move(ui->label->x(), ui->label->y());
    resize(ui->label->width(), ui->label->height());
    background.fill(Qt::white);
    update();

    show();
}

void Imge::paintEvent(QPaintEvent* event)
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(background.rect(), background);
    painter.fillRect(background.rect(), QBrush(QColor(0, 0, 0, 127)));
}
