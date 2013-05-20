//
//  BufferFiller.h
//  Tonic
//
//  Created by Nick Donaldson on 2/9/13.

//
// See LICENSE.txt for license and usage information.
//


#ifndef __Tonic__BufferFiller__
#define __Tonic__BufferFiller__

#include <map>
#include "Generator.h"

//! BufferFiller is the base class for any generator expected to produce output for a buffer fill.
/*!
 BufferFillers provide a high-level interface for combinations of generators, and can be used to fill
 arbitraryly large buffers.
  
 Subclasses to include mixer, channel, synth, etc.
*/
namespace Tonic{
  
  namespace Tonic_ {
    
    class BufferFiller_ : public Generator_ {
      
    private:
      
      int                         bufferReadPosition_;
      
    protected:
      
      Tonic_::SynthesisContext_   synthContext_;
      
    public:
      
      BufferFiller_();
      
      //! Process a single synthesis vector, output to frames
      /*!
       tick method without context argument passes down this instance's SynthesisContext_
       */
      void tick( TonicFrames& frames );
      
      void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels);

    };
    
    
    inline void BufferFiller_::tick( TonicFrames& frames ){
      Generator_::tick(frames, synthContext_);
      synthContext_.tick();
    }
    
    inline void BufferFiller_::fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels)
    {
#ifdef TONIC_DEBUG
      if(numChannels > outputFrames_.channels()) error("Mismatch in channels sent to Synth::fillBufferOfFloats", true);
#endif
      
      const unsigned int sampleCount = outputFrames_.size();
      const unsigned int channelsPerSample = (outputFrames_.channels() - numChannels) + 1;
      
      TonicFloat sample = 0.0f;
      TonicFloat *outputSamples = &outputFrames_[bufferReadPosition_];
      
      for(unsigned int i = 0; i<numFrames * numChannels; i++){
        
        sample = 0;
        
        for (unsigned int c = 0; c<channelsPerSample; c++){
          if(bufferReadPosition_ == 0){
            tick(outputFrames_);
          }
          
          sample += *outputSamples++;
          
          if(++bufferReadPosition_ == sampleCount){
            bufferReadPosition_ = 0;
            outputSamples = &outputFrames_[0];
          }
        }
        
        *outData++ = sample / (float)channelsPerSample;
      }
    }
    
  }
  
  class BufferFiller : public Generator {
    
  public:
        
    //! Fill an arbitrarily-sized, interleaved buffer of audio samples as floats
    /*!
     This BufferFiller's outputGen is used to fill an interleaved buffer starting at outData.
     */
    inline void fillBufferOfFloats(float *outData,  unsigned int numFrames, unsigned int numChannels){
      static_cast<Tonic_::BufferFiller_*>(mGen)->fillBufferOfFloats(outData, numFrames, numChannels);
    }
  
  };
  
  template<class GenType>
  class TemplatedBufferFiller : public BufferFiller {
  protected:
    GenType* gen(){
      return static_cast<GenType*>(mGen);
    }
  public:
    TemplatedBufferFiller(){
      delete mGen;
      mGen = new GenType();
    }
  };
  
}

#endif /* defined(__TonicDemo__BufferFiller__) */
