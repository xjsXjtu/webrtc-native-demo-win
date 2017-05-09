#pragma once

#include "webrtc/media/engine/webrtcvideocapturer.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/api/video/video_frame.h"

namespace demo{

static const cricket::VideoFormat kDefaultFormat = {
  640, 480, FPS_TO_INTERVAL(30), cricket::FOURCC_ANY };

class VideoCapture : public sigslot::has_slots<> {
public:
  static VideoCapture* Create();

  void AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink);

  bool Start();

  void Stop();

private:
  VideoCapture(cricket::VideoCapturer *capture) :capture_(capture) {}
  // from VideoCapturerTrackSource, but do nothing here
  void OnStateChange(cricket::VideoCapturer* capturer, cricket::CaptureState capture_state) {}

  cricket::VideoCapturer *capture_;
  std::vector<rtc::VideoSinkInterface<webrtc::VideoFrame>*> sinks_;
};
}