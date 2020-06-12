#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLShaderProgram>
#include <mutex>

#include "IVideoCall.h"
struct AVFrame;
class XVideoWidget : public QOpenGLWidget, protected QOpenGLFunctions, public IVideoCall
{
	Q_OBJECT

public:
	void Init(int width, int height);
	XVideoWidget(QWidget *parent);
	~XVideoWidget();

	//���ܳɹ�����ͷ�frame�ռ�
	virtual void Repaint(AVFrame *frame);
protected:
	//ˢ����ʾ
	void paintGL() override;

	//��ʼ��gl
	void initializeGL() override;

	// ���ڳߴ�仯
	void resizeGL(int width, int height) override;
private:
	std::mutex mux;

	//shader����
	QGLShaderProgram program;

	//shader��yuv������ַ
	GLuint unis[3] = { 0 };
	//openg�� texture��ַ
	GLuint texs[3] = { 0 };

	//�����ڴ�ռ�
	unsigned char *datas[3] = { 0 };

	int width = 240;
	int height = 128;

};

