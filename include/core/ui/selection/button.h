/*
 * button.h
 *
 *  Created on: 21.11.2015
 *      Author: scones
 */

#ifndef INCLUDE_CORE_UI_SELECTION_BUTTON_H_
#define INCLUDE_CORE_UI_SELECTION_BUTTON_H_


#include "core/ui/widget.h"


namespace core {

  namespace ui {

    class button : public core::ui::widget {

      public:


      button(core::support::duktape& duk, std::string const& parent_id = "");
      virtual ~button() = default;
    };
  }

}


#endif /* INCLUDE_CORE_UI_SELECTION_BUTTON_H_ */
