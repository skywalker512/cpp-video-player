#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "Xplay.h"
#include "XDemuxThread.h"

static XDemuxThread dt;

Xplay::Xplay(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	dt.Start();
	startTimer(40);
}

Xplay::~Xplay()
{
	dt.Close();
}

void Xplay::timerEvent(QTimerEvent* e)
{
	auto total = dt.totalMs;

	if (total > 0)
	{
		const auto pos = static_cast<double>(dt.pts) / static_cast<double>(total);
		const auto v = ui.playPos->maximum() * pos;
		ui.playPos->setValue(v);
	}
}

void Xplay::OpenFile()
{
	// ѡ���ļ�
	auto name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ����Ƶ�ļ�"));

	if (name.isEmpty())return;
	this->setWindowTitle(name);
	if (!dt.Open(name.toLocal8Bit(), ui.video))
	{
		QMessageBox::information(nullptr, "error", "open file failed!");
	}
}
