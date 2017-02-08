#ifndef TRACKINGDATARECEIVER_H
#define TRACKINGDATARECEIVER_H

#include <cstdint>
#include "eyetribe/gazeapi.hpp"
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

struct EyeData
{
    float x, y;
    float xRaw, yRaw;
};

struct TrackingData
{
    EyeData left, right;
    float x, y;
    std::int64_t timestamp;
    bool isFixated;
};

class TrackingDataReceiver : public gtl::IGazeListener
{
    gtl::GazeApi receiver;
    TrackingData state;
    boost::mutex stateMutex;

private:
    void on_gaze_data(gtl::GazeData const & gaze_data);
public:
    TrackingDataReceiver();
    ~TrackingDataReceiver();

    void connect();
    void disconnect();
    bool isConnected();
    TrackingData getState();
    //virtual void on_gaze_data(gtl::GazeData const & gaze_data) override;
};

#endif
