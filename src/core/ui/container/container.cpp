/*
 * container.cpp
 *
 *  Created on: 23.11.2015
 *      Author: scones
 */


#include "core/ui/container/container.h"


namespace core {

  namespace ui {

    container::container(std::string id, vec2 const& position, vec2 const& size, std::string parent_id, std::uint32_t layer, std::uint32_t state, t_widget_map const& widgets)
      :
        widget(id, position, size, parent_id, vec4(), layer),
        m_widgets(widgets)
    {}
  }

}

