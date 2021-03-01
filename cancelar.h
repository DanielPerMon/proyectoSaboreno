#ifndef CANCELAR_H
#define CANCELAR_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QHBoxLayout>
#include "autoriza.h"

namespace Ui {
class cancelar;
}

class cancelar : public QDialog
{
    Q_OBJECT

public:
    explicit cancelar(QWidget *parent = 0, int = 0, int = 0);
    int val();
    ~cancelar();

private slots:
    void on_btnAceptar_clicked();

    void on_btnCancelar_clicked();

    void borrarItem();

private:
    Ui::cancelar *ui;
    int cta;
    int mesa;
    void cargarOrden();
    int valor;
    int montoAux;
    QList<QString> idOrden;
};

#endif // CANCELAR_H
