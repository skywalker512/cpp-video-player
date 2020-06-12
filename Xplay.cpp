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
	// 选择文件
	auto name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));

	qDebug() << name;
}
