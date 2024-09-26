#include "zepwrapper.h"

Zep::NVec2f GetPixelScale() {
	float ddpi = 0.0f;
	float hdpi = 0.0f;
	float vdpi = 0.0f;

	auto res = ImGui::GetIO().DisplayFramebufferScale;
	if (res.x != 0 && res.y != 0) {
		hdpi = res.x;
		vdpi = res.y;
	}

	return Zep::NVec2f(hdpi, vdpi);
}
