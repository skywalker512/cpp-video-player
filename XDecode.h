#pragma once

#include <mutex>

struct AVCodecParameters;
struct AVCodecContext;
struct AVPacket;
struct AVFrame;
extern void XFreePacket(AVPacket **pkt);
class XDecode
{
public:
	// ��ǰ���뵽�� pts
	long long pts = 0;
	bool isAudio = false;

	//�򿪽�����,���ܳɹ�����ͷ�para�ռ�
	virtual bool Open(AVCodecParameters* para);

	//���͵������̣߳����ܳɹ�����ͷ�pkt�ռ䣨�����ý�����ݣ�
	virtual bool Send(AVPacket *pkt);

	//��ȡ�������ݣ�һ��send������Ҫ���Recv����ȡ�����е�����Send NULL��Recv���
	//ÿ�θ���һ�ݣ��ɵ������ͷ� av_frame_free
	virtual AVFrame* Recv();

	virtual void Close();
	virtual void Clear();

	XDecode();
	virtual ~XDecode();
protected:
	AVCodecContext* codec = nullptr;
	std::mutex mux;
};
