#include "XVideoThread.h"
#include "XDecode.h"
#include <iostream>
using namespace std;
//打开，不管成功与否都清理
bool XVideoThread::Open(AVCodecParameters* para, IVideoCall* call, int width, int height)
{
	if (!para)return false;
	mux.lock();

	//初始化显示窗口
	this->call = call;
	if (call)
	{
		call->Init(width, height);
	}

	//打开解码器
	if (!decode) decode = new XDecode();
	int re = true;
	if (!decode->Open(para))
	{
		cout << "audio XDecode open failed!" << endl;
		re = false;
	}
	mux.unlock();
	cout << "XAudioThread::Open :" << re << endl;
	return re;
}

void XVideoThread::Push(AVPacket* pkt)
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
		msleep(1);
	}
}

void XVideoThread::run()
{
	while (!isExit)
	{
		mux.lock();

		//没有数据
		if (packs.empty() || !decode)
		{
			mux.unlock();
			msleep(1);
			continue;
		}

		AVPacket* pkt = packs.front();
		packs.pop_front();
		bool re = decode->Send(pkt);
		if (!re)
		{
			mux.unlock();
			msleep(1);
			continue;
		}
		//一次send 多次recv
		while (!isExit)
		{
			AVFrame* frame = decode->Recv();
			if (!frame) break;
			//显示视频
			if (call)
			{
				call->Repaint(frame);
			}
		}
		mux.unlock();
	}
}

XVideoThread::XVideoThread()
{
}


XVideoThread::~XVideoThread()
{
	//等待线程退出
	isExit = true;
	wait();
}
