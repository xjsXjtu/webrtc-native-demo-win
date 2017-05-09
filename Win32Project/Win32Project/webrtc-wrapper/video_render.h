#pragma once

#include "stdafx.h"
#include "webrtc/media/base/videosinkinterface.h"
#include "webrtc/api/video/video_frame.h"
#include "webrtc/base/criticalsection.h"

namespace demo {
  class VideoRenderer : public rtc::VideoSinkInterface<webrtc::VideoFrame> {
  public:
    VideoRenderer(HWND wnd);
    virtual ~VideoRenderer();

    // VideoSinkInterface implementation
    void OnFrame(const webrtc::VideoFrame& frame) override;

    const BITMAPINFO& bmi() const { return bmi_; }
    const uint8_t* image() const { return image_.get(); }

  protected:
    void SetSize(int width, int height);

    HWND wnd_;
    BITMAPINFO bmi_;
    std::unique_ptr<uint8_t[]> image_;
    rtc::CriticalSection buffer_lock_;
  };
}