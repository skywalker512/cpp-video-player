#pragma once

#include <QtWidgets/QWidget>
#include "ui_Xplay.h"

class Xplay : public QWidget
{
    Q_OBJECT

public:
    Xplay(QWidget *parent = Q_NULLPTR);
	~Xplay();


	// ��ʱ�� ˢ�»�����
	void timerEvent(QTimerEvent* e) override;

	// ���ڳߴ�仯
	void resizeEvent(QResizeEvent* e) override;

	// ˫��ȫ��
	void mouseDoubleClickEvent(QMouseEvent* e) override;

	// ��ͣ
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