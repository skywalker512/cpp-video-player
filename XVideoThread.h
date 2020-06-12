#pragma once

///解码和显示视频
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
	//打开，不管成功与否都清理
	virtual bool Open(AVCodecParameters* para, IVideoCall* call, int width, int height);
	virtual void Push(AVPacket* pkt);
	void run() override;

	XVideoThread();
	virtual ~XVideoThread();
	//最大队列
	int maxList = 100;
	bool isExit = false;
	//同步时间，由外部传入
	long long synpts = 0;
protected:
	std::list<AVPacket*> packs;
	std::mutex mux;
	XDecode* decode = nullptr;
	IVideoCall* call = nullptr;
};
