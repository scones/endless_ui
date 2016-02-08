/*
 * duktape.h
 *
 *  Created on: 23 Jan 2016
 *      Author: scones
 */

#ifndef INCLUDE_CORE_SUPPORT_DUKTAPE_H_
#define INCLUDE_CORE_SUPPORT_DUKTAPE_H_


#include "core/error/duktape_error.h"


#include <duktape/duktape.h>
#include <cinttypes>
#include <iostream>


namespace core {

  namespace support {

    class duktape {

      public:


      duktape() : m_stack_depth(0) {
        m_context = duk_create_heap(nullptr, nullptr, nullptr, nullptr, core::error::duktape_error::error_handler);
        if (!m_context)
          throw core::error::duktape_error("unable to create duktape context");
      }
      ~duktape() {
        duk_destroy_heap(m_context);
        m_context = 0;
        m_stack_depth = 0;
      }
      duktape(duktape&& right) {
        m_context = right.m_context;
        m_stack_depth = right.m_stack_depth;
        right.m_context = 0;
        right.m_stack_depth = 0;
      }
      duktape(duktape&) = delete;


      static duktape create_from_json(std::string const& json) {
        core::support::duktape duk;
        duk.push_string(json.c_str());
        duk.json_decode(0);
        return duk;
      }


      static duktape peval_string(std::string const& source) {
        core::support::duktape duk;
        duk.push_string(source.c_str());
        if (0 != duk.peval())
          throw core::error::duktape_error(std::string("peval failed:") + duk.safe_to_string());

        return duk;
      }


      inline std::uint32_t peval() {
        return duk_peval(m_context);
      }


      inline char const* push_string(std::string const& input) {
        return duk_push_string(m_context, input.c_str());
      }

      inline void json_decode(std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        ++m_stack_depth;
        duk_json_decode(m_context, stack_index);
      }

      inline bool get_prop_string(std::string const& property_name, std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        ++m_stack_depth;
        return duk_get_prop_string(m_context, stack_index, property_name.c_str());
      }

      std::uint32_t get_top() {
        return duk_get_top(m_context);
      }

      const char* get_string(std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        ++m_stack_depth;
        return duk_get_string(m_context, stack_index);
      }

      std::int32_t get_int(std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        ++m_stack_depth;
        return duk_get_int(m_context, stack_index);
      }

      void pop() {
        --m_stack_depth;
        duk_pop(m_context);
      }

      void pop_2() {
        m_stack_depth -= 2;
        duk_pop_2(m_context);
      }

      void pop_3() {
        m_stack_depth -= 3;
        duk_pop_3(m_context);
      }

      void pop_n(std::uint32_t count) {
        m_stack_depth -= count;
        duk_pop_n(m_context, count);
      }

      std::string get_string_property(std::string const& property_name, std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        get_prop_string(property_name.c_str());
        std::string result = get_string(stack_index);
        pop();
        return result;
      }

      std::int32_t get_int_property(std::string const& property_name, std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        get_prop_string(property_name.c_str());
        std::int32_t result = get_int(stack_index);
        pop();
        return result;
      }

      bool has_property(std::string const& property_name, std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        return duk_has_prop_string(m_context, stack_index, property_name.c_str());
      }


      bool is_array(std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        return duk_is_array(m_context, stack_index);
      }


      void enumerate(std::uint32_t flags, std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        duk_enum(m_context, stack_index, flags);
      }


      bool next(std::uint32_t get_value, std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        return duk_next(m_context, stack_index, get_value);
      }


      bool get_index_property(std::uint32_t array_index, std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        return duk_get_prop_index(m_context, stack_index, array_index);
      }


      std::string safe_to_string(std::int32_t stack_index = DEFAULT_STACK_POSITION) {
        return duk_safe_to_string(m_context, stack_index);
      }

      protected:


      static constexpr std::int32_t DEFAULT_STACK_POSITION = -1;


      duk_context* m_context;
      int m_stack_depth;
    };

  }

}


#endif /* INCLUDE_CORE_SUPPORT_DUKTAPE_H_ */

