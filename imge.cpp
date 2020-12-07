#include "imge.h"
#include <QWindow>
#include <QScreen>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QGuiApplication>

//TODO: Fare tekerleği ile labelin pozisyonunun değişmesi.
//TODO: Büyük pecnereye tekrar gelindiğinde yeni arkaplandaki yeni ekran görüntüsü alınmasında sorun var.

Imge::Imge(const QString& filePath) : image{filePath}, label{this}
{
    /*
     * Bşangıçta ekran görüntüsü alınır.
     */
    takeAScreenShot();

    /*
     * Çeşitli hesaplama sonrası label merkeze set edilir.
     */
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

    label.setGeometry(QRect{x, y, pix.width(), pix.height()});
    label.setPixmap(pix);
    label.raise();

    /*
     * Tam full ekran olması istenmiyor ama bu ayar güzel.
     */
    setWindowFlag(Qt::FramelessWindowHint, true);
    setWindowState(Qt::WindowMaximized);

    //QGuiApplication::setApplicationDisplayName(image.getFileName().toStdString().c_str());
    setWindowTitle(QCoreApplication::translate("Imge", "Imge", nullptr));
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

    move(label.x(), label.y());
    resize(label.width(), label.height());
    background.fill(Qt::white);
    update();

    show();
}

void Imge::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawPixmap(background.rect(), background);
    painter.fillRect(background.rect(), QBrush(QColor(0, 0, 0, 127)));
}
