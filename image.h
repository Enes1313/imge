#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>

class Image : public QObject
{
    Q_OBJECT
public:
    explicit Image(QObject *parent = nullptr);

signals:

};

#endif // IMAGE_H
