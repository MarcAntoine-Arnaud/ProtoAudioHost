#ifndef _LV2_HOST_COMMON_WORLD_HPP_
#define _LV2_HOST_COMMON_WORLD_HPP_

#include <lilv/lilvmm.hpp>

#include <vector>
#include <string>

namespace lv2host
{

typedef std::vector< std::pair< std::string, std::string > > PluginList;

class World
{
public:
	World();
	
  	PluginList getPluginList();

protected:
	Lilv::World _world;
};

}
#endif