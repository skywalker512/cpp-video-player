#include "XDemuxThread.h"
#include "XDemux.h"
#include "XVideoThread.h"
#include "XAudioThread.h"
#include <iostream>
#include <QDebug>

extern "C" {
#include <libavformat/avformat.h>
}

#include "XDecode.h"

using namespace std;

void XDemuxThread::run()
{
	while (!isExit)
	{
		mux.lock();

		if (isPause)
		{
			mux.unlock();
			msleep(5);
			continue;
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
			if (at)at->Push(pkt, at, vt, pts);
		}
		else //��Ƶ
		{
			if (vt)vt->Push(pkt, at, vt, pts);
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
	totalMs = demux->totalMs;
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

void XDemuxThread::Close()
{
	isExit = true;
	wait();
	if (vt) vt->Close();
	if (at) at->Close();
	mux.lock();
	delete vt;
	delete at;
	vt = nullptr;
	at = nullptr;
	mux.unlock();
}

void XDemuxThread::Clear()
{
	mux.lock();
	if (demux)
	{
		demux->Clear();
	}
	if (at)
	{
		at->Clear();
	}
	if (vt)
	{
		vt->Clear();
	}
	mux.unlock();
}

void XDemuxThread::Seek(double pos)
{
	Clear();
	mux.lock();
	auto status = this->isPause;
	mux.unlock();
	//��ͣ
	SetPause(true);

	mux.lock();
	if (demux)
	{
		demux->Seek(pos);
	}
	//ʵ��Ҫ��ʾ��λ��pts
	long long seekPts = pos * demux->totalMs;
	while (!isExit)
	{
		AVPacket* pkt = demux->Read();
		if (!pkt) break;
		if (pkt->stream_index == demux->audioStream)
		{
			av_packet_free(&pkt);
			continue;
		}
		//������뵽seekPts
		auto re = vt->decode->Send(pkt);
		if (!re) break;
		auto* frame = vt->decode->Recv();
		if (!frame) continue;
		//����λ��
		if (frame->pts >= seekPts)
		{
			this->pts = frame->pts;
			vt->call->Repaint(frame);
			break;
		}
		av_frame_free(&frame);
	}

	mux.unlock();

	//seek�Ƿ���ͣ״̬
	if (!status)
		SetPause(false);
}

XDemuxThread::XDemuxThread()
{
}


XDemuxThread::~XDemuxThread()
{
	isExit = true;
	wait();
}

void XDemuxThread::SetPause(bool isPause)
{
	this->isPause = isPause;
	if (at)
	{
		at->SetPause(isPause);
	}
	if (vt)
	{
		vt->SetPause(isPause);
	}
}
