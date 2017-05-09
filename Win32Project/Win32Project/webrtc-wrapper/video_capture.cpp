#include "stdafx.h"

#include "video_capture.h"

namespace demo {
//static
VideoCapture* VideoCapture::Create() {
	std::vector<std::string> device_names;
	{
		std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
			webrtc::VideoCaptureFactory::CreateDeviceInfo());
		if (!info) {
			return nullptr;
		}
		int num_devices = info->NumberOfDevices();
		for (int i = 0; i < num_devices; ++i) {
			const uint32_t kSize = 256;
			char name[kSize] = { 0 };
			char id[kSize] = { 0 };
			if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
				device_names.push_back(name);
			}
		}
	}

	cricket::WebRtcVideoDeviceCapturerFactory factory;
	cricket::VideoCapturer* capturer = nullptr;
	for (const auto& name : device_names) {
		capturer = factory.Create(cricket::Device(name, 0));
		if (capturer) {
			break;
		}
	}
	if (!capturer) return nullptr;
	return new VideoCapture(capturer);
}

void VideoCapture::AddOrUpdateSink(rtc::VideoSinkInterface<webrtc::VideoFrame>* sink) {
	capture_->AddOrUpdateSink(sink, rtc::VideoSinkWants());
}

bool VideoCapture::Start() {
	capture_->SignalStateChange.connect(this, &VideoCapture::OnStateChange);
	cricket::VideoFormat format;
	if (!capture_->GetBestCaptureFormat(kDefaultFormat, &format)) {
		return false;
	}
	if (!capture_->StartCapturing(format)) {
		return false;
	}
	return true;
}

void VideoCapture::Stop() {
	for (auto sink = sinks_.begin(); sink != sinks_.end(); sink++) {
		capture_->RemoveSink(*sink);
	}
	capture_->Stop();
}
}