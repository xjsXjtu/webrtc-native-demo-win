#pragma once

#include "video_capture.h"
#include "video_render.h"

namespace demo {
  class EngineWrapper {
  public:
    EngineWrapper():capture_(NULL), render_(NULL) {}
    ~EngineWrapper() {
      StopEngine();
    }

	bool StartEngine(HWND hWnd);

	void StopEngine();

    VideoRenderer *GetVideoRender() const {
      return render_;
    }

  private:
    VideoCapture *capture_;
    VideoRenderer *render_;

  };
  
}
