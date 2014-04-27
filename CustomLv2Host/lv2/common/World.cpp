#include <lv2/common/World.hpp>

namespace lv2host
{

World::World()
  : _world( Lilv::World() )
{
  _world.load_all();
}


PluginList World::getPluginList()
{
  PluginList list;
  const LilvPlugins* pluginList = _world.get_all_plugins();

  LILV_FOREACH( plugins, i, pluginList )
  {
    const LilvPlugin* p = lilv_plugins_get( pluginList, i );
    LilvNode* n = lilv_plugin_get_name( p );

    list.push_back( std::pair<std::string, std::string>(
      lilv_node_as_uri( lilv_plugin_get_uri( p ) ),
      lilv_node_as_string( n )
      ) );

    lilv_node_free( n );
  }
  return list;
}

}