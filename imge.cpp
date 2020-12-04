#include "imge.h"
#include "ui_imge.h"

Imge::Imge(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Imge)
{
    ui->setupUi(this);
}

Imge::~Imge()
{
    delete ui;
}

