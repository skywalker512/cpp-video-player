#include "XDemuxThread.h"
#include "XDemux.h"
#include "XVideoThread.h"
#include "XAudioThread.h"
#include <iostream>
#include <QDebug>
using namespace std;

void XDemuxThread::run()
{
	while (!isExit)
	{
		mux.lock();
		
		//����Ƶͬ��
		if (vt && at)
		{
			vt->synpts = at->pts;
		}
		if (!demux)
		{
			mux.unlock();
			msleep(5);
			continue;
		}
		AVPacket* pkt = demux->Read();
		if (!pkt)
		{
			mux.unlock();
			msleep(5);
			continue;
		}
		//�ж���������Ƶ
		if (demux->IsAudio(pkt))
		{
			if (at)at->Push(pkt);
		}
		else //��Ƶ
		{
			if (vt)vt->Push(pkt);
			msleep(1);
		}
		mux.unlock();
	}
}


bool XDemuxThread::Open(const char* url, IVideoCall* call)
{
	if (url == nullptr || url[0] == '\0')
		return false;

	mux.lock();
	if (!demux) demux = new XDemux();
	if (!vt) vt = new XVideoThread();
	if (!at) at = new XAudioThread();

	//�򿪽��װ
	bool re = demux->Open(url);
	if (!re)
	{
		cout << "demux->Open(url) failed!" << endl;
		return false;
	}
	//����Ƶ�������ʹ����߳�
	if (!vt->Open(demux->CopyVPara(), call, demux->width, demux->height))
	{
		re = false;
		cout << "vt->Open failed!" << endl;
	}
	//����Ƶ�������ʹ����߳�
	if (!at->Open(demux->CopyAPara(), demux->sampleRate, demux->channels))
	{
		re = false;
		cout << "at->Open failed!" << endl;
	}
	mux.unlock();
	cout << "XDemuxThread::Open " << re << endl;
	return re;
}

//���������߳�
void XDemuxThread::Start()
{
	mux.lock();
	if (!demux) demux = new XDemux();
	if (!at) at = new XAudioThread();
	if (!vt) vt = new XVideoThread();
	//������ǰ�߳�
	start();
	if (at)at->start();
	if (vt)vt->start();

	mux.unlock();
}

XDemuxThread::XDemuxThread()
{
}


XDemuxThread::~XDemuxThread()
{
	isExit = true;
	wait();
}
