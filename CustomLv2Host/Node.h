#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

// Lilv wrapper C++
#include <lilv/lilvmm.hpp>

namespace sound
{

class Lv2Graph;

class Node
{
  typedef Lilv::Node Property;

public:

  Node( Lv2Graph* graph, const std::string pluginURI, int sampleRate );
  ~Node( );

  //Node& operator=(const Node& otherNode);

  void connectAudioInput( std::vector< short >& audioInputBuffer );
  void connectAudioOutput( std::vector< short >& audioOutputBuffer );

  void connectControlInput( );
  void connectControlOutput( );

  void setParam( const std::string& portSymbol, const unsigned int value );

  void process( size_t sampleCount );

  /**
  * Getters
  */
  Lv2Graph* getGraph() const { return pGraph; }
  Lilv::Instance* getInstance( ) const { return pInstance; }
  std::vector< std::vector< int > > getControlBuffers() const { return controlBuffers; }
  Lilv::Plugin getPlugin( Property pluginURI ) const;

private:

  void initAudioBuffers( );

  Lv2Graph*       pGraph;

  Lilv::Instance* pInstance;
  
  std::vector< std::vector< int > > controlBuffers;

  static const size_t bufferControlInput = 0;
  static const size_t bufferControlOutput = 1;
};

}


#endif //NODE_H