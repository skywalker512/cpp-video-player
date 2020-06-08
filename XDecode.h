#pragma once

#include <mutex>

struct AVCodecParameters;
struct AVCodecContext;

class XDecode
{
public:
	bool isAudio = false;

	//�򿪽�����,���ܳɹ�����ͷ�para�ռ�
	virtual bool Open(AVCodecParameters* para);

	virtual void Close();
	virtual void Clear();

	XDecode();
	virtual ~XDecode();
protected:
	AVCodecContext* codec = nullptr;
	std::mutex mux;
};
