#include "imge.h"
#include <QWindow>
#include <QScreen>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QGuiApplication>
#include <QThread>
#include <QtCore>

#ifdef __linux__
#include <xcb/xcb.h>
#elif _WIN32
#include <windows.h>
#endif

Imge::Imge(const QString& filePath) : image{filePath}, label{this}
{
    /*
     * Çeşitli hesaplama sonrası label merkeze konumlandırılır.
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

    setWindowFlag(Qt::FramelessWindowHint, true);       // Title bar kısmı ekran büyükken olmayacak.
    setWindowTitle("Imge - " + image.getFileName());    // Imge - fileName
    expendWindow();
}

void Imge::mouseDoubleClickEvent(QMouseEvent *event)
{
    if ((windowState() & Qt::WindowMaximized))
        return;

    if (event->button() == Qt::LeftButton)
    {
        grabMouse();
        showMaximized();
        //expendWindow();
    }
}

void Imge::mousePressEvent(QMouseEvent* event)
{
    if (!(windowState() & Qt::WindowMaximized))
        return;

    if (event->button() == Qt::LeftButton)
    {
        grabMouse();
        setWindowState(Qt::WindowNoState);
        //restoreWindow();
    }
}

void Imge::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.drawPixmap(background.rect(), background);
    painter.fillRect(background.rect(), QBrush(QColor(0, 0, 0, 127)));
}

/* TODO:
 * Büyülten ifadedeki hesaplamalar değişebilir. (image % 100 den fazla büyütülünce değişsin.)
 * Büyültmede ve küçültmede sınırlar getirilmeli.
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
// TODO: bug, ekran maximize olurken işletim sistemi ilk önce maximize ediyor sonra burası çalışıyor.
// önce burası çalışmalı idi. Bu sebeple nativeEvent kısmı ile bug giderilmeli.
void Imge::changeEvent(QEvent * e)
{
    if(e->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent* event = static_cast<QWindowStateChangeEvent* >( e );

        if (event->oldState() != Qt::WindowNoState && this->windowState() == Qt::WindowNoState)
        {
            restoreWindow();
        }
        else if (event->oldState() != Qt::WindowMaximized && this->windowState() == Qt::WindowMaximized)
        {
            expendWindow();
        }
    }
}

// https://forum.qt.io/topic/104184/is-it-possible-to-intercept-maximizing-the-main-window
// https://forum.qt.io/topic/104533/how-to-ignore-the-event-when-i-click-on-the-button-to-maximize-the-window-windowmaximizebuttonhint/11
bool Imge::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef __linux__
    if (eventType == "xcb_generic_event_t")
    {
        xcb_generic_event_t* ev = static_cast<xcb_generic_event_t *>(message);

        if (XCB_EXPOSE == ev->response_type)
        {
            xcb_expose_event_t* pev = reinterpret_cast<xcb_expose_event_t*>(ev);
        }
    }
#elif _WIN32
    if (eventType == "windows_generic_MSG")
    {
        MSG *msg = static_cast<MSG *>(message);
        if (msg->message == WM_SYSCOMMAND)
        {
            if (msg->wParam == SC_MAXIMIZE)
            {
                //
            }
        }
    }
#endif
    return QWidget::nativeEvent(eventType, message, result);
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

void Imge::expendWindow()
{
    setVisible(false);

    // Buradaki problem, X11 de setVisible işleminden sonra X11 pencereyi hemen görünmez
    // kılmaması. Windowsta sanırım sorun yokmuş. Bununla ilgili iki link aşağıda.
    // https://stackoverflow.com/questions/28977748/qt-screenshot-example-without-timer
    // https://www.qtcentre.org/threads/20985-Ensuring-a-Widget-Is-Hidden
    // Bu probleme çözümüm msleep kullanılması lakin hiç güzel bir çözüm değil.
    QThread::msleep(100);
    /*
     * Ekran maksimum olmadan ekran görüntüsü alınır.
     * Ekran görüntüsü değilde opaklık yapılsaydı : stabil bir kod görmedim.
     * https://forum.qt.io/topic/1090/solved-how-to-get-an-simple-transparent-window
     * https://www.youtube.com/watch?v=AIkyCKpag7s
     */
    takeAScreenShot();
    label.move(x() + label.x(), y() + label.y());

    setWindowState(Qt::WindowMaximized);
    setWindowFlag(Qt::FramelessWindowHint, true);

    update();
    setVisible(true);
}

void Imge::restoreWindow()
{
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
    move(label.x(), label.y());
    label.move(0,0);
    resize(label.width(), label.height());
    background.fill(Qt::white);
    setWindowFlag(Qt::FramelessWindowHint, false);

    QThread::msleep(100);
    update();
    setVisible(true);
}
