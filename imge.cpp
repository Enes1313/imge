#include "imge.h"
#include <QWindow>
#include <QScreen>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QGuiApplication>
#include <QThread>
#include <QtCore>

Imge::Imge(const QString& filePath) : image{filePath}, label{this}
{
    /*
     * Başlangıçta ekran görüntüsü alınır.
     * Ekran görüntüsü değilde opaklık yapılsaydı : stabil bir kod görmedim.
     * https://forum.qt.io/topic/1090/solved-how-to-get-an-simple-transparent-window
     * https://www.youtube.com/watch?v=AIkyCKpag7s
     */

    takeAScreenShot();

    /*
     * Çeşitli hesaplama sonrası label merkeze set edilir.
     */
    int imgx, imgy;
    QPixmap pix{QPixmap::fromImage(image.getImage())};
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    if (pix.width() < screenGeometry.width())
    {
        imgx = (screenGeometry.width() - pix.width()) / 2;
        imgFirstAreaW = pix.width();
    }
    else
    {
        imgx = screenGeometry.width() / 4;
        imgFirstAreaW = screenGeometry.width() / 2;
    }

    if (pix.height() < screenGeometry.height())
    {
        imgy = (screenGeometry.height() - pix.height()) / 2;
        imgFirstAreaH = pix.height();
    }
    else
    {
        imgy = screenGeometry.height() / 4;
        imgFirstAreaH = screenGeometry.height() / 2;
    }

    label.setScaledContents(true);
    label.setGeometry(imgx, imgy, imgFirstAreaW, imgFirstAreaH);
    label.setPixmap(pix);

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

/* TODO:
 * Küçültme ve büyülten ifadedeki hesaplamalar değişmeli optimal bir algoritma olmalı.
 */
void Imge::wheelEvent(QWheelEvent *event)
{
    static int w1{}, w2{}, h1{}, h2{};
    static int mX{}, mY{}, lX{}, lY{};
    static int mouseX{-1}, mouseY{-1};

    if (mouseX != event->x() || mouseY != event->y())
    {
        mouseX = event->x();
        mouseY = event->y();

        if (label.geometry().contains(event->pos()))
        {
            mX = event->pos().x();
            mY = event->pos().y();
        }
        else if (label.geometry().contains(width() / 2, height() / 2))
        {
            mX = width() / 2;
            mY = height() / 2;
        }
        else
        {
            mX = label.x() + label.width() / 2;
            mY = label.y() + label.height() / 2;
        }

        lX = label.x();
        lY = label.y();
        w1 = label.width();
        h1 = label.height();
    }


    if (event->delta() < 0)
    {
        zoomFactor = qPow(0.87, ++zoom);
    }
    else
    {
        zoomFactor = qPow(0.87, --zoom);
    }

    label.resize(w2 = (imgFirstAreaW * zoomFactor - 1), h2 = (imgFirstAreaH * zoomFactor - 1));
    label.move(mX - qRound(static_cast<double>(w2 * (mX - lX)) / w1), mY - qRound(static_cast<double>(h2 * (mY - lY)) / h1));
}
