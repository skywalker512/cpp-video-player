#pragma once

#include <QtWidgets/QWidget>
#include "ui_Xplay.h"

class Xplay : public QWidget
{
    Q_OBJECT

public:
    Xplay(QWidget *parent = Q_NULLPTR);
	~Xplay();


	// 定时器 刷新滑动条
	void timerEvent(QTimerEvent* e) override;
public slots:
	void OpenFile();
private:
    Ui::XplayClass ui {};
};