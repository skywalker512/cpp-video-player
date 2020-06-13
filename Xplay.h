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

	// 窗口尺寸变化
	void resizeEvent(QResizeEvent* e) override;

	// 双击全屏
	void mouseDoubleClickEvent(QMouseEvent* e) override;

	// 暂停
	void SetPause(bool isPause);
public slots:
	void OpenFile();
	void PlayOrPause();
	void SliderPress();
	void SliderRelease();
private:
    Ui::XplayClass ui {};
	bool isSliderPress = false;
};