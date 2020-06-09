#include <iostream>
#include <QtWidgets/QApplication>
#include <QThread>

#include "XDecode.h"
#include "Xplay.h"
#include "XDemux.h"

using namespace std;



class TestThread :public QThread
{
public:
	void Init()
	{
		//�������
		char *url = "2.mp4";
		
		cout << "demux.Open = " << demux.Open(url);
		cout << "CopyVPara = " << demux.CopyVPara() << endl;
		cout << "CopyAPara = " << demux.CopyAPara() << endl;
		//cout << "seek=" << demux.Seek(0.95) << endl;

		/////////////////////////////

		cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
		//vdecode.Clear();
		//vdecode.Close();
		cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;

	}
	void run()
	{
		for (;;)
		{
			AVPacket *pkt = demux.Read();
			if (demux.IsAudio(pkt))
			{
				//adecode.Send(pkt);
				//AVFrame *frame = adecode.Recv();
				//cout << "Audio:" << frame << endl;
			}
			else
			{
				vdecode.Send(pkt);
				AVFrame *frame = vdecode.Recv();
				video->Repaint(frame);
				msleep(40);
				//cout << "Video:" << frame << endl;
			}
			if (!pkt)break;
		}
	}
	///����XDemux
	XDemux demux;
	///�������
	XDecode vdecode;
	XDecode adecode;
	XVideoWidget *video;

};


int main(int argc, char *argv[])
{
	
	//��ʼ����ʾ
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


	//��ʼ��gl����
	w.ui.video->Init(tt.demux.width, tt.demux.height);
	tt.video = w.ui.video;
	tt.start();
	
    return a.exec();
}
