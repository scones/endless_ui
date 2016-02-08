/*
 * addon.h
 *
 *  Created on: 7 Feb 2016
 *      Author: scones
 */

#ifndef INCLUDE_CORE_UI_ADDON_H_
#define INCLUDE_CORE_UI_ADDON_H_


#include <string>
#include <vector>


namespace core {

  namespace ui {


    class container;


    class addon {

      public:

      explicit addon(std::string const& addon_directory_path);
      virtual ~addon();


      static std::vector<addon*> load_addons(std::string const& addon_directory);

      void reload_addon();
      void update();
      std::vector<core::ui::container*> get_widgets();


      protected:


      void shutdown();
      std::string file_contents(std::string const& file);
      void build_prefix();


      std::vector<core::ui::container*> m_widgets;
      std::vector<std::string> m_callbacks;
      std::vector<std::string> m_layouts;
      std::string m_name;
      std::string m_prefix;
      std::string m_version;
      std::string m_config_file_name;
      std::string m_hash;
    };

  }

}


#endif /* INCLUDE_CORE_UI_ADDON_H_ */

