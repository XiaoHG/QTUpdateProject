#include "mainwindow.h"
#include "filemanager.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载中文翻译
    QTranslator qtTranslator;
    qtTranslator.load("orbitEditor_zh_CN.qm",":/");
    a.installTranslator(&qtTranslator);

    //设置全局字体
    QFont font  = a.font();
    font.setPointSize(10);
    font.setFamily("黑体");
    a.setFont(font);

    MainWindow w;
    w.show();

    FileManager fm;
    fm.Print();

    return a.exec();
}
