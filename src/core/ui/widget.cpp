/*
 * widget.cpp
 *
 *  Created on: 22.11.2015
 *      Author: scones
 */


#include "core/ui/widget.h"
#include "core/event/event.h"
#include "core/error/duktape_error.h"
#include "core/error/json_error.h"
#include "core/support/duktape.h"

#include "core/ui/container/window.h"
#include "core/ui/container/group.h"
#include "core/ui/input/text_box.h"
#include "core/ui/input/text.h"
#include "core/ui/navigation/scrollbar.h"
#include "core/ui/navigation/tab.h"
#include "core/ui/selection/button.h"
#include "core/ui/selection/check_box.h"
#include "core/ui/selection/cycle_button.h"
#include "core/ui/selection/drop_down.h"
#include "core/ui/selection/icon.h"
#include "core/ui/selection/list_box.h"
#include "core/ui/selection/number_picker.h"
#include "core/ui/selection/radio_button.h"
#include "core/ui/selection/slider.h"
#include "core/ui/static/label.h"
#include "core/ui/static/progress_bar.h"
#include "core/ui/static/tooltip.h"


#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <utility>


namespace core {

  namespace ui {


    std::uint32_t widget::s_widget_count = 0;


    widget::widget()
      :
        m_background_color(),
        m_focus_color(),
        m_active_color(),
        m_position(),
        m_size(),
        m_id(0),
        m_parent_id(0),
        m_layer(0),
        m_is_active(false),
        m_is_focus(false)
    {
      m_id = ++s_widget_count;
    }


    widget::widget(core::support::duktape& duk, widget* parent) : m_background_color(), m_focus_color(), m_active_color(), m_position(), m_size(), m_parent_id(), m_is_active(false), m_is_focus(false) {
      for (std::string const& property : this->required_attributes(duk)) {
        if (!duk.has_property(property)) {
          char buffer[1024];
          std::snprintf(buffer, 1024, "required property missing: %s", property.c_str());
          throw core::error::json_error(buffer);
        }
      }
      m_position.x = duk.get_int_property("position_x");
      m_position.y = duk.get_int_property("position_y");
      m_absolute_position = m_position;
      m_size.x = duk.get_int_property("size_x");
      m_size.y = duk.get_int_property("size_y");
      m_id = duk.get_string_property("id");
      m_layer = 0;

      if (duk.has_property("background_color")) {
        m_background_color = convert_color(duk.get_string_property("background_color"));
      }

      if (duk.has_property("focus_color")) {
        m_focus_color = convert_color(duk.get_string_property("focus_color"));
      }

      if (duk.has_property("active_color")) {
        m_active_color = convert_color(duk.get_string_property("active_color"));
      }

      if (parent) {
        m_parent_id = parent->get_id();
        m_absolute_position += parent->get_absolute_position();
        m_layer = parent->get_layer() + 1;
      }

    }


    widget* widget::parse_widget(core::support::duktape& duk, widget* parent) {
      widget* result = nullptr;
      try {
        if (!duk.has_property("type"))
          throw core::error::json_error("required property missing: type");
        std::string type = duk.get_string_property("type");
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        if ("window" == type) {
          result = new window(duk, parent);
        } else if ("button" == type) {
          result = new button(duk, parent);
        }
      } catch (core::error::error const& error) {
      }
      return result;
    }


    widget::t_attributes const& widget::required_attributes(core::support::duktape& duk) {
      static const t_attributes c_attributes({
        "id",
        "position_x",
        "position_y",
        "size_x",
        "size_y",
        "type"
      });
      return c_attributes;
    }


    std::vector<widget::fvec2> widget::get_coordinates_ccw_2d() const {
      std::vector<fvec2> result;
      result.push_back(get_left_upper_2d());
      result.push_back(get_left_lower_2d());
      result.push_back(get_right_upper_2d());
      result.push_back(get_right_upper_2d());
      result.push_back(get_left_lower_2d());
      result.push_back(get_right_lower_2d());
      return result;
     }


    std::vector<widget::fvec3> widget::get_coordinates_ccw_3d() const {
      std::vector<fvec3> result;
      result.push_back(get_left_upper_3d());
      result.push_back(get_left_lower_3d());
      result.push_back(get_right_upper_3d());
      result.push_back(get_right_upper_3d());
      result.push_back(get_left_lower_3d());
      result.push_back(get_right_lower_3d());
      return result;
    }


    std::vector<widget::fvec4> widget::get_coordinates_ccw_4d() const {
      std::vector<fvec4> result;
      result.push_back(get_left_upper_4d());
      result.push_back(get_left_lower_4d());
      result.push_back(get_right_upper_4d());
      result.push_back(get_right_upper_4d());
      result.push_back(get_left_lower_4d());
      result.push_back(get_right_lower_4d());
      return result;
    }


    std::vector<widget::fvec2> widget::get_coordinates_cw_2d() const {
      std::vector<fvec2> result;
      result.push_back(get_left_upper_2d());
      result.push_back(get_right_upper_2d());
      result.push_back(get_left_lower_2d());
      result.push_back(get_left_lower_2d());
      result.push_back(get_right_upper_2d());
      result.push_back(get_right_lower_2d());
      return result;
    }


    std::vector<widget::fvec3> widget::get_coordinates_cw_3d() const {
      std::vector<fvec3> result;
      result.push_back(get_left_upper_3d());
      result.push_back(get_right_upper_3d());
      result.push_back(get_left_lower_3d());
      result.push_back(get_left_lower_3d());
      result.push_back(get_right_upper_3d());
      result.push_back(get_right_lower_3d());
      return result;
    }


    std::vector<widget::fvec4> widget::get_coordinates_cw_4d() const {
      std::vector<fvec4> result;
      result.push_back(get_left_upper_4d());
      result.push_back(get_right_upper_4d());
      result.push_back(get_left_lower_4d());
      result.push_back(get_left_lower_4d());
      result.push_back(get_right_upper_4d());
      result.push_back(get_right_lower_4d());
      return result;
    }


    widget::fvec2 widget::get_left_upper_2d() const {
      return widget::fvec2(
        m_absolute_position.x,
        m_absolute_position.y
      );
    }


    widget::fvec2 widget::get_left_lower_2d() const {
      return widget::fvec2(
        m_absolute_position.x,
        m_absolute_position.y + m_size.y
      );
    }


    widget::fvec2 widget::get_right_lower_2d() const {
      return widget::fvec2(
        m_absolute_position.x + m_size.x,
        m_absolute_position.y + m_size.y
      );
    }


    widget::fvec2 widget::get_right_upper_2d() const {
      return widget::fvec2(
        m_absolute_position.x + m_size.x,
        m_absolute_position.y
      );
    }


    widget::fvec3 widget::get_left_upper_3d() const {
      return widget::fvec3(
        m_absolute_position.x,
        m_absolute_position.y,
        m_layer
      );
    }


    widget::fvec3 widget::get_left_lower_3d() const {
      return widget::fvec3(
        m_absolute_position.x,
        m_absolute_position.y + m_size.y,
        m_layer
      );
    }


    widget::fvec3 widget::get_right_lower_3d() const {
      return widget::fvec3(
        m_absolute_position.x + m_size.x,
        m_absolute_position.y + m_size.y,
        m_layer
      );
    }


    widget::fvec3 widget::get_right_upper_3d() const {
      return widget::fvec3(
        m_absolute_position.x + m_size.x,
        m_absolute_position.y,
        m_layer
      );
    }


    widget::fvec4 widget::get_left_upper_4d() const {
      return widget::fvec4(
        m_absolute_position.x,
        m_absolute_position.y,
        m_layer,
        0.0
      );
    }


    widget::fvec4 widget::get_left_lower_4d() const {
      return widget::fvec4(
        m_absolute_position.x,
        m_absolute_position.y + m_size.y,
        m_layer,
        0.0
      );

    }


    widget::fvec4 widget::get_right_lower_4d() const {
      return widget::fvec4(
        m_absolute_position.x + m_size.x,
        m_absolute_position.y + m_size.y,
        m_layer,
        0.0
      );

    }


    widget::fvec4 widget::get_right_upper_4d() const {
      return widget::fvec4(
        m_absolute_position.x + m_size.x,
        m_absolute_position.y,
        m_layer,
        0.0
      );

    }


    bool widget::compare_front_to_back(widget const*const a, widget const*const b) {
      return a->get_layer() > b->get_layer();
    }


    bool widget::compare_back_to_front(widget const*const a, widget const*const b) {
      return b->get_layer() < a->get_layer();
    }


    widget::fvec4 widget::convert_color(std::string const& color_string) {
      if (color_string.size() != 8)
        throw core::error::json_error("wrong color format size");

      fvec4 color;
      std::uint32_t color_code = std::stoll(color_string, nullptr, 16);
      color.a = float(color_code % 0x100) / float(0xff);
      color_code >>= 8;
      color.b = float(color_code % 0x100) / float(0xff);
      color_code >>= 8;
      color.g = float(color_code % 0x100) / float(0xff);
      color_code >>= 8;
      color.r = float(color_code % 0x100) / float(0xff);

      return color;
    }


    void widget::set_position(vec2 position) {
      auto delta = position - m_position;
      m_position += delta;
      m_absolute_position += delta;
    }


    void widget::set_size(vec2 size) {
      m_size = size;
    }


    void widget::set_layer(std::uint32_t layer) {
      m_layer = layer;
    }


    widget::t_widget_vector widget::get_all_widgets() {
      return t_widget_vector({this});
    }


    widget::fvec4 widget::get_current_color() const {
      if (m_is_active) {
        return m_active_color;
      } else if (m_is_focus) {
        return m_focus_color;
      }
      return m_background_color;
    }

  }

}

