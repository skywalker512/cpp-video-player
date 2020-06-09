#include <iostream>
#include <QtWidgets/QApplication>
#include <QThread>


#include "XAudioPlay.h"
#include "XDecode.h"
#include "Xplay.h"
#include "XDemux.h"
#include "XResample.h"

using namespace std;


class TestThread : public QThread
{
public:
	void Init()
	{
		//香港卫视
		char* url = "2.mp4";
		
		
		cout << "demux.Open = " << demux.Open(url);
		cout << "CopyVPara = " << demux.CopyVPara() << endl;
		cout << "CopyAPara = " << demux.CopyAPara() << endl;
		//cout << "seek=" << demux.Seek(0.95) << endl;

		cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
		//vdecode.Clear();
		//vdecode.Close();
		cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
		cout << "resample.Open = " << resample.Open(demux.CopyAPara()) << endl;
		cout << demux.channels << endl;
		cout << demux.sampleRate << endl;

		XAudioPlay::Get()->channels = demux.channels;
		XAudioPlay::Get()->sampleRate = demux.sampleRate;

		cout << "XAudioPlay::Get()->Open() = " << XAudioPlay::Get()->Open() << endl;
	}

	unsigned char* pcm = new unsigned char[1024 * 1024];

	void run() override
	{
		for (;;)
		{
			AVPacket* pkt = demux.Read();
			if (demux.IsAudio(pkt))
			{
				adecode.Send(pkt);
				AVFrame* frame = adecode.Recv();
				cout << "Resample:" << resample.Resample(frame, pcm) << " ";
				//cout << "Audio:" << frame << endl;
			}
			else
			{
				vdecode.Send(pkt);
				AVFrame* frame = vdecode.Recv();
				video->Repaint(frame);
				msleep(40);
				//cout << "Video:" << frame << endl;
			}
			if (!pkt)break;
		}
	}

	///测试XDemux
	XDemux demux;
	///解码测试
	XDecode vdecode;
	XDecode adecode;
	XResample resample;
	XVideoWidget* video;
};


int main(int argc, char* argv[])
{
	//初始化显示
	/*

	for (;;)
	{
		AVPacket *pkt = demux.Read();
		if (demux.IsAudio(pkt))
		{
			adecode.Send(pkt);
			AVFrame *frame = adecode.Recv();
			//cout << "Audio:" << frame << endl;
		}
		else
		{
			vdecode.Send(pkt);
			AVFrame *frame = vdecode.Recv();
			//cout << "Video:" << frame << endl;
		}
		if (!pkt)break;
	}*/

	TestThread tt;
	tt.Init();


	QApplication a(argc, argv);
	Xplay w;
	w.show();


	//初始化gl窗口
	w.ui.video->Init(tt.demux.width, tt.demux.height);
	tt.video = w.ui.video;
	tt.start();

	return a.exec();
}
