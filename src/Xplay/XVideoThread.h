#pragma once

///�������ʾ��Ƶ
struct AVPacket;
struct AVCodecParameters;
class XDecode;
#include <mutex>
#include "IVideoCall.h"
#include "XDecodeThread.h"

class XVideoThread : public XDecodeThread
{
public:
	//�򿪣����ܳɹ��������
	virtual bool Open(AVCodecParameters* para, IVideoCall* call, int width, int height);
	void run() override;

	XVideoThread();
	virtual ~XVideoThread();
	//ͬ��ʱ�䣬���ⲿ����
	long long synpts = 0;
	void SetPause(bool isPause);
	bool isPause = false;
	IVideoCall* call = nullptr;
protected:
	std::mutex vmux;
};
