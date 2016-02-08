/*
 * addon.h
 *
 *  Created on: 7 Feb 2016
 *      Author: scones
 */


#include "core/ui/addon.h"
#include "core/ui/container/container.h"
#include "core/error/addon_error.h"
#include "core/support/duktape.h"


#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include <sstream>
#include <fstream>
#include <system_error>
#include <iostream>

using namespace boost::filesystem;
using namespace boost::program_options;

namespace core {

  namespace ui {

    addon::addon(std::string const& addon_directory_path)
      : m_widgets(),
        m_callbacks(),
        m_layouts(),
        m_name(),
        m_prefix(),
        m_version(),
        m_config_file_name(),
        m_hash()
    {
      path directory(addon_directory_path);

      // sanity checks for existence ..
      if (!exists(directory))
        throw core::error::addon_error(std::string("addon directory path does not exist: ") + addon_directory_path);

      // .. and correct format
      if (!is_directory(directory))
        throw core::error::addon_error(std::string("addon directory path is not a directory: ") + addon_directory_path);

      m_name = directory.filename().string();
      build_prefix();

      for (directory_iterator it(directory); it != directory_iterator(); ++it) {
        if (".conf" == boost::algorithm::to_lower_copy(it->path().extension().string())) {
          m_config_file_name = it->path().string();
          break;
        }
      }
      if ("" == m_config_file_name)
        throw core::error::addon_error(std::string("config file missing in directory: ") + addon_directory_path);

      reload_addon();
    }


    addon::~addon(){
      shutdown();
    }


    void addon::shutdown() {
      for(auto pwidget : m_widgets)
        delete pwidget;
      m_widgets.clear();

      m_callbacks.clear();
      m_layouts.clear();
    }


    void addon::reload_addon() {
      static const std::vector<std::string> s_required_options = {
        "callbacks", "layouts", "version"
      };

      shutdown();

      // sanity checks on config file
      path config_file(m_config_file_name);
      if (!exists(config_file))
        throw core::error::addon_error(std::string("config file not found: ") + config_file.string());
      if (!is_regular_file(config_file))
        throw core::error::addon_error(std::string("config file is not of type file: ") + config_file.string());

      // setup options
      variables_map vm;
      std::string callbacks, layouts;
      options_description addon_config_options("addon config options");
      addon_config_options.add_options()
        ("callbacks", value<std::string>(&callbacks), "callbacks")
        ("layouts", value<std::string>(&layouts), "layouts")
        ("prefix", value<std::string>(&m_prefix), "prefix")
        ("version", value<std::string>(&m_version), "addon version")
        ("hash", value<std::string>(&m_hash), "hash")
      ;

      // parse config file
      ifstream config_stream(m_config_file_name);
      if (!config_stream)
        throw core::error::addon_error(std::string("unable to open config file: ") + m_config_file_name);
      store(parse_config_file(config_stream, addon_config_options, true), vm);
      notify(vm);

      // make sure all required options were present
      for (auto const& option : s_required_options)
        if (0 == vm.count(option.c_str()))
          throw core::error::addon_error(std::string("option '") + option + std::string("' not found in config file: ") + m_config_file_name);

      // load callbacks
      auto ss = std::stringstream(callbacks);
      std::string file;
      while (std::getline(ss, file, ',')) {
        boost::algorithm::trim(file);
        auto content = file_contents(config_file.parent_path().string() + '\\' + file);
        core::support::duktape::peval_string(content);
        m_callbacks.push_back(content);
      }

      // load layouts
      ss = std::stringstream(layouts);
      while(std::getline(ss, file, ',')) {
        boost::algorithm::trim(file);
        auto content = file_contents(config_file.parent_path().string() + '\\' + file);
        m_layouts.push_back(content);
        m_widgets.push_back(core::ui::container::parse_config(content));
      }
    }


    void addon::build_prefix() {
      m_prefix = m_name.substr(0, 3);
    }


    std::string addon::file_contents(std::string const& file) {
      std::ifstream file_handle(file.c_str(), std::ios::in);

      if(!file_handle.is_open())
        throw std::system_error(ENOENT, std::system_category(), file.c_str());

      return std::string((std::istreambuf_iterator<char>(file_handle) ), (std::istreambuf_iterator<char>() ));
    }


    std::vector<core::ui::container*> addon::get_widgets() {
      return m_widgets;
    }


    std::vector<addon*> addon::load_addons(std::string const& addon_directory) {
      std::vector<addon*> addons;

      // load directory
      path directory_path(addon_directory);

      // fix directory separators on windows
      directory_path.make_preferred();

      try {

        // make sure it exists
        if (!exists(directory_path)) {
          throw core::error::addon_error(std::string("addons directory does not exist: ") + addon_directory);
        }

        // make sure its a directory
        if (!is_directory(directory_path)) {
          throw core::error::addon_error(std::string("addons directory path is not a directory: ") + addon_directory);
        }

        // iterate over all contents, load addon for all directories
        for (directory_iterator it(directory_path); it != directory_iterator(); ++it) {
          if (is_directory(it->path())) {
            addons.push_back(new addon(it->path().string()));
          }
        }

      } catch ( filesystem_error const& error) {
        throw core::error::addon_error(error.what());
      }

      return addons;
    }

  }

}

