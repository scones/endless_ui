/*
 * widget.cpp
 *
 *  Created on: 22.11.2015
 *      Author: scones
 */


#include "core/ui/widget.h"
#include "core/event/event.h"


namespace core {

  namespace ui {


    std::uint32_t widget::s_widget_count = 0;


    widget::widget(vec2 position, vec2 size, std::uint32_t parent_id, std::uint32_t layer, std::uint32_t state)
      :
        m_position(position),
        m_size(size),
        m_parent_id(parent_id),
        m_layer(layer),
        m_state(state)
    {
      m_id = ++s_widget_count;
    }

  }
}

