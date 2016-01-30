/*
 * window.cpp
 *
 *  Created on: 26.11.2015
 *      Author: scones
 */


#include <iostream>
#include <system_error>


#include "core/window.h"
#include "core/glfw.h"
#include "core/file.h"
#include "core/ui/widget.h"


int main(int argc, char** argv) {

  core::glfw glfw;
  glfw.init();

  core::window window("window example");
  window.set_position({20, 20});
  window.set_resolution({500, 500});
  window.set_fullscreen(false);
  window.set_window_hints({
    {GLFW_SAMPLES, 4},
    {GLFW_CONTEXT_VERSION_MAJOR, 4},
    {GLFW_CONTEXT_VERSION_MINOR, 5}
  });
  window.init();

  std::string foo = core::file::content("data/script/window_example/main_window.json");

  try {
    core::ui::widget* x = core::ui::widget::parse_config(foo);
  } catch (std::system_error const&) {
    std::cout << "system_error" << std::endl;
  }

  do {
    glfwPollEvents();

    window.swap_buffers();
  } while (!window.should_close());
  return 1;
}

