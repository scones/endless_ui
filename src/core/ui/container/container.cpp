/*
 * container.cpp
 *
 *  Created on: 23.11.2015
 *      Author: scones
 */


#include "core/ui/container/container.h"


namespace core {

  namespace ui {

/*
//    container::container(std::string id, vec2 const& position, vec2 const& size, std::string parent_id, std::uint32_t layer, std::uint32_t state, t_widget_map const& widgets)
//      :
//        widget(id, position, size, parent_id, vec4(), layer),
//        m_widgets(widgets)
//    {}
*/


    container::container(core::support::duktape& duk, std::string const& parent_id) : widget(duk, parent_id) {
      if (duk.has_property("children")) {
        duk.get_prop_string("children");

        if (duk.is_array()) {
          duk.enumerate(0);

          while(duk.next(1)) {
            widget* child = widget::parse_widget(duk);
            if (child)
              m_widgets[child->get_id()] = child;

            duk.pop_2();
          }

          duk.pop(); // duk_enum
        }

        duk.pop(); // get_prop_string
      }

    }

  }

}

