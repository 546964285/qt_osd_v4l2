#ifndef BACKPLAY_H
#define BACKPLAY_H
#include <QWidget>
#include <QDialog>
#include <QPushButton>

class BackPlay : public QDialog
{
    Q_OBJECT

public:
    BackPlay(QWidget *parent = 0);

private:

    QPushButton *button;
};

#endif // MAINMENU_H
