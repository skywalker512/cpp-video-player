#pragma once
///�������ʾ��Ƶ
struct AVPacket;
class XDecode;
#include <list>
#include <mutex>
#include <QThread>
class XAudioThread;
class XVideoThread;

class XDecodeThread : public QThread
{
public:
	XDecodeThread();
	virtual ~XDecodeThread();
	virtual void Push(AVPacket* pkt, XAudioThread* at, XVideoThread* vt, long long &pts);
	// �������
	virtual void Clear();
	// ������Դ���ر��߳�
	virtual void Close();

	//ȡ��һ֡���ݣ�����ջ�����û�з���NULL
	virtual AVPacket* Pop();
	//������
	int maxList = 100;
	bool isExit = false;
	XDecode* decode = nullptr;
protected:
	std::list<AVPacket*> packs;
	std::mutex mux;
};