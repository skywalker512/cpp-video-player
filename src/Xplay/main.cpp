#include "Xplay.h"

using namespace std;

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	// ui 组件 继承 QWidget
	Xplay w;
	// 显示窗口
	w.show();

	// qt 主线程循环
	return a.exec();
}
