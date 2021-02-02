#ifndef MESERO_MENU_H
#define MESERO_MENU_H

#include <QDialog>

namespace Ui {
class mesero_menu;
}

class mesero_menu : public QDialog
{
    Q_OBJECT

public:
    explicit mesero_menu(QWidget *parent = nullptr);
    ~mesero_menu();

private slots:
    void on_salir_clicked();

    void on_verMesas_pushButton_clicked();

private:
    Ui::mesero_menu *ui;
};

#endif // MESERO_MENU_H
