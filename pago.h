#ifndef PAGO_H
#define PAGO_H

#include <QDialog>
#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QPrinter>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QtCore>
#include <QTextDocument>
namespace Ui {
class Pago;
}

class Pago : public QDialog
{
    Q_OBJECT

public:
    explicit Pago(QWidget *parent = nullptr,int = 0,double = 0);
    ~Pago();

private slots:
    void on_efectivo_radioButton_clicked();

    void on_tarjeta_radioButton_clicked();

    void on_pagar_pushButton_clicked();

    void on_cancelar_pushButton_clicked();

signals:
    void pagada();

private:
    Ui::Pago *ui;
    int Id_cuenta;
    double Monto;
};

#endif // PAGO_H
