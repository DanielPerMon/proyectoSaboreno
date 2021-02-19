#ifndef CAJERO_MENU_H
#define CAJERO_MENU_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QWidget>


namespace Ui {
class cajero_menu;
}

class cajero_menu : public QDialog
{
    Q_OBJECT

public:
    explicit cajero_menu(QSqlDatabase, QString, QWidget *parent = nullptr);
    ~cajero_menu();

private slots:
    void on_saliC_pushButton_clicked();

    void on_historial_pushButton_clicked();

private:
    Ui::cajero_menu *ui;
      QSqlDatabase baseCajero;
};

#endif // CAJERO_MENU_H
