#include <QFileDialog>
#include <QDebug>

#include "Xplay.h"


Xplay::Xplay(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void Xplay::OpenFile()
{
	// ѡ���ļ�
	auto name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ����Ƶ�ļ�"));

	qDebug() << name;
}
