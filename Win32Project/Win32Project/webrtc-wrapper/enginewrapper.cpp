#include "stdafx.h"

#include "enginewrapper.h"

namespace demo {

bool EngineWrapper::StartEngine(HWND hWnd) {
	capture_ = demo::VideoCapture::Create();
	if (!capture_) return false;
	render_ = new demo::VideoRenderer(hWnd);
	capture_->AddOrUpdateSink(render_);
	capture_->Start();
	return true;
}

void EngineWrapper::StopEngine() {
	capture_->Stop();
	delete capture_;
	delete render_;
}
}