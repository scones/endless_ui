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
#include <unordered_map>

#include <glm/glm.hpp>


#include "core/support/duktape.h"

#include "core/event/event_receiver.h"


namespace core {

  namespace ui {

    class widget : public core::event::event_receiver {

      public:


      typedef std::vector<std::string> t_attributes;
      typedef std::unordered_map<std::string, widget*> t_widget_map;
      typedef std::vector<widget*> t_widget_vector;


      enum STATE {
        ACTIVE = 0,
        INACTIVE,
        INACCESSIBLE,
        MINIMIZED,
        MAX
      };


      typedef glm::tvec2<std::int32_t, glm::precision::highp> vec2;
      typedef glm::tvec4<std::uint32_t, glm::precision::highp> vec4;

      typedef glm::tvec2<float, glm::precision::highp> fvec2;
      typedef glm::tvec3<float, glm::precision::highp> fvec3;
      typedef glm::tvec4<float, glm::precision::highp> fvec4;

      widget();
      widget(core::support::duktape& duk, widget* parent);
      virtual ~widget() {}
      widget(widget const& rhs) = default;
      widget(widget && rhs) = default;

      widget& operator=(widget& rhs) = default;
      widget& operator=(widget&&) = default;

      virtual void set_background_color(vec4 color) { m_background_color = color; }
      virtual void set_active_color(vec4 color) { m_active_color = color; }
      virtual void set_focus_color(vec4 color) { m_focus_color = color; }
      virtual void set_background_color(std::string const& color_string) { m_background_color = convert_color(color_string); }
      virtual void set_active_color(std::string const& color_string) { m_active_color = convert_color(color_string); }
      virtual void set_focus_color(std::string const& color_string) { m_focus_color = convert_color(color_string); }
      virtual void set_position(vec2 position);
      virtual void set_size(vec2 size);
      virtual void set_layer(std::uint32_t layer);
      virtual void set_active(bool state) { m_is_active = state; }
      virtual void set_focus(bool state) { m_is_focus = state; }

      std::string const& get_id() const { return m_id; }
      vec2 const& get_absolute_position() const { return m_absolute_position; }
      std::uint32_t get_layer() const { return m_layer; }
      fvec4 get_background_color() const { return m_background_color; }
      fvec4 get_focus_color() const { return m_focus_color; }
      fvec4 get_active_color() const { return m_active_color; }
      fvec4 get_current_color() const;

      virtual std::vector<fvec2> get_coordinates_ccw_2d() const;
      virtual std::vector<fvec3> get_coordinates_ccw_3d() const;
      virtual std::vector<fvec4> get_coordinates_ccw_4d() const;
      virtual std::vector<fvec2> get_coordinates_cw_2d() const;
      virtual std::vector<fvec3> get_coordinates_cw_3d() const;
      virtual std::vector<fvec4> get_coordinates_cw_4d() const;

      virtual fvec2 get_left_upper_2d() const;
      virtual fvec2 get_left_lower_2d() const;
      virtual fvec2 get_right_lower_2d() const;
      virtual fvec2 get_right_upper_2d() const;
      virtual fvec3 get_left_upper_3d() const;
      virtual fvec3 get_left_lower_3d() const;
      virtual fvec3 get_right_lower_3d() const;
      virtual fvec3 get_right_upper_3d() const;
      virtual fvec4 get_left_upper_4d() const;
      virtual fvec4 get_left_lower_4d() const;
      virtual fvec4 get_right_lower_4d() const;
      virtual fvec4 get_right_upper_4d() const;

      virtual t_widget_vector get_all_widgets();

      virtual void click() {};
      virtual void release() {};
      virtual void focus() {};
      virtual void blur() {};
      virtual void move(vec2 delta) {
        m_position += delta;
        m_absolute_position += delta;
      }

      fvec4 convert_color(std::string const& color_string);

      static widget* parse_widget(core::support::duktape&, widget* parent = nullptr);

      static bool compare_front_to_back(widget const*const a, widget const*const b);
      static bool compare_back_to_front(widget const*const a, widget const*const b);

      protected:


      virtual t_attributes const& required_attributes(core::support::duktape&);


      static std::uint32_t s_widget_count;

      fvec4 m_background_color;
      fvec4 m_focus_color;
      fvec4 m_active_color;
      vec2 m_position;
      vec2 m_absolute_position;
      vec2 m_size;
      std::string m_id;
      std::string m_parent_id;
      std::uint32_t m_layer;
      bool m_is_active;
      bool m_is_focus;
    };

  }

}


#endif /* INCLUDE_CORE_UI_H_ */

