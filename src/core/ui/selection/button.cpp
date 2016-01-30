/*
 * button.cpp
 *
 *  Created on: 27 Jan 2016
 *      Author: scones
 */


#include "core/ui/selection/button.h"


namespace core {

  namespace ui {

    button::button(core::support::duktape& duk, std::string const& parent_id) : widget(duk, parent_id) {

    }
  }
}

