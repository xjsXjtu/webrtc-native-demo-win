#include "stdafx.h"
#include "video_render.h"
#include "webrtc/api/video/i420_buffer.h"
#include "webrtc/base/checks.h"
#include "third_party/libyuv/include/libyuv.h"

namespace demo {
  

  VideoRenderer::VideoRenderer(
    HWND wnd)
    : wnd_(wnd) {
    ZeroMemory(&bmi_, sizeof(bmi_));
    bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi_.bmiHeader.biPlanes = 1;
    bmi_.bmiHeader.biBitCount = 32;
    bmi_.bmiHeader.biCompression = BI_RGB;
    bmi_.bmiHeader.biWidth = 0;
    bmi_.bmiHeader.biHeight = 0;
    bmi_.bmiHeader.biSizeImage = 0;
  }

  VideoRenderer::~VideoRenderer() {
  }

  void VideoRenderer::SetSize(int width, int height) {
    rtc::CritScope lock(&buffer_lock_);

    if (width == bmi_.bmiHeader.biWidth && height == bmi_.bmiHeader.biHeight) {
      return;
    }

    bmi_.bmiHeader.biWidth = width;
    bmi_.bmiHeader.biHeight = height;
    bmi_.bmiHeader.biSizeImage = width * height *
      (bmi_.bmiHeader.biBitCount >> 3);
    image_.reset(new uint8_t[bmi_.bmiHeader.biSizeImage]);
  }

  void VideoRenderer::OnFrame(
    const webrtc::VideoFrame& video_frame) {
      {
        rtc::CritScope lock(&buffer_lock_);

        rtc::scoped_refptr<webrtc::VideoFrameBuffer> buffer(
          video_frame.video_frame_buffer());
        if (video_frame.rotation() != webrtc::kVideoRotation_0) {
          buffer = webrtc::I420Buffer::Rotate(*buffer, video_frame.rotation());
        }

        SetSize(buffer->width(), buffer->height());

        RTC_DCHECK(image_.get() != NULL);
        libyuv::I420ToARGB(buffer->DataY(), buffer->StrideY(),
          buffer->DataU(), buffer->StrideU(),
          buffer->DataV(), buffer->StrideV(),
          image_.get(),
          bmi_.bmiHeader.biWidth *
          bmi_.bmiHeader.biBitCount / 8,
          buffer->width(), buffer->height());
      }
      InvalidateRect(wnd_, NULL, TRUE);
  }
}