/*
 * window.cpp
 *
 *  Created on: 26.11.2015
 *      Author: scones
 */


#include <iostream>
#include <system_error>
#include <algorithm>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "core/window.h"
#include "core/glfw.h"
#include "core/file.h"
#include "core/ui/widget.h"
#include "core/ui/container/container.h"

#include "core/vao.h"
#include "core/buffer.h"
#include "core/shader.h"

#include "core/event/event_handler.h"


void global_key_callback(GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods);
void global_window_resize_callback(GLFWwindow* pwindow, std::int32_t width, std::int32_t height);
void global_window_reposition_callback(GLFWwindow* pwindow, std::int32_t xpos, std::int32_t ypos);
void global_mouse_button_callback(GLFWwindow* pwindow, std::int32_t button, std::int32_t action, std::int32_t modifiers);
void global_cursor_position_callback(GLFWwindow* pwindow, double xpos, double ypos);
void global_scroll_callback(GLFWwindow* pwindow, double xoffset, double yoffset);


core::event::event_handler* event_handler = nullptr;

namespace {

  namespace BUFFER {
    enum {
      POSITION = 0,
      COLOR,
      MAX
    };
  }

  namespace ATTRIBUTES {
    enum {
      POSITION = 0,
      COLOR,
      MAX
    };
  }

  namespace BINDINGS {
    enum {
      PRIMARY = 0,
      SECONDARY,
      MAX
    };
  }

  namespace EVENTS {
    enum {
      MOUSE_BUTTON,
      MOUSE_MOVE,
      MOUSE_SCROLL,
      KEYBOARD_BUTTON
    };
  }

  const std::uint32_t WINDOW_SIZE_X = 500;
  const std::uint32_t WINDOW_SIZE_Y = 500;
}


void glfw_error_callback(std::int32_t error, char const* description) {
  char buffer[1024];
  std::snprintf(buffer, 1024, "GLFW error (%d): %s", error, description);
  throw core::error::error(buffer);
}


int main(int argc, char** argv) {

  event_handler = new core::event::event_handler;

  // initialize basic like glfw a window and glew
  core::glfw glfw;
  glfwSetErrorCallback(&glfw_error_callback);
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
  window.make_current();

  // parse the widgets from the json
  auto json = core::file::content("data/script/window_example/main_window.json");
  core::ui::container* x = core::ui::container::parse_config(json);

  auto widgets = x->get_all_widgets();
  std::sort(widgets.begin(), widgets.end(), core::ui::widget::compare_back_to_front);
  std::vector<core::ui::widget::fvec3> vertices;
  std::vector<core::ui::widget::fvec4> colors;
  for (auto widget : widgets) {
    auto widget_vertices = widget->get_coordinates_ccw_3d();
    auto color = widget->get_current_color();
    vertices.insert(vertices.end(), widget_vertices.begin(), widget_vertices.end());
    for (std::uint32_t i(0); i < widget_vertices.size(); ++i)
      colors.push_back(color);

    event_handler->bind(EVENTS::MOUSE_BUTTON, widget);
    event_handler->bind(EVENTS::MOUSE_MOVE, widget);
    event_handler->bind(EVENTS::MOUSE_SCROLL, widget);
  }

  // get all necessary objects
  core::vao vao;
  core::shader shader({
    450,
    {
      {GL_VERTEX_SHADER, "widget"},
      {GL_FRAGMENT_SHADER, "widget"}
    },
    {"world_matrix"}
  });
  core::buffer buffer(BUFFER::MAX);

  // initialize the objects
  shader.init();
  vao.init();
  {
    auto v = vao.activate();
    buffer.init();
    buffer.data(ATTRIBUTES::POSITION, sizeof(core::ui::widget::fvec3) * vertices.size(), nullptr, GL_STATIC_DRAW);
    buffer.data(ATTRIBUTES::COLOR, sizeof(core::ui::widget::fvec4) * vertices.size(), nullptr, GL_STATIC_DRAW);

    // fill the buffers
    void* pmemory = buffer.map(ATTRIBUTES::POSITION, GL_WRITE_ONLY);
    std::memcpy(pmemory, vertices.data(), sizeof(core::ui::widget::fvec3) * vertices.size());
    buffer.unmap(ATTRIBUTES::POSITION);
    pmemory = buffer.map(ATTRIBUTES::COLOR, GL_WRITE_ONLY);
    std::memcpy(pmemory, colors.data(), sizeof(core::ui::widget::fvec4) * colors.size());
    buffer.unmap(ATTRIBUTES::COLOR);

    // init vao with format
    glVertexAttribBinding(ATTRIBUTES::POSITION, BINDINGS::PRIMARY);
    glVertexAttribFormat(ATTRIBUTES::POSITION, 3, GL_FLOAT, GL_FALSE, 0);

    glVertexAttribBinding(ATTRIBUTES::COLOR, BINDINGS::SECONDARY);
    glVertexAttribFormat(ATTRIBUTES::COLOR, 4, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexAttribArray(ATTRIBUTES::POSITION);
    glEnableVertexAttribArray(ATTRIBUTES::COLOR);
    glBindVertexBuffer(BINDINGS::PRIMARY, buffer.get_name(BUFFER::POSITION), 0, sizeof(core::ui::widget::fvec3));
    glBindVertexBuffer(BINDINGS::SECONDARY, buffer.get_name(BUFFER::COLOR), 0, sizeof(core::ui::widget::fvec4));
  }
  auto world_matrix_id = shader.get_uniform_location("world_matrix");
  glm::highp_mat4x4 world_matrix(glm::ortho<float>(0, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0, -1.0f, 10000.0f));
  glProgramUniformMatrix4fv(shader.get_program_name(), world_matrix_id, 1, GL_FALSE, &world_matrix[0][0]);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0.1, 0.1, 0.1, 0.0);
  do {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto s = shader.activate();
    auto v = vao.activate();

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    window.swap_buffers();
  } while (!window.should_close());
  return 1;
}


void global_key_callback(GLFWwindow* window, std::int32_t key, std::int32_t scancode, std::int32_t action, std::int32_t mods) {

}

void global_window_resize_callback(GLFWwindow* pwindow, std::int32_t width, std::int32_t height) {

}

void global_window_reposition_callback(GLFWwindow* pwindow, std::int32_t xpos, std::int32_t ypos) {

}

void global_mouse_button_callback(GLFWwindow* pwindow, std::int32_t button, std::int32_t action, std::int32_t modifiers) {

}

void global_cursor_position_callback(GLFWwindow* pwindow, double xpos, double ypos) {

}

void global_scroll_callback(GLFWwindow* pwindow, double xoffset, double yoffset) {

}


