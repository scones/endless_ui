/*
 * container.cpp
 *
 *  Created on: 23.11.2015
 *      Author: scones
 */


#include "core/ui/container/container.h"


namespace core {

  namespace ui {

    container::container(vec2 position, vec2 size, std::uint32_t parent_id, std::uint32_t layer, std::uint32_t state, std::vector<widget*> widgets)
      :
        widget(position, size, parent_id, layer),
        m_widgets(widgets)
    {}
  }

}

