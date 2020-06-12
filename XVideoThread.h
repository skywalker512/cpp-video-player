#pragma once

///�������ʾ��Ƶ
struct AVPacket;
struct AVCodecParameters;
class XDecode;
#include <list>
#include <mutex>
#include <QThread>
#include "IVideoCall.h"

class XVideoThread : public QThread
{
public:
	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters* para, IVideoCall* call, int width, int height);
	virtual void Push(AVPacket* pkt);
	void run() override;

	XVideoThread();
	virtual ~XVideoThread();
	//������
	int maxList = 100;
	bool isExit = false;
	//ͬ��ʱ�䣬���ⲿ����
	long long synpts = 0;
protected:
	std::list<AVPacket*> packs;
	std::mutex mux;
	XDecode* decode = nullptr;
	IVideoCall* call = nullptr;
};
