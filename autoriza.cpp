#include "autoriza.h"
#include "ui_autoriza.h"

autoriza::autoriza(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::autoriza)
{
    ui->setupUi(this);
}

autoriza::~autoriza()
{
    delete ui;
}
