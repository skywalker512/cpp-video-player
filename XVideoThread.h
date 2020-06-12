#pragma once

///解码和显示视频
struct AVPacket;
struct AVCodecParameters;
class XDecode;
#include <mutex>
#include "IVideoCall.h"
#include "XDecodeThread.h"

class XVideoThread : public XDecodeThread
{
public:
	//打开，不管成功与否都清理
	virtual bool Open(AVCodecParameters* para, IVideoCall* call, int width, int height);
	void run() override;

	XVideoThread();
	virtual ~XVideoThread();
	//同步时间，由外部传入
	long long synpts = 0;
protected:
	std::mutex vmux;
	IVideoCall* call = nullptr;
};
