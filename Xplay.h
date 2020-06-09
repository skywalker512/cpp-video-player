#pragma once

#include <QtWidgets/QWidget>
#include "ui_Xplay.h"

class Xplay : public QWidget
{
    Q_OBJECT

public:
    Xplay(QWidget *parent = Q_NULLPTR);

//private:
    Ui::XplayClass ui {};
};
