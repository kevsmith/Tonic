//
//  Multiplier.h
//  TonicDemo
//
//  Created by Morgan Packard on 1/25/13.
//  Copyright (c) 2013 Morgan Packard. All rights reserved.
//

#ifndef __TonicDemo__Multiplier__
#define __TonicDemo__Multiplier__

#include <iostream>
#include <vector>
#include "TonicFrames.h"
#include "Generator.h"
#include "GeneratorHandle.h"

namespace Tonic{


class Multiplier_ : public Generator_{
  
	TonicFrames workSpace;
  vector<Generator> inputs;

public:
  Multiplier_();
  void in(Generator& inputSource);
  void tick( TonicFrames& frames);
  
};

class Multiplier : public TemplatedGenerator<Multiplier_>{
public:
  Multiplier in(Generator& inputSource){
    gen()->in(inputSource);
    return *this;
  }
};

static Multiplier operator*(Generator a, Generator b){
  Multiplier mult;
  mult.in(a);
  mult.in(b);
  return mult;
}

}

#endif /* defined(__TonicDemo__Multiplier__) */
