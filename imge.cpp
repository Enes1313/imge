#include "imge.h"
#include <QWindow>
#include <QScreen>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QGuiApplication>
#include <QThread>

//TODO: Fare tekerleği ile labelin pozisyonunun değişmesi.
//TODO: Büyük pecnereye tekrar gelindiğinde yeni arkaplandaki yeni ekran görüntüsü alınmasında sorun var.

Imge::Imge(const QString& filePath) : image{filePath}, label{this}
{
    /*
     * Başlangıçta ekran görüntüsü alınır.
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

    setWindowTitle("Imge - " + image.getFileName());
}

void Imge::takeAScreenShot()
{
    QScreen* screen = QGuiApplication::primaryScreen();

    if (const QWindow* window = windowHandle())
        screen = window->screen();

    if (!screen)
        background.fill(Qt::red);
    else
        background = screen->grabWindow(0);
}

void Imge::mouseDoubleClickEvent(QMouseEvent *event)
{
    if ((event->button() != Qt::LeftButton) || (windowState() & Qt::WindowMaximized))
        return;

    grabMouse();

    setVisible(false);

    label.move(x(),y());

    // Buradaki problem, X11 de setVisible işleminden sonra X11 pencereyi hemen görünmez
    // kılmaması. Windowsta sanırım sorun yokmuş. Bununla ilgili iki link aşağıda.
    // https://stackoverflow.com/questions/28977748/qt-screenshot-example-without-timer
    // https://www.qtcentre.org/threads/20985-Ensuring-a-Widget-Is-Hidden
    // Bu probleme çözümüm msleep kullanılması lakin hiç güzel bir çözüm değil.
    QThread::msleep(100);

    takeAScreenShot();
    update();

    setWindowFlag(Qt::FramelessWindowHint, true);
    setWindowState(Qt::WindowMaximized);

    setVisible(true);
}

void Imge::mousePressEvent(QMouseEvent* event)
{
    if (!((event->button() == Qt::RightButton) || (event->button() == Qt::LeftButton)) || !(windowState() & Qt::WindowMaximized))
        return;

    grabMouse();

    setVisible(false);

    /*
     * Linuxta pencere küçüldükten sonra, fareyle pencereyi taşıyın.
     * Resim sağdan monitorunuzun bir yerinden taşsın.
     * Çift tıklayıp window'u maximize edin. Resim hala taşmış bir şekilde duruyor olacaktır.
     * Şimdi ise tek tıklayın. Eğer küçülmüş pencere monitorunuzdan taşmamış ise taşması laızm.
     * Linuxta denedim taşmıyor. Lakin windowsta taşacak yüksek ihtimal.
     *
     * Resim çok küçültülse bile pencere boyutu fazla küçülmemeli. bir sınır koyulmalı.
     * Resim çok büyütülse bile pencere boyutu fazla büyük olmamalı. bir sınır koyulmalı.
     * Bu sınır belki kurucu fonksiyonda ayarlanabilir. Belki burada if ile kontrol edilebilir.
     */

    setWindowFlag(Qt::FramelessWindowHint, false);
    setWindowState(Qt::WindowNoState);

    move(label.x(), label.y());
    resize(label.width(), label.height());
    label.move(0,0);
    background.fill(Qt::white);
    update();

    setVisible(true);
}

void Imge::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.drawPixmap(background.rect(), background);
    painter.fillRect(background.rect(), QBrush(QColor(0, 0, 0, 127)));
}
