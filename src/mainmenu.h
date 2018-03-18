#ifndef MAINMENU_H
#define MAINMENU_H
#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QDeclarativeView>

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = 0);

private:

    QPushButton *button;
};

#endif // MAINMENU_H
