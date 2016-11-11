#include "mainwindow.cpp"
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QCommandLineParser>
#include <string>


int main(int argc, char *argv[]){
    QApplication app(argc,argv);
    QScreen *screen =  QApplication::screens()[0]; //Video should be displayed on "first" screen
    MainWindow w;
    QString fps_str = QString(argv[2]);
    int fps = fps_str.toInt();
    w.run(argv[1],fps,screen);
    return app.exec();
}
