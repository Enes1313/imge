#ifndef IMGE_H
#define IMGE_H

#include <QMainWindow>
#include <image.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Imge; }
QT_END_NAMESPACE

class Imge : public QMainWindow
{
    Q_OBJECT

public:
    Imge(const QString& filePath, QWidget* parent = nullptr);
    ~Imge();

private:
    Image image;
    Ui::Imge* ui;
    QPalette palette;
};
#endif // IMGE_H
