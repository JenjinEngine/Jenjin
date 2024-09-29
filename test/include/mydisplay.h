#pragma once

#include "jenjin/target.h"

class MyDisplay : public Jenjin::Target {
public:
	MyDisplay() = default;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual glm::vec2 GetSize() override;
	virtual void Resize(glm::vec2 size) override;

	virtual glm::vec2 GetMousePosition() override;
};
