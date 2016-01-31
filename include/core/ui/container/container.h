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


      container(core::support::duktape& duk, widget* parent = nullptr);
      virtual ~container() = default;


      t_widget_vector get_all_widgets();


      static container* parse_config(std::string const&);
      static container* parse_widget(core::support::duktape&, widget* parent = nullptr);


      protected:


      t_widget_map m_widgets;
    };
  }
}

#endif /* INCLUDE_CORE_UI_CONTAINER_CONTAINER_H_ */

