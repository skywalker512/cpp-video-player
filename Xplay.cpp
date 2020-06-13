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

void Xplay::resizeEvent(QResizeEvent* e)
{
	// 移动控件位置
	ui.playPos->move(50, this->height() - 50);
	ui.openFile->move(50, this->height() - 100);
	ui.isplay->move(ui.openFile->x() + ui.openFile->width() + 10, ui.openFile->y());

	// 滑动条宽度变化
	ui.playPos->resize(this->width() - 100, ui.playPos->height());
	// video 进行尺寸变化
	ui.video->resize(this->size());
}

void Xplay::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (isFullScreen())
	{
		this->showNormal();
	}
	else
	{
		this->showFullScreen();
	}
}

void Xplay::SetPause(bool isPause)
{
	if (isPause)
	{
		ui.isplay->setText(QString::fromLocal8Bit("播放"));
	} else
	{
		ui.isplay->setText(QString::fromLocal8Bit("暂停"));
	}
}

void Xplay::OpenFile()
{
	// 选择文件
	auto name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择视频文件"));

	if (name.isEmpty())return;
	this->setWindowTitle(name);
	if (!dt.Open(name.toLocal8Bit(), ui.video))
	{
		QMessageBox::information(nullptr, "error", "open file failed!");
	}
	SetPause(dt.isPause);
}

void Xplay::PlayOrPause()
{
	auto isPause = !dt.isPause;
	SetPause(isPause);
	dt.SetPause(isPause);
}
