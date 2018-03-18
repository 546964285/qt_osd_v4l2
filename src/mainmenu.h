#ifndef MAINMENU_H
#define MAINMENU_H
#include <QWidget>
#include <QDialog>
#include <QPushButton>

class MainMenu : public QDialog
{
    Q_OBJECT

    public:
        MainMenu(QWidget *parent = 0);

    public slots:
        void cppSlot(int index);

    private:

        QPushButton *button;
};

#endif // MAINMENU_H
