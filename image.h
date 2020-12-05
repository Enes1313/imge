#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QImage>

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
