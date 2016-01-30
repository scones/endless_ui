/*
 * container.h
 *
 *  Created on: 23.11.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_UI_CONTAINER_CONTAINER_H_
#define INCLUDE_CORE_UI_CONTAINER_CONTAINER_H_


#include "../widget.h"


#include <unordered_map>


namespace core {

  namespace ui {

    class container : public core::ui::widget {

      public:


      typedef std::unordered_map<std::string, widget*> t_widget_map;


      container(core::support::duktape& duk, std::string const& parent_id = "");
//      container(std::string id, vec2 const&  position, vec2 const& size, std::string parent_id, std::uint32_t layer, std::uint32_t state, t_widget_map const& widgets);
      virtual ~container() = default;

      protected:


      t_widget_map m_widgets;
    };
  }
}

#endif /* INCLUDE_CORE_UI_CONTAINER_CONTAINER_H_ */

