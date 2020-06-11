#include "XVideoThread.h"
#include "XDecode.h"
#include <iostream>
using namespace std;
//�򿪣����ܳɹ��������
bool XVideoThread::Open(AVCodecParameters* para, IVideoCall* call, int width, int height)
{
	if (!para)return false;
	mux.lock();

	//��ʼ����ʾ����
	this->call = call;
	if (call)
	{
		call->Init(width, height);
	}

	//�򿪽�����
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
	//����
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

		//û������
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
		//һ��send ���recv
		while (!isExit)
		{
			AVFrame* frame = decode->Recv();
			if (!frame) break;
			//��ʾ��Ƶ
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
	//�ȴ��߳��˳�
	isExit = true;
	wait();
}
