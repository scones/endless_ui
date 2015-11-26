/*
 * container.h
 *
 *  Created on: 23.11.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_UI_CONTAINER_CONTAINER_H_
#define INCLUDE_CORE_UI_CONTAINER_CONTAINER_H_


#include "../widget.h"


#include <vector>


namespace core {

  namespace ui {

    class container : public core::ui::widget {

      public:


      container(vec2 position, vec2 size, std::uint32_t parent_id, std::uint32_t layer, std::uint32_t state, std::vector<widget*> widgets);


      protected:


      std::vector<widget*> m_widgets;
    };
  }
}

#endif /* INCLUDE_CORE_UI_CONTAINER_CONTAINER_H_ */

