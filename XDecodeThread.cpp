#include "XDecodeThread.h"
#include "XDecode.h"
#include <QDebug>
void XDecodeThread::Push(AVPacket *pkt)
{
	if (!pkt)return;
	//阻塞
	while (!isExit)
	{
		mux.lock();
		if (packs.size() < maxList)
		{
			packs.push_back(pkt);
			mux.unlock();
			break;
		}
		mux.unlock();
	}
}

AVPacket* XDecodeThread::Pop()
{
	mux.lock();
	if (packs.empty())
	{
		mux.unlock();
		return NULL;
	}
	AVPacket *pkt = packs.front();
	packs.pop_front();
	mux.unlock();
	return pkt;
}

XDecodeThread::XDecodeThread()
{
	//打开解码器
	if (!decode) decode = new XDecode();
}


XDecodeThread::~XDecodeThread()
{	//等待线程退出
	isExit = true;
	wait();
}
