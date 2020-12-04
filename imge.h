#ifndef IMGE_H
#define IMGE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Imge; }
QT_END_NAMESPACE

class Imge : public QMainWindow
{
    Q_OBJECT

public:
    Imge(QWidget *parent = nullptr);
    ~Imge();

private:
    Ui::Imge *ui;
};
#endif // IMGE_H
