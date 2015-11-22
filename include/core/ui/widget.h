/*
 * widget.h
 *
 *  Created on: 21.11.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_UI_H_
#define INCLUDE_CORE_UI_H_


#include <cinttypes>
#include <glm/glm.hpp>


namespace core {

  namespace ui {

    class widget {

      public:


      enum STATE {
        ACTIVE = 0,
        INACTIVE,
        INACCESSIBLE,
        MINIMIZED,
        MAX
      };


      typedef glm::tvec2<std::int32_t, glm::precision::highp> vec2;


      widget(vec2 position, vec2 size, std::uint32_t state = ACTIVE, std::uint32_t layer = 0)
        :
          m_position(position),
          m_size(size),
          m_state(state),
          m_layer(layer)
      {
      }
      virtual ~widget() {}
      widget(widget const& rhs) = default;
      widget(widget && rhs) = default;

      widget& operator=(widget& rhs) = default;

      virtual void click() {};
      virtual void release() {};
      virtual void enter() {};
      virtual void leave() {};


      protected:


      vec2 m_position;
      vec2 m_size;
      std::uint32_t m_state;
      std::uint32_t m_layer;
    };

  }

}


#endif /* INCLUDE_CORE_UI_H_ */

