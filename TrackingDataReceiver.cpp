#include "TrackingDataReceiver.h"

TrackingDataReceiver::TrackingDataReceiver()
{
    std::memset(&state, 0, sizeof(state));
}

TrackingDataReceiver::~TrackingDataReceiver()
{

}

void TrackingDataReceiver::connect()
{
    if(receiver.connect(true))
    {
        receiver.add_listener(*this);
    }
}

void TrackingDataReceiver::disconnect()
{
    receiver.remove_listener(*this);
    receiver.disconnect();
}

void TrackingDataReceiver::on_gaze_data(const gtl::GazeData &data)
{
    if(data.state & gtl::GazeData::GD_STATE_TRACKING_GAZE)
    {
        TrackingData update;
        update.timestamp = data.time;
        update.x = data.avg.x;
        update.y = data.avg.y;
        update.left.x = data.lefteye.avg.x;
        update.left.y = data.lefteye.avg.x;
        update.left.xRaw = data.lefteye.raw.x;
        update.left.yRaw = data.lefteye.raw.x;
        update.right.x = data.righteye.avg.x;
        update.right.y = data.righteye.avg.y;
        update.right.xRaw = data.righteye.raw.x;
        update.right.yRaw = data.righteye.raw.x;
        update.isFixated = data.fix;

        boost::lock_guard<boost::mutex> lock(stateMutex);
        state = update;
    }
}

TrackingData TrackingDataReceiver::getState()
{
    boost::lock_guard<boost::mutex> lock(stateMutex);
    return state;
}

bool TrackingDataReceiver::isConnected()
{
    return receiver.get_server_state().trackerstate == gtl::ServerState::TRACKER_CONNECTED;
}
