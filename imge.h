#ifndef IMGE_H
#define IMGE_H

#include <QWidget>
#include <QLabel>
#include "image.h"

/*
 * Bu sınıf fotoğraf görüntüleyicinin ana ekranını oluşturma sorumluluğunu taşımaktadır. QMainWindow sınıfından
 * türetilmişti. Lakin miras alınan sınıf QWidget olarak değiştirildi. Ui kaldırıldı.
 *
 * Bu sınıf temel en üst hiyerarşiye sahip sınıftır. Bu sebeple bu sınıftan oluşacak nesnenin parent'ı yoktur.
 * Sınıf içinde QLabel sınıfından bir veri elemanı vardır. Kurucu fonksiyonda imge.cpp de hayata gelirken this
 * parametresi verilmiştir. Çünkü QLabel'in parent'i bu sınıftır. Eğer bu yapılmazsa ekranda QLabel görülmez.
 *
 * QLabel türünden veri elemanı bu sınıfın child elemanıdır. QLabel'e kullanıcının görüntülemek istediği resim
 * dosyası aktarılmaktadır. Bu resim dosyası Image sınıfından alınmaktadır. Image sınıfı ise  Imge sınıfı hayata
 * gelirken aldığı dosya path'i üzerinden resim dosyasına erişmektedir. QLabel türünden veri elemanı yerine
 * daha sonra QLabel sınıfından türetilmiş bir sınıf kullanılabilir. Belki bu sınıf şuan var olan Image sınıfı
 * olabilir.
 *
 * QPixmap verielemanı ise program açılmadan önceki ekranın görüntüsünü tutmaktadır.
 *
 * İleri zamanda bottomtabs, sağ üstteki çıkış yuşu, sağ tuşa basıldığında çıkması gereken tablar eklenmelidir.
 * Mouse çift tık eventi, mouse tekerleğinin hareket ettirilmesieventi ve mouse tuşunuun basılıp hareket edilmesi
 * eventi hem QLabel için hem de Imge için anlamları olan eventlerdir.
 *
 * takeAScreenShot üye fonksiyonu ekran görüntüsü almakla sorumldur. Bir işletim sisteminde sorunlu çalışan
 * arkaplan resmi için ilk bakılması gereken yerlerden biridir.
 *
 * Imge sınıfı mouseDoubleClickEvent, mousePressEvent, paintEvent isimli fonksiyonları override etmiştir.
 */

class Imge : public QWidget
{
    Q_OBJECT

public:
    Imge(const QString& filePath);

private:
    Image image;
    QLabel label;
    QPixmap background;
    int imgFirstAreaW, imgFirstAreaH;
    int zoom{};
    double zoomFactor{};

    void takeAScreenShot();
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void wheelEvent(QWheelEvent *event) override;
    void changeEvent( QEvent* e ) override;
};

#endif // IMGE_H
