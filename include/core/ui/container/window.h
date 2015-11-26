/*
 * window.h
 *
 *  Created on: 21.11.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_UI_CONTAINER_WINDOW_H_
#define INCLUDE_CORE_UI_CONTAINER_WINDOW_H_


#include "container.h"


namespace core {

  namespace ui {

    class window : public core::ui::container {

      public:


      window(vec2 position, vec2 size, std::uint32_t parent_id, std::uint32_t layer, std::uint32_t state, std::vector<widget*> widgets);


      protected:


    };

  }

}


#endif /* INCLUDE_CORE_UI_CONTAINER_WINDOW_H_ */
