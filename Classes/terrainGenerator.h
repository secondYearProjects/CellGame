//
// Created by sju on 17.11.18.
//

#ifndef TEMPLATECPP_TERRAINGENERATOR_H
#define TEMPLATECPP_TERRAINGENERATOR_H

#include "CImg.h"
using namespace cimg_library;

void createe()
{
    CImg<int> image(200,200);
    image.save("Resources/test.png");
}



#endif //TEMPLATECPP_TERRAINGENERATOR_H
