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


namespace core {

  namespace ui {


    std::uint32_t widget::s_widget_count = 0;
    const std::vector<std::string>  widget::c_mandatory_attributes = {
      "id",
      "position_x",
      "position_y",
      "color",
      "size_x",
      "size_y",
      "type"
    };

/*
    widget::widget(std::string id, vec2 const& position, vec2 const& size, std::string parent_id, vec4 const& color, std::uint32_t layer, std::uint32_t state)
      :
        m_color(color),
        m_position(position),
        m_size(size),
        m_id(id),
        m_parent_id(parent_id),
        m_layer(layer),
        m_state(state)
    {
      m_id = ++s_widget_count;
    }
*/

    widget::widget()
      :
        m_color(),
        m_position(),
        m_size(),
        m_id(0),
        m_parent_id(0),
        m_layer(0),
        m_state(ACTIVE)
    {
      m_id = ++s_widget_count;
    }


    widget::widget(core::support::duktape& duk, std::string const& parent_id) : m_color(), m_position(), m_size(), m_parent_id(parent_id) {
      for (std::string const& property : this->required_attributes(duk)) {
        if (!duk.has_property(property)) {
          char buffer[1024];
          std::snprintf(buffer, 1024, "required property missing: %s", property.c_str());
          throw core::error::json_error(buffer);
        }
      }
      m_position.x = duk.get_int_property("position_x");
      m_position.y = duk.get_int_property("position_y");
      m_size.x = duk.get_int_property("size_x");
      m_size.y = duk.get_int_property("size_y");
      m_id = duk.get_string_property("id");
      m_layer = 0;
      m_state = duk.get_int_property("state");
    }


    widget* widget::parse_config(std::string const& input) {
      try {
        auto duk = core::support::duktape::create_from_json(input);
        return parse_widget(duk);
      } catch (core::error::error const& error) {
        return nullptr;
      }
    }


    widget* widget::parse_widget(core::support::duktape& duk) {
      widget* result = nullptr;
      try {
        if (!duk.has_property("type"))
          throw core::error::json_error("required property missing: type");
        std::string type = duk.get_string_property("type");
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        if ("window" == type) {
          result = new window(duk);
        } else if ("button" == type) {
          result = new button(duk);
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
        "color",
        "size_x",
        "size_y",
        "type"
      });
      return c_attributes;
    }

  }

}

