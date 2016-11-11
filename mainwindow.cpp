#include <mainwindow.h>

MainWindow::MainWindow()
{
    tracking_data = nullptr;
    frame_idx = 0;
    target_visible = false;

    // Get the initial timestamp for the tracker.
    // We use this to synchronize the video with
    // the tracker.
    //tracker.connect();
}

MainWindow::~MainWindow()
{}

void MainWindow::writeCsvHeader(QFile* file, int vid_width, int vid_height, int disp_width, int disp_height){

    file->write(QString("VideoWidth:%1\n").arg(vid_width).toStdString().c_str());
    file->write(QString("VideoHeight:%1\n").arg(vid_height).toStdString().c_str());
    file->write(QString("DisplayWidth:%1\n").arg(disp_width).toStdString().c_str());
    file->write(QString("DisplayHeight:%1\n").arg(disp_height).toStdString().c_str());

    //change when tracker is added!
    //trackerInitialTimestamp = state.timestamp;
    //tracking_data->write("frame;time;visible;x;y\n");
    file->write("frame;time;visible;x;y\n");
}

void MainWindow::writeCsvData(QFile* file, int frame_idx, QString time, bool visible, double x, double y){

    //int mid_y = video_widget.height() / 2;
    //QRectF bounds(0, mid_y - display_height / 2, display_width, display_height);
    //float x = (state.x - bounds.x()) / bounds.width();
    //float y = (state.y - bounds.y()) / bounds.height();

    file->write(QString("%1;%2;%3;%4;%5\n")
                       .arg(frame_idx++).arg(time).arg(visible).arg(x).arg(y)
                       .toStdString().c_str());
}

void MainWindow::run(QString vid_path, int fps, QScreen* screen)
{


    qDebug() << "Connecting to tracker";
    tracker.connect();
    qDebug() << "Tracker is connected: " << tracker.isConnected();

    int screen_width = screen->size().width();
    int screen_height = screen->size().height();
    qDebug() << "  Size:" << screen->size().width() << "x" << screen->size().height();


    qDebug() << "Opening video: " << vid_path;
    cv::VideoCapture cap(vid_path.toStdString()); // open the default camera
    //if(!cap.isOpened())  // check if we succeeded
     //   return -1;

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

    QString overlay_msg = QString("Press spacebar to mark gaze position");
    cv::Point msg_pos = cv::Point(0,display_height*0.1);
    cv::Mat edges;
    cv::namedWindow("video",CV_WINDOW_NORMAL);
    cv::setWindowProperty("video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    TrackingData state = tracker.getState();
    bool visible = true;
        cv::Mat resized_frame;
        //cap.release();
        //cv::VideoCapture cap(vid_path.toStdString()); // open the default camera
        cap.set(CV_CAP_PROP_POS_FRAMES,0); //Rewind
        cap.set(CV_CAP_PROP_FPS,10); //Rewind
        int frameCnt = cap.get(CV_CAP_PROP_FRAME_COUNT);
        qDebug() << "Frame count: " << frameCnt;
        qDebug() << "Fps: " << fps;

    while(true)
    {
        cap >> frame; // get a new frame from camera
        if(frame.empty()) break;
        cv::resize(frame,resized_frame,display_size);
        char key = (char) cv::waitKey(1000/fps);
        if(key == 32){
          qDebug() << "Pressed spacebar";
          visible= !visible;
        }
        if(visible)
          cv::putText(resized_frame, overlay_msg.toStdString(), msg_pos, cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0,true);
        else{
          cv::putText(resized_frame, overlay_msg.toStdString(), msg_pos, cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0,true);
        }

        state = tracker.getState();
        int mid_y = display_height / 2;
        QRectF bounds(0, mid_y - display_height / 2, display_width, display_height);
        float x = (state.x - bounds.x()) / bounds.width();
        float y = (state.y - bounds.y()) / bounds.height();
        writeCsvData(tracking_data, frame_idx++, 0, visible, x, y);
        qDebug() << "Gaze position: (x,y) = (" << x << "," << y << ")";

        //cv::circle(resized_frame, cv::Point(state.x - bounds.width(),state.y-bounds.height()),10, cv::Scalar(0,255,0),2, 8,0);
        cv::circle(resized_frame, cv::Point(state.x ,state.y),10, cv::Scalar(0,255,0),2, 8,0);
        cv::imshow("video", resized_frame);
    }
    qDebug() << "Closing csv file: " << data_path;
    tracking_data->close();
    qDebug() << "Closing window";
    cv::destroyWindow("video");
    tracker.disconnect();
}
