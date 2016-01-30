/*
 * widget.h
 *
 *  Created on: 21.11.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_UI_H_
#define INCLUDE_CORE_UI_H_


#include <cinttypes>
#include <vector>
#include <string>
#include <glm/glm.hpp>


#include "core/support/duktape.h"


namespace core {

  namespace ui {

    class widget {

      public:


      typedef std::vector<std::string> t_attributes;


      enum STATE {
        ACTIVE = 0,
        INACTIVE,
        INACCESSIBLE,
        MINIMIZED,
        MAX
      };


      typedef glm::tvec2<std::int32_t, glm::precision::highp> vec2;
      typedef glm::tvec4<std::uint32_t, glm::precision::highp> vec4;


      widget();
      widget(core::support::duktape& duk, std::string const& parent_id = "");
//      widget(std::string id, vec2 const& position, vec2 const& size, std::string parent_id = "", vec4 const& color = vec4(), std::uint32_t layer = 0, std::uint32_t state = ACTIVE);
      virtual ~widget() {}
      widget(widget const& rhs) = default;
      widget(widget && rhs) = default;

      widget& operator=(widget& rhs) = default;
      widget& operator=(widget&&) = default;

      std::string get_id() const { return m_id; }

      void set_color(vec4 color);
      void set_color(std::string const& color_string);
      void set_position(vec2 position);
      void set_size(vec2 size);
      void set_layer(std::uint32_t layer);
      void set_state(std::uint32_t state);


      virtual void click() {};
      virtual void release() {};
      virtual void focus() {};
      virtual void blur() {};
      virtual void move(vec2 delta) {
        m_position += delta;
      }


      static widget* parse_config(std::string const&);
      static widget* parse_widget(core::support::duktape&);


      protected:


      virtual t_attributes const& required_attributes(core::support::duktape&);


      static const t_attributes c_mandatory_attributes;
      static std::uint32_t s_widget_count;

      vec4 m_color;
      vec2 m_position;
      vec2 m_size;
      std::string m_id;
      std::string m_parent_id;
      std::uint32_t m_layer;
      std::uint32_t m_state;
    };

  }

}


#endif /* INCLUDE_CORE_UI_H_ */

