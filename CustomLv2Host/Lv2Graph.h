#ifndef LV2_GRAPH_H
#define LV2_GRAPH_H

// Lilv wrapper C++
#include <lilv/lilvmm.hpp>

#include "Node.h"


namespace sound
{

class Lv2Graph
{
public:
  Lv2Graph();
  ~Lv2Graph();

  Node& addNode( const std::string pluginURI, int samplerate );

  void connect( Node& node1, Node& node2 );

  void processFrame( const short* bufferIn, short* bufferOut );

  Lilv::World* getWorld() const {return pWorld;}
  Node& getNode( size_t indexNode ) const { return *nodes.at(indexNode); }
  
  /**
  * Size of audio buffers.
  */
  static const size_t audioBufferSize = 128;

private:

  void initAudioBuffers();

  /**
  * Contains all data of all lv2 plugins on the system.
  */
  Lilv::World* pWorld;

  /**
  * The list of nodes (intances) in the graph.
  */
  std::vector<Node*> nodes;

  /**
  * Audio buffers, to manage results of the nodes's process.
  */
  std::vector< std::vector< short > > audioBuffers;
};

}

#endif //LV2_GRAPH_H