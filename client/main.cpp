#include "widget.h"

#include <QApplication>
#include <QTextDecoder>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    //设置中文编码

//    QTextCodec *codec = QTextCodec::codecForName("utf-8");
//    QTextCodec::setCodecForLocale(codec);


    return a.exec();
}
