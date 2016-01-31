/*
 * container.cpp
 *
 *  Created on: 23.11.2015
 *      Author: scones
 */


#include "core/ui/container/container.h"
#include "core/error/duktape_error.h"
#include "core/error/json_error.h"

#include "core/ui/container/window.h"
#include "core/ui/selection/button.h"


#include <algorithm>
#include <cstring>
#include <iterator>


namespace core {

  namespace ui {

/*
//    container::container(std::string id, vec2 const& position, vec2 const& size, std::string parent_id, std::uint32_t layer, std::uint32_t state, t_widget_map const& widgets)
//      :
//        widget(id, position, size, parent_id, vec4(), layer),
//        m_widgets(widgets)
//    {}
*/


    container::container(core::support::duktape& duk, widget* parent) : widget(duk, parent) {
      if (duk.has_property("children")) {
        duk.get_prop_string("children");

        if (duk.is_array()) {
          duk.enumerate(0);

          while(duk.next(1)) {
            widget* child = widget::parse_widget(duk, this);
            if (child)
              m_widgets[child->get_id()] = child;

            duk.pop_2();
          }

          duk.pop(); // duk_enum
        }

        duk.pop(); // get_prop_string
      }

    }


    container* container::parse_config(std::string const& input) {
      try {
        auto duk = core::support::duktape::create_from_json(input);
        return parse_widget(duk);
      } catch (core::error::error const& error) {
        return nullptr;
      }
    }


    container* container::parse_widget(core::support::duktape& duk, widget* parent) {
      container* result = nullptr;
      try {
        if (!duk.has_property("type"))
          throw core::error::json_error("required property missing: type");
        std::string type = duk.get_string_property("type");
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        if ("window" == type) {
          result = new window(duk, parent);
        }
      } catch (core::error::error const& error) {
      }
      return result;
    }


    widget::t_widget_vector container::get_all_widgets() {
      auto vector(widget::get_all_widgets());
      for (auto widget_pair : m_widgets) {
        widget* w = widget_pair.second;
        t_widget_vector children = w->get_all_widgets();
        vector.insert(vector.end(), std::make_move_iterator(children.begin()), std::make_move_iterator(children.end()));
      }
      return vector;
    }
  }

}

