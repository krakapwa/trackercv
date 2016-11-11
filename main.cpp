#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <string>

//void writeCsvHeader(const QFile& file, int vid_width, int vid_height, int disp_width, int disp_height, TrackingDataReceiver tracker){
void writeCsvHeader(QFile* file, int vid_width, int vid_height, int disp_width, int disp_height){

    file->write(QString("VideoWidth:%1\n").arg(vid_width).toStdString().c_str());
    file->write(QString("VideoHeight:%1\n").arg(vid_height).toStdString().c_str());
    file->write(QString("DisplayWidth:%1\n").arg(disp_width).toStdString().c_str());
    file->write(QString("DisplayHeight:%1\n").arg(disp_height).toStdString().c_str());

    //change when tracker is added!
    //trackerInitialTimestamp = state.timestamp;
    //tracking_data->write("frame;time;visible;x;y\n");
    file->write("frame;time;visible;x;y\n");

}

void writeCsvData(QFile* file, int frame_idx, QString time, bool visible, double x, double y){

	//int mid_y = video_widget.height() / 2;
	//QRectF bounds(0, mid_y - display_height / 2, display_width, display_height);
	//float x = (state.x - bounds.x()) / bounds.width();
	//float y = (state.y - bounds.y()) / bounds.height();

	file->write(QString("%1;%2;%3;%4;%5\n")
                       .arg(frame_index++).arg(time).arg(visible).arg(x).arg(y)
                       .toStdString().c_str());
}

int main(int argc, char *argv[])
{

    QString vid_path = QString("");
    if ( argc != 2 ) // argc should be 2 for correct execution
        qDebug() << "usage: "<< argv[0] <<" <filename>\n";
    else {
        // We assume argv[1] is a filename to open
        vid_path = argv[1];
    }

    qDebug() << "Opening video: " << vid_path;

    QGuiApplication a(argc, argv);

    qDebug() << "Number of screens:" << QGuiApplication::screens().size();

    qDebug() << "Primary screen:" << QGuiApplication::primaryScreen()->name();

    QScreen *screen =  QGuiApplication::screens()[0];
    int screen_width = screen->size().width();
    int screen_height = screen->size().height();
    qDebug() << "  Size:" << screen->size().width() << "x" << screen->size().height();


    cv::VideoCapture cap(vid_path.toStdString()); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cv::Mat frame;
    cap >> frame; // get a new frame from camera
    int vid_width = frame.size().width;
    int vid_height = frame.size().height;

    int vid_ratio = vid_width/vid_height;
    int screen_ratio = screen_width/screen_height;

    int display_width = 0;
    int display_height = 0;
    if(vid_ratio < screen_ratio){
        display_height = screen_height;
        display_width = vid_width*display_height/vid_height;
    }
    else{
        display_width = screen_width;
        display_height = vid_height*display_width/vid_width;
    }

    cv::Size display_size = cv::Size(display_width,display_height);

    qDebug() << "Video size:" << vid_width << "x" << vid_height;
    qDebug() << "Display size:" << display_size.width << "x" << display_size.height;



    QFileInfo file_info = QFileInfo(vid_path);
    QString data_path = QString(file_info.dir().path() + "/" + file_info.completeBaseName() + ".csv");
    qDebug() << "Opening csv file: " << data_path;

    QFile* tracking_data = new QFile(data_path);
    tracking_data->open(QFile::WriteOnly | QFile::Text);
    writeCsvHeader(tracking_data,vid_width,vid_height,display_width,display_height);

    QString overlay_msg = QString("Press spacebar to record gaze position");
    cv::Point msg_pos = cv::Point(0,display_height*0.1);
    cv::Mat edges;
    cv::namedWindow("video",CV_WINDOW_NORMAL);
    cv::setWindowProperty("video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    bool space_on;
    for(;;)
    {
        cv::Mat frame;
        cv::Mat resized_frame;
        cap >> frame; // get a new frame from camera
        cv::resize(frame,resized_frame,display_size);
        char key = (char) cv::waitKey(30);
        if(key == 32){
          //qDebug() << "Pressed spacebar";
          space_on = !space_on;
        }
        if(space_on)
          cv::putText(resized_frame, overlay_msg.toStdString(), msg_pos, cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0,true);
        else{
          cv::putText(resized_frame, overlay_msg.toStdString(), msg_pos, cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0,true);
        }
        cv::imshow("video", resized_frame);

        if(key == 27){
          qDebug() << "Pressed escape...Quitting";
          break;
        }
    }
    qDebug() << "Closing csv file: " << data_path;
    tracking_data->close();
    return 0;
}
