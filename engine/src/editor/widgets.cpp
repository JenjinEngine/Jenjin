#include "jenjin/editor/widgets.h"
#include "jenjin/gameobject.h"

#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_internal.h>

using namespace Jenjin::Editor;

static void drawButtonWithDrag(const std::string& buttonLabel, const std::string& dragLabel, float& value, const ImVec4& buttonColor, const ImVec4& buttonHoveredColor, const ImVec4& buttonActiveColor, const ImVec2& buttonSize, int defaultValue = 0) {
	ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActiveColor);

	if (ImGui::Button(buttonLabel.c_str(), buttonSize))
		value = defaultValue;

	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat(dragLabel.c_str(), &value, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
}

static void start_widget(const std::string& label, int count = 2) {
	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 100.0f);
	ImGui::Text("%s", label.c_str());
	ImGui::NextColumn();

	// HACK: This is a hack to make the drag float buttons all the same size... this needs to be revisited
	ImGui::PushMultiItemsWidths(count, ImGui::CalcItemWidth() + (count == 1 ? ImGui::GetStyle().ItemSpacing.x * 2 + ImGui::GetStyle().FramePadding.x + 1 : 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
}

static void end_widget() {
	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
}

bool Widgets::transformWidget(Jenjin::GameObject::Transform* transform) {
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	start_widget("Position");
	drawButtonWithDrag("X", "##X", transform->position.x,
										ImVec4{ 0.6f, 0.2f, 0.2f, 1.0f },
										ImVec4{ 0.7f, 0.3f, 0.3f, 1.0f },
										ImVec4{ 0.6f, 0.2f, 0.2f, 1.0f },
										buttonSize);

	drawButtonWithDrag("Y", "##Y", transform->position.y,
										ImVec4{ 0.3f, 0.6f, 0.3f, 1.0f },
										ImVec4{ 0.4f, 0.7f, 0.4f, 1.0f },
										ImVec4{ 0.3f, 0.6f, 0.3f, 1.0f },
										buttonSize);
	end_widget();

	start_widget("Scale");
	drawButtonWithDrag("W", "##ScaleW", transform->scale.x,
										ImVec4{ 0.6f, 0.2f, 0.2f, 1.0f },
										ImVec4{ 0.7f, 0.3f, 0.3f, 1.0f },
										ImVec4{ 0.6f, 0.2f, 0.2f, 1.0f },
										buttonSize, 1);

	drawButtonWithDrag("H", "##ScaleH", transform->scale.y,
										ImVec4{ 0.3f, 0.6f, 0.3f, 1.0f },
										ImVec4{ 0.4f, 0.7f, 0.4f, 1.0f },
										ImVec4{ 0.3f, 0.6f, 0.3f, 1.0f },
										buttonSize, 1);
	end_widget();

	start_widget("Rotation", 1);
	drawButtonWithDrag("Z", "##Rotation", transform->rotation,
										ImVec4{ 0.3f, 0.4f, 0.7f, 1.0f },
										ImVec4{ 0.4f, 0.5f, 0.8f, 1.0f },
										ImVec4{ 0.3f, 0.4f, 0.7f, 1.0f },
										buttonSize);
	end_widget();

	return false;
}
