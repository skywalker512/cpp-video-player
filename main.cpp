#include "Xplay.h"

using namespace std;

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	// ui ��� �̳� QWidget
	Xplay w;
	// ��ʾ����
	w.show();

	// qt ���߳�ѭ��
	return a.exec();
}
