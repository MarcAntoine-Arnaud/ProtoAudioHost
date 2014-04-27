#ifndef LV2_GRAPH_H
#define LV2_GRAPH_H

#include <lv2/node/Node.hpp>
#include <lv2/common/World.hpp>

#include <lilv/lilvmm.hpp>

#include <vector>

namespace lv2host
{

class Lv2Graph : protected World
{
public:

  Lv2Graph();
  ~Lv2Graph();

  Node& addNode( const std::string pluginURI, int samplerate );
  
  void connect( Node& node1, Node& node2 );

  void processFrame( const std::vector<short>& bufferIn, std::vector<short>& bufferOut );

  Lilv::World& getWorld() { return World::_world; }
  Node& getNode( size_t indexNode ) const { return *_nodes.at(indexNode); }

  std::vector< short >& getInputBuffer();
  std::vector< short >& getOutputBuffer(){ return _audioBuffers.back(); };

  
  /**
  * Size of audio buffers.
  */
  static const size_t _audioBufferSize = 128;

private:

  void initAudioBuffers();

  /**
  * The list of nodes (intances) in the graph.
  */
  std::vector<Node*> _nodes;

  /**
  * Audio buffers, to manage results of the nodes's process.
  */
  std::vector< std::vector< short > > _audioBuffers;
};

}

#endif
