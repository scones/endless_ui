/*
 * duktape_error.h
 *
 *  Created on: 23 Jan 2016
 *      Author: scones
 */

#ifndef INCLUDE_CORE_ERROR_DUKTAPE_ERROR_H_
#define INCLUDE_CORE_ERROR_DUKTAPE_ERROR_H_


#include "core/error/error.h"


#include <duktape/duktape.h>
#include <cstdio>
#include <iostream>
#include <duktape/duktape.h>


namespace core {

  namespace error {

    class duktape_error : public core::error::error {


      public:


      duktape_error(std::string const& what) : core::error::error(what) {
      }


      static void error_handler(duk_context* context, int code, char const* blubb) {
        char buffer[1024];
        std::snprintf(buffer, 1024, "duktape failed with error code %d: %s\nError: %s", code, blubb, duk_safe_to_string(context, -1));
        throw core::error::duktape_error(buffer);
      }

    };

  }

}


#endif /* INCLUDE_CORE_ERROR_DUKTAPE_ERROR_H_ */
