#include <sndfile.hh>
#include <vector>
#include <exception>
#include <iostream>

#include "lv2/node/Node.hpp"
#include "lv2/graph/Graph.hpp"


/**
* Proto which read test.wav, process 2 gain (of 5db and 10db), and write testVFX.wav
*/
int main(int argc, char** argv)
{
  if( argc != 3 )
  {
    std::cout << "error using lv2do." << std::endl;
    std::cout << "require 2 arguments:" << std::endl;
    std::cout << "    lv2do inputFile.ext outputFile.ext" << std::endl;
  }

  SndfileHandle infile;
  SndfileHandle outfile;
  
  try
  {
    infile = SndfileHandle( argv[1] );
  }
  catch( std::exception& e )
  {
    std::cout << "Exception when try to read the file " << argv[1] << ": " << e.what() << std::endl;
    return -1;
  }

  const int format = infile.format();
  const int numChannels = infile.channels();
  const int samplerate = infile.samplerate();
  
  try
  {
    outfile = SndfileHandle( argv[2], SFM_WRITE, format , numChannels , samplerate );
  }
  catch( std::exception& e )
  {
    std::cout << "Exception when try to write the file " << argv[2] << ": " << e.what() << std::endl;
    return -1;
  }
  
  const size_t nbFrames = infile.frames();
  const size_t nbSamples = nbFrames * numChannels;
  //const float timeToRead = nbFrames / (float)samplerate; //in seconds
  
  std::vector< short > audioIn;
  std::vector< short > audioOut;
  
  audioIn = std::vector< short >( nbSamples, 0 );
  audioOut = std::vector< short >( nbSamples, 0 );


  lv2host::Lv2Graph graph;
  // add nodes to the graph
  lv2host::Node& gain = graph.addNode( "http://lv2plug.in/plugins/eg-amp", samplerate );
  lv2host::Node& deesser = graph.addNode( "http://calf.sourceforge.net/plugins/Deesser", samplerate );
  // connect ports
  graph.connect( gain, deesser );
  // update params
  gain.setParam( "gain", 1.f );
  deesser.setParam( "bypass", 1.f );
  
  size_t processedSamples = 0;
  while( processedSamples <= nbSamples )
  {
    // read on disk
    size_t readedSamples = infile.read( &audioIn[0], samplerate );
    std::cout << "\rprocessedSamples : " << processedSamples << std::flush;
    
    if( ! readedSamples )
      break;
	
    // process graph
    for( size_t i = 0; i < readedSamples; ++i ) 
    {
      //graph.processFrame( audioIn, audioOut );
    }
	
    // write on disk
    outfile.write( &audioOut[processedSamples], readedSamples );
    
    processedSamples += readedSamples;
  }
  std::cout << std::endl;

  return 0;
}
