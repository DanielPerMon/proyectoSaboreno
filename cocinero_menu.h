#ifndef COCINERO_MENU_H
#define COCINERO_MENU_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QVariant>
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>
#include <QObject>
#include <QDebug>
namespace Ui {
class cocinero_menu;
}

class cocinero_menu : public QDialog
{
    Q_OBJECT

public:
    explicit cocinero_menu(QSqlDatabase, QString, QWidget *parent = nullptr);
    ~cocinero_menu();



private slots:
    void on_ordenlista_pushButton_clicked();

    void ActualizarTablas();

    void OrdenElegida(int);

    void on_Csalir_pushButton_clicked();

private:
    Ui::cocinero_menu *ui;
    QSqlDatabase baseCocinero;
    int FilaOrdenCola;
};

#endif // COCINERO_MENU_H
