#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScreen>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QMainWindow>
#include <QString>
#include <opencv2/opencv.hpp>
#include "TrackingDataReceiver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
        void run(QString vid_path, int fps, QScreen *screen);

private:
        void writeCsvHeader(QFile* file, int vid_width, int vid_height, int disp_width, int disp_height);
        void writeCsvData(QFile* file, int frame_idx, int time, bool visible, double x, double y);
    QFile *tracking_data;
    TrackingDataReceiver tracker;
    std::int64_t trackerInitialTimestamp;
    int video_width;
    int video_height;
    int display_width;
    int display_height;
    int frame_idx;
    bool target_visible;

    //void closeTrackingFile();
    //void videoFrameReceived(const QVideoFrame& frame);
    //void videoKeyPressed(QKeyEvent *e);
    //void videoKeyReleased(QKeyEvent *e);
    //void videoSelected(const QString& path);
    //void videoStateChanged(QMediaPlayer::State state);
    //void playbackSpeedChanged(double rate);
    //bool isPlaying();
    //std::int64_t getTime(const TrackingData& data);
};

#endif // MAINWINDOW_H
