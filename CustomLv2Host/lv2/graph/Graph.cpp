
#include "lv2/graph/Graph.hpp"
#include "lv2/common/Debugger.hpp"

#include <cstring>
#include <iostream>
#include <exception>
#include <cassert>

namespace lv2host
{

Lv2Graph::Lv2Graph()
  : World()
{
  initAudioBuffers();
}

// private function
void Lv2Graph::initAudioBuffers()
{
  _audioBuffers.push_back( std::vector< short >( _audioBufferSize, 0 ) );
  _audioBuffers.push_back( std::vector< short >( _audioBufferSize, 0 ) );
  _audioBuffers.push_back( std::vector< short >( _audioBufferSize, 0 ) );
}

Lv2Graph::~Lv2Graph()
{
  for ( size_t indexNode = 0; indexNode < _nodes.size(); ++indexNode )
  {
	  //delete _nodes.at(indexNode);
  }
}

Node& Lv2Graph::addNode( const std::string pluginURI, int samplerate )
{
  Node* newNode = new Node( this, pluginURI, samplerate );
  _nodes.push_back( newNode );
  return *_nodes.back();
}

void Lv2Graph::connect( Node& node1, Node& node2 )
{
  node1.connectAudioInput( _audioBuffers.at(0) );
  node1.connectAudioOutput( _audioBuffers.at(1) );
  
  node2.connectAudioInput( _audioBuffers.at(1) );
  node2.connectAudioOutput( _audioBuffers.at(2) );
}

void Lv2Graph::processFrame( const std::vector<short>& bufferIn, std::vector<short>& bufferOut )
{
  assert( bufferIn.size() == bufferOut.size() );

  for( size_t index; index < _audioBuffers.size(); ++ index )
  {
    _audioBuffers.resize( bufferIn.size() );
  }

  // if the graph is empty
  if( _nodes.size() == 0 )
  {
    memcpy( &bufferOut[0], &bufferIn[0], bufferIn.size() * sizeof( bufferIn.at( 0 ) ) );
    return;
  }

  // copy input
  memcpy( &_audioBuffers.front()[0], &bufferIn[0], bufferIn.size() * sizeof( bufferIn.at( 0 ) ) );
  
  // process nodes
  for ( unsigned int indexInstance = 0; indexInstance < _nodes.size(); ++indexInstance )
  {
    //getNode( indexInstance ).process( 1 );
  }
  
  // copy output
  memcpy( &bufferOut[0], &_audioBuffers.back()[0], bufferIn.size() * sizeof( bufferIn.at( 0 ) ) );
}

}
