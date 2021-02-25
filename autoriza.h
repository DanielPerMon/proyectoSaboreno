#ifndef AUTORIZA_H
#define AUTORIZA_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class autoriza;
}

class autoriza : public QDialog
{
    Q_OBJECT

public:
    explicit autoriza(QWidget *parent = 0);
    ~autoriza();
    int valor();

private slots:
    void on_btnAceptar_clicked();

    void on_btnCancelar_clicked();

private:
    Ui::autoriza *ui;
    int x;
};

#endif // AUTORIZA_H
