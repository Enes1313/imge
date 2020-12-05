#include "image.h"
#include <QDir>
#include <QImageReader>
#include <QColorSpace>
#include <QFileInfo>

Image::Image(const QString& filePath, QObject* parent) : QObject{parent}
{
    QImageReader reader{filePath};
    QFileInfo file{filePath};

    reader.setAutoTransform(true);

    image = reader.read();
    fileName = file.fileName();

    if (image.isNull()) {
        // throw tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(fileName), reader.errorString());
    }

    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);

    /*
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
            statusBar()->showMessage(message);
    */
}

const QString& Image::getFileName(void) // TODO:
{
    return fileName;
}


const QImage& Image::getImage(void)
{
    return image;
}
