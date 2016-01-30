/*
 * json_error.h
 *
 *  Created on: 27 Jan 2016
 *      Author: scones
 */

#ifndef INCLUDE_CORE_ERROR_JSON_ERROR_H_
#define INCLUDE_CORE_ERROR_JSON_ERROR_H_


#include "core/error/error.h"


namespace core {

  namespace error {

    class json_error : public error {

      public:


      json_error(std::string const& what) : error(what) {}
    };

  }

}


#endif /* INCLUDE_CORE_ERROR_JSON_ERROR_H_ */

