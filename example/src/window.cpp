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

  const std::uint32_t WINDOW_SIZE_X = 500;
  const std::uint32_t WINDOW_SIZE_Y = 500;
}


void glfw_error_callback(std::int32_t error, char const* description) {
  char buffer[1024];
  std::snprintf(buffer, 1024, "GLFW error (%d): %s", error, description);
  throw core::error::error(buffer);
}


int main(int argc, char** argv) {

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
  std::sort(widgets.begin(), widgets.end(), core::ui::widget::compare_front_to_back);

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
  std::vector<core::buffer*> buffers;
  std::vector<core::vao*> vaos;

  // initialize the objects
  shader.init();
  {
    for (std::uint32_t i(0); i < widgets.size(); ++i) {
      core::vao* pvao = new core::vao;
      pvao->init();
      auto v = pvao->activate();
      vaos.push_back(pvao);
      // build buffer
      auto pbuffer = new core::buffer(BUFFER::MAX);
      pbuffer->init();

      // fill buffer
      auto vertices = widgets[i]->get_coordinates_ccw_3d();
      auto color = widgets[i]->get_current_color();
      std::vector<core::ui::widget::fvec4> color_data;
      color_data.resize(vertices.size(), color);
      pbuffer->data(ATTRIBUTES::POSITION, sizeof(core::ui::widget::fvec3) * vertices.size(), nullptr, GL_STATIC_DRAW);
      pbuffer->data(ATTRIBUTES::COLOR, sizeof(core::ui::widget::fvec4) * vertices.size(), nullptr, GL_STATIC_DRAW);
      void* pmemory = pbuffer->map(ATTRIBUTES::POSITION, GL_WRITE_ONLY);
      std::memcpy(pmemory, vertices.data(), sizeof(core::ui::widget::fvec3) * vertices.size());
      pbuffer->unmap(ATTRIBUTES::POSITION);
      pmemory = pbuffer->map(ATTRIBUTES::COLOR, GL_WRITE_ONLY);
      std::memcpy(pmemory, color_data.data(), sizeof(color) * color_data.size());
      pbuffer->unmap(ATTRIBUTES::COLOR);

      // init vao with format
      glVertexAttribBinding(ATTRIBUTES::POSITION, BINDINGS::PRIMARY);
      glVertexAttribFormat(ATTRIBUTES::POSITION, 3, GL_FLOAT, GL_FALSE, 0);

      glVertexAttribBinding(ATTRIBUTES::COLOR, BINDINGS::SECONDARY);
      glVertexAttribFormat(ATTRIBUTES::COLOR, 4, GL_FLOAT, GL_FALSE, 0);

      glEnableVertexAttribArray(ATTRIBUTES::POSITION);
      glEnableVertexAttribArray(ATTRIBUTES::COLOR);
      glBindVertexBuffer(BINDINGS::PRIMARY, pbuffer->get_name(BUFFER::POSITION), 0, sizeof(core::ui::widget::fvec3));
      glBindVertexBuffer(BINDINGS::SECONDARY, pbuffer->get_name(BUFFER::COLOR), 0, sizeof(core::ui::widget::fvec4));

      // store buffer
      buffers.push_back(pbuffer);
    }
  }
  auto world_matrix_id = shader.get_uniform_location("world_matrix");
  glm::highp_mat4x4 world_matrix(glm::ortho<float>(0, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0, -1.0f, 10000.0f));
  glProgramUniformMatrix4fv(shader.get_program_name(), world_matrix_id, 1, GL_FALSE, &world_matrix[0][0]);

  glClearColor(0.1, 0.1, 0.1, 0.0);
  do {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto s = shader.activate();
    for (std::uint32_t i(0); i < vaos.size(); ++i) {
      auto v = vaos[i]->activate();
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    window.swap_buffers();
  } while (!window.should_close());
  return 1;
}

