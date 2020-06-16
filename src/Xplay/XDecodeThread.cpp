#include "XDecodeThread.h"
#include "XDecode.h"
#include <QDebug>
#include "XVideoThread.h"
#include "XAudioThread.h"

void XDecodeThread::Push(AVPacket* pkt, XAudioThread* at, XVideoThread* vt, long long &pts)
{
	if (!pkt)return;
	while (!isExit)
	{
		// ���������
		mux.lock();
		//����Ƶͬ��
		if (vt && at)
		{
			pts = at->pts;
			vt->synpts = at->pts;
		}
		if (packs.size() < maxList)
		{
			packs.push_back(pkt);
			mux.unlock();
			break;
		}
		mux.unlock();
		msleep(1);
	}
}

void XDecodeThread::Clear()
{
	mux.lock();
	decode->Clear();
	while (!packs.empty())
	{
		AVPacket* pkt = packs.front();
		XFreePacket(&pkt);
		packs.pop_front();
	}
	mux.unlock();
}

void XDecodeThread::Close()
{
	Clear();
	isExit = true;
	wait();

	decode->Close();

	mux.lock();
	delete decode;
	decode = nullptr;
	mux.unlock();
}

AVPacket* XDecodeThread::Pop()
{
	mux.lock();
	if (packs.empty())
	{
		mux.unlock();
		return nullptr;
	}
	AVPacket* pkt = packs.front();
	packs.pop_front();
	mux.unlock();
	return pkt;
}

XDecodeThread::XDecodeThread()
{
	//�򿪽�����
	if (!decode) decode = new XDecode();
}


XDecodeThread::~XDecodeThread()
{
	//�ȴ��߳��˳�
	isExit = true;
	wait();
}
