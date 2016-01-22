/*
 * window.cpp
 *
 *  Created on: 26.11.2015
 *      Author: scones
 */


#include "core/window.h"
#include "core/glfw.h"


int main(void) {

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

  do {
    glfwPollEvents();

    window.swap_buffers();
  } while (!window.should_close());
  return 1;
}

