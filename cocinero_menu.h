#ifndef COCINERO_MENU_H
#define COCINERO_MENU_H

#include <QDialog>

namespace Ui {
class cocinero_menu;
}

class cocinero_menu : public QDialog
{
    Q_OBJECT

public:
    explicit cocinero_menu(QWidget *parent = nullptr);
    ~cocinero_menu();



private:
    Ui::cocinero_menu *ui;
};

#endif // COCINERO_MENU_H
