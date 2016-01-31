/*
 * window.cpp
 *
 *  Created on: 27 Jan 2016
 *      Author: scones
 */


#include "core/ui/container/window.h"


namespace core {

  namespace ui {

    window::window(core::support::duktape& duk, widget* parent) : container(duk, parent) {
    }

  }

}

