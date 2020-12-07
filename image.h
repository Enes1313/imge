#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QImage>

/*
 * Bu sınıf kullanıcının görüntülemek istediği resim dosyasını tutan sınıftır. İler izamanda bu sınıf
 * QLabel sınıfından türetilmiş bir widget olarak tasarlanabilir.
 */
class Image : public QObject
{
    Q_OBJECT
public:
    explicit Image(const QString& filePath, QObject* parent = nullptr);
    const QString& getFileName(void);
    const QImage& getImage(void);
signals:

private:
    QString fileName;
    QImage image;
};

#endif // IMAGE_H
