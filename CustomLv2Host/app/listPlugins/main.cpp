#include "lv2/common/World.hpp"

#include <exception>
#include <iostream>
#include <iomanip>

int main( int argc, char** argv )
{
  try
  {
    lv2host::World lv2world;
    
    for( auto pluginPair : lv2world.getPluginList() )
    {
      std::cout << std::left << std::setw( 80 ) << pluginPair.first << pluginPair.second << std::endl;
    }
  }
  catch( ... )
  {
    std::cerr << "unknown error" << std::endl;
  }

  return 0;
}
