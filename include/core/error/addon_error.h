/*
 * addon_error.h
 *
 *  Created on: 7 Feb 2016
 *      Author: scones
 */

#ifndef INCLUDE_CORE_ERROR_ADDON_ERROR_H_
#define INCLUDE_CORE_ERROR_ADDON_ERROR_H_


#include "core/error/error.h"


namespace core {

  namespace error {

    class addon_error : public core::error::error {

      public:

        addon_error(std::string const& what) : core::error::error(what) {

        }

    };

  }

}


#endif /* INCLUDE_CORE_ERROR_ADDON_ERROR_H_ */

