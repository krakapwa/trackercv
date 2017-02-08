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

void MainWindow::writeCsvData(QFile* file, int frame_idx, int time, bool visible, double x, double y){

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

    float vid_ratio = (float)vid_width/(float)vid_height;
    float screen_ratio = (float)screen_width/(float)screen_height;
    qDebug() << "Video ratio: " << vid_ratio;
    qDebug() << "Screen ratio: " << screen_ratio;

    int display_width = 0;
    int display_height = 0;
        float top_left_corner_x;
        float top_left_corner_y;
    if(vid_ratio < screen_ratio){
            qDebug() << "Video ratio < screen_ratio";

        display_height = screen_height;
        display_width = vid_width*display_height/vid_height;
        top_left_corner_x = (screen_width-display_width)/2;
        top_left_corner_y = 0;
    }
    else{
            qDebug() << "Video ratio > screen_ratio";
        display_width = screen_width;
        //display_height = vid_height*screen_height/vid_width;
        display_height = vid_height*screen_height/vid_width;
        top_left_corner_y = (screen_height-display_height)/2;
        top_left_corner_x = 0;
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
    int time = 0;
    bool visible = true;
        cv::Mat resized_frame;
        //cap.release();
        //cv::VideoCapture cap(vid_path.toStdString()); // open the default camera
        cap.set(CV_CAP_PROP_POS_FRAMES,0); //Rewind
        cap.set(CV_CAP_PROP_FPS,10); //Rewind
        int frameCnt = cap.get(CV_CAP_PROP_FRAME_COUNT);
        qDebug() << "Frame count: " << frameCnt;
        qDebug() << "Fps: " << fps;

        qDebug() << "top_left_corner_x = " << top_left_corner_x;
        qDebug() << "top_left_corner_y = " << top_left_corner_y;
        float x = (1537 - top_left_corner_x) ;
        float y = (0 - top_left_corner_y) ;
        qDebug() << "(x0,y0) = (" << x << "," << y << ")";

    cap >> frame; // get a new frame from camera
    bool playing = false;
    while(true)
    {
        char key = (char) cv::waitKey(1000/fps);
        if(key == 32){
          qDebug() << "Pressed spacebar";
          playing = true;
        }
        if(playing){
                cap >> frame; // get a new frame from camera
                if(frame.empty()) break;
        }
        cv::resize(frame,resized_frame,display_size);
        //if(visible)
        //  cv::putText(resized_frame, overlay_msg.toStdString(), msg_pos, cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0,true);
        //else{
        //  cv::putText(resized_frame, overlay_msg.toStdString(), msg_pos, cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0,true);
        //}

        state = tracker.getState();
        float x_norm = (state.x) / display_width;
        float y_norm = (state.y) / display_height;
        if(playing){
                writeCsvData(tracking_data, frame_idx++, time, visible, x_norm, y_norm);
                qDebug() << "Gaze position: (x,y) = (" << x_norm << "," << y_norm << ")";
        }

        //cv::Point this_point = cv::Point(x_to_draw,y_to_draw);
        cv::Point this_point = cv::Point(state.x - top_left_corner_x,state.y - top_left_corner_y);
        //cv::circle(resized_frame, cv::Point(state.x - bounds.width(),state.y-bounds.height()),10, cv::Scalar(0,255,0),2, 8,0);
        cv::circle(resized_frame, this_point,10, cv::Scalar(0,255,0),2, 8,0);
        cv::imshow("video", resized_frame);
        playing = false;
    }
    qDebug() << "Closing csv file: " << data_path;
    tracking_data->close();
    qDebug() << "Closing window";
    cv::destroyWindow("video");
    tracker.disconnect();
}
