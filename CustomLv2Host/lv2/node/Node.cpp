#include <exception>
#include <stdexcept> //runtime error
#include <iostream>

#include <lv2/node/Node.hpp>

#include <lv2/graph/Graph.hpp>


namespace lv2host
{

Node::Node( Lv2Graph* graph, const std::string pluginURIstr, int samplerate )
: _pGraph(graph)
{
  Property pluginURI = _pGraph->getWorld().new_uri( &( pluginURIstr[0] ) );
  Lilv::Plugin plugin = ( ( Lilv::Plugins )_pGraph->getWorld().get_all_plugins() ).get_by_uri( pluginURI );
  
  _pInstance = Lilv::Instance::create( plugin, samplerate, NULL );

  // instantiation failed
  if( !_pInstance )
  {
    throw std::bad_alloc( );
  }
  
  _pInstance->activate( );

  initControlBuffers( );
  connectControls( );
}

Node::~Node()
{
  //_pInstance->deactivate();
  //pInstance->free();
}

void Node::initControlBuffers( )
{
  for (unsigned int portIndex = 0; portIndex < getPlugin( ).get_num_ports(); ++portIndex)
    _controlBuffers.push_back( 0.f );
}

void Node::connectAudioInput( std::vector< short >& audioInputBuffer)
{
  for (unsigned int portIndex = 0; portIndex < getPlugin( ).get_num_ports(); ++portIndex)
  {
    Lilv::Port port = getPlugin( ).get_port_by_index( portIndex );
  
    if( port.is_a( getAudioURIProperty( ) ) && port.is_a( getInputURIProperty( ) ) )
    {
      _pInstance->connect_port( portIndex, &audioInputBuffer[0] );
    }
  }
}

void Node::connectAudioOutput( std::vector< short >& audioOutputBuffer)
{
  for (unsigned int portIndex = 0; portIndex < getPlugin( ).get_num_ports(); ++portIndex)
  {
    Lilv::Port port = getPlugin( ).get_port_by_index( portIndex );
  
    if( port.is_a( getAudioURIProperty( ) ) && port.is_a( getOutputURIProperty( ) ) )
    {
      _pInstance->connect_port( portIndex, &audioOutputBuffer[0] );
    }
  }
}


void Node::connectControls( )
{
  for (unsigned int portIndex = 0; portIndex < getPlugin( ).get_num_ports(); ++portIndex)
  {
    // BUG : infinite loop with some plugins...
    getPlugin( ).get_num_ports( );
    
    Lilv::Port port = getPlugin( ).get_port_by_index( portIndex );
    
    if( port.is_a( getControlURIProperty( ) ) )
    {
      // get default value of port
      float minValue;
      float maxValue;
      float defaultValue;
      getPlugin( ).get_port_ranges_float( &minValue, &maxValue, &defaultValue );
      
      // add a buffer for this control
      _controlBuffers.at( portIndex ) = defaultValue;
      
      // connect the port to the buffer
      _pInstance->connect_port( portIndex, &( _controlBuffers.at( portIndex ) ) );
    }
  }
}

void Node::setParam( const std::string& portSymbol, const float value)
{
  Lilv::Port port = getPlugin( ).get_port_by_symbol( _pGraph->getWorld().new_string(&portSymbol[0]) );
  if( ! port.me )
  {
    throw std::runtime_error( portSymbol + " : param not found."  );
  }
  
  if ( port.is_a( getInputURIProperty( ) ) )
  {
    _controlBuffers.at( lilv_port_get_index( getPlugin(), port ) ) = value;
  }
  else // port.is_a( getOutputURIProperty( ) )
  {
    _controlBuffers.at( lilv_port_get_index( getPlugin(), port ) ) = value;
  }
}

void Node::process(size_t sampleCount)
{
  _pInstance->run( sampleCount );
}

Lilv::Plugin Node::getPlugin( ) const {
  return ( ( Lilv::Plugins )_pGraph->getWorld().get_all_plugins() ).get_by_uri( getPluginURIProperty( ) );
}

const Node::Property Node::getPluginURIProperty( ) const 
{ 
  return _pGraph->getWorld().new_uri( &( _pInstance->get_descriptor()->URI[0] ) ); 
}

const Node::Property Node::getAudioURIProperty( ) const 
{ 
  return _pGraph->getWorld().new_uri( LILV_URI_AUDIO_PORT ); 
}

const Node::Property Node::getInputURIProperty( ) const 
{ 
  return _pGraph->getWorld().new_uri( LILV_URI_INPUT_PORT ); 
}

const Node::Property Node::getOutputURIProperty( ) const 
{ 
  return _pGraph->getWorld().new_uri( LILV_URI_OUTPUT_PORT ); }

const Node::Property Node::getControlURIProperty( ) const 
{ 
  return _pGraph->getWorld().new_uri( LILV_URI_CONTROL_PORT ); 
}

const Node::Property Node::getSymbolProperty( const std::string& symbol ) const 
{ 
  return _pGraph->getWorld().new_string( &symbol[0] ); 
}

}
