#ifndef JENJIN_EDITOR_H
#define JENJIN_EDITOR_H

#include "scene.h"

#include <imgui.h>

#include <memory>
#include <string>

void header(std::string title);
void save(Jenjin::Scene* scene);
std::string getFilename(const std::string& fullPath);
void texture_picker(Jenjin::Scene* scene, std::shared_ptr<Jenjin::GameObject>& gobj);

void jenjin_explorer(Jenjin::Scene* scene);
void jenjin_menubar(Jenjin::Scene* scene);
int jenjin_dockspace();
void jenjin_viewport();

#endif
