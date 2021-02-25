#ifndef AUTORIZA_H
#define AUTORIZA_H

#include <QDialog>

namespace Ui {
class autoriza;
}

class autoriza : public QDialog
{
    Q_OBJECT

public:
    explicit autoriza(QWidget *parent = 0);
    ~autoriza();

private:
    Ui::autoriza *ui;
};

#endif // AUTORIZA_H
