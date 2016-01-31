/*
 * window.cpp
 *
 *  Created on: 16.08.2013
 *      Author: scn
 */


#include "core/window.h"
#include "core/error/opengl_error.h"

//#include <stdexcept>
#include <iostream>
#include <gl/glew.h>


namespace core {

  window::t_hint_map window::s_defaults({
    { GLFW_SAMPLES,                0 },
    { GLFW_CONTEXT_VERSION_MAJOR,  4 },
    { GLFW_CONTEXT_VERSION_MINOR,  3 },
    { GLFW_OPENGL_PROFILE,         GLFW_OPENGL_CORE_PROFILE },
    { GLFW_RESIZABLE,              GL_TRUE },
    { GLFW_FOCUSED,                GL_FALSE },
    { GLFW_DECORATED,              GL_TRUE },
#ifdef DEBUG
    { GLFW_OPENGL_DEBUG_CONTEXT,   GL_TRUE},
#endif
    { GLFW_AUTO_ICONIFY,           GL_FALSE }
  });


  window::window(std::string const & title, t_hint_map hint_map, GLFWwindow* parent, GLFWmonitor* monitor)
    :
      m_handle(nullptr),
      m_parent(parent),
      m_monitor(monitor),
      m_title(title),
      m_resolution(),
      m_position(),
      m_fullscreen(false),
      m_hint_map(s_defaults)
  {
    for (auto const& hint : hint_map)
      m_hint_map[hint.first] = hint.second;
  }


  void window::init() {

    auto previous_context = glfwGetCurrentContext();

    for (auto const& hint : m_hint_map)
      glfwWindowHint(hint.first, hint.second);

    m_handle = glfwCreateWindow(m_resolution.x, m_resolution.y, m_title.c_str(), m_monitor, NULL);
    if (NULL == m_handle) {
      this->shutdown();
      throw std::runtime_error("window could not be initialised");
    }

    glfwMakeContextCurrent(m_handle);

    glewExperimental = GL_TRUE;
    auto glew_err = glewInit();
    if (glew_err != GLEW_OK) {
      this->shutdown();
      throw std::runtime_error("glew could not be initialised");
    }

    while (GL_NO_ERROR != glGetError());
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glDebugMessageCallback(&core::error::opengl_error::callback, nullptr);
//    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, -1, "start initialization");

    // check extensions
    if (!GL_AMD_gpu_shader_int64) {
      throw std::runtime_error("extension not supported: AMD_gpu_shader_int64");
    }

    if (!GL_ARB_gpu_shader_fp64) {
      throw std::runtime_error("extension_not_supported: GL_ARB_gpu_shader_fp64");
    }

    glfwMakeContextCurrent(previous_context);
  }


  void window::shutdown() {
    if (m_handle == glfwGetCurrentContext()) {
      glfwMakeContextCurrent(0);
    }
    glfwDestroyWindow(m_handle);
  }


  void window::make_current() {
    glfwMakeContextCurrent(m_handle);
  }


  bool window::should_close() {
    return glfwWindowShouldClose(m_handle);
  }


  void window::swap_buffers() {
    glfwSwapBuffers(m_handle);
  }


  void window::set_key_callback(GLFWkeyfun key_callback_function) {
    glfwSetKeyCallback(m_handle, key_callback_function);
  }


  void window::set_resize_callback(GLFWwindowsizefun resize_callback_function) {
    glfwSetWindowSizeCallback(m_handle, resize_callback_function);
  }


  void window::set_reposition_callback(GLFWwindowposfun reposition_callback_function) {
    glfwSetWindowPosCallback(m_handle, reposition_callback_function);
  }


  void window::set_mouse_button_callback(GLFWmousebuttonfun mouse_button_callback) {
    glfwSetMouseButtonCallback(m_handle, mouse_button_callback);
  }


  void window::set_cursor_position_callback(GLFWcursorposfun cursor_position_callback) {
    glfwSetCursorPosCallback(m_handle, cursor_position_callback);
  }


  void window::set_scroll_callback(GLFWscrollfun scroll_callback) {
    glfwSetScrollCallback(m_handle, scroll_callback);
  }


  void window::set_window_hints(window::t_hint_map const& hints) {
    for (auto const& hint : hints)
      m_hint_map[hint.first] = hint.second;
  }


  void window::set_fullscreen(bool fullscreen) {
    m_fullscreen = fullscreen;
    if (!m_monitor)
      m_monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

    m_hint_map[GLFW_RED_BITS]     = mode->redBits;
    m_hint_map[GLFW_GREEN_BITS]   = mode->greenBits;
    m_hint_map[GLFW_BLUE_BITS]    = mode->blueBits;
    m_hint_map[GLFW_REFRESH_RATE] = mode->refreshRate;

    if (!m_fullscreen)
      m_monitor = nullptr;
  }


  core::t_vec2d const window::get_cursor_position() const {
    core::t_vec2d result;
    glfwGetCursorPos(m_handle, &result.x, &result.y);
    return result;
  }


  void window::set_input_mode(std::uint32_t input_mode) {
    glfwSetInputMode(m_handle, GLFW_CURSOR, input_mode);
  }

}

