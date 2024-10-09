#define GLFW_INCLUDE_NONE

#include "jenjin/editor/utils.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <cstdlib>

void Jenjin::Editor::ensure_dir(std::string path) {
  if (!std::filesystem::exists(path)) {
    spdlog::debug("Creating directory: {}", path);
    std::filesystem::create_directories(path);
  } else {
    spdlog::debug("Directory exists: {}", path);
  }
}

std::string Jenjin::Editor::get_jendir() {
  std::string jendir = "";
#ifdef _WIN32
  jendir = std::getenv("USERPROFILE") + std::string("\\Documents\\Jenjin");
#elif __APPLE__ || __linux__
  jendir = std::getenv("HOME") + std::string("/Documents/Jenjin");
#else
  jendir = "Jendir";
#endif

  return jendir;
}
