#include <sndfile.hh>
#include <vector>
#include <exception>
#include <iostream>

#include "Node.hpp"
#include "Lv2Graph.hpp"


/**
* Proto which read test.wav, process 2 gain (of 5db and 10db), and write testVFX.wav
*/
int main(int argc, char** argv)
{
  SndfileHandle infile;
  SndfileHandle outfile;
  try
  {
    infile = SndfileHandle( "../data/underwater.wav" );
  }
  catch(std::exception& e)
  {
    std::cout << "Exception when try to read the file :  " << e.what() << std::endl;
    return 1;
  }
  
  const int format = infile.format();
  const int numChannels = infile.channels();
  const int samplerate = infile.samplerate();
  const size_t nbFrames = infile.frames();
  const size_t nbSamples = nbFrames * numChannels;
  //const float timeToRead = nbFrames / (float)samplerate; //in seconds
  
  std::vector< short > audioIn;
  std::vector< short > audioOut;
  try
  {
    audioIn = std::vector< short > ( nbSamples, 0 );
    audioOut = std::vector< short > ( nbSamples, 0 );
  }
  catch(std::exception& e)
  {
    std::cout << "Exception when try to allocate audio buffers : " << e.what() << std::endl;
    return 1;
  }

  outfile = SndfileHandle( "../data/underwaterVFX.wav" , SFM_WRITE, format , numChannels , samplerate );

  sound::Lv2Graph graph;
  // add nodes to the graph
  sound::Node& gain = graph.addNode( "http://lv2plug.in/plugins/eg-amp", samplerate );
  sound::Node& deesser = graph.addNode( "http://calf.sourceforge.net/plugins/Deesser", samplerate );
  // connect ports
  graph.connect( gain, deesser );
  // update params
  gain.setParam( "gain", 1.f );
  deesser.setParam( "bypass", 1.f );
  
  size_t readedSamples = 0;
  while( 1 )
  {
    if ( readedSamples == nbSamples )
      break;
    
    // read on disk
    size_t currentReadedSamples = infile.read( &audioIn[readedSamples], samplerate );
    std::cout << "readedSamples : " << currentReadedSamples << std::endl;
    
	if( ! currentReadedSamples )
      break;
	
    // process graph
    for (size_t i = 0; i < currentReadedSamples; ++i) 
    {
      graph.processFrame( &audioIn[ readedSamples + i ], &audioOut[ readedSamples + i ] );
    }
	
    // write on disk
    outfile.write( &audioOut[readedSamples], currentReadedSamples );
    
    readedSamples += currentReadedSamples;
  }

  return 0;
}
