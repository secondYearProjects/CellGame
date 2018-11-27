## Libraries used in Terrain generation module and CellGame project

### 1) <a href="https://github.com/cocos2d/cocos2d-x"> cocos2d-x </a>
Multi-platform framework for building 2d games. 
This framework was chosen due to my past experiance of using it. I didn't want to use some bigger game frameworks like unity/unreal  because of my will of creating game without using GUI creators on c++ to see how it behaves during developing of a game. I know about existence of Cocos Creator, but it supports c++ in a translation way. 

Installation on linux was really simple and starter <a href="http://docs.cocos2d-x.org/cocos2d-x/en/"> manuals </a> were enough to understand basics and obtain intuitive mind-path of cocos2d-x. <a href="http://docs.cocos2d-x.org/api-ref/cplusplus/v3x/"> Other </a> parts of documentation is harder to use (compared to Qt framework as an example) but is great for a full open source project. Basic modules are simple in use and good from easy-code perspective.

As a bad thing goes the size of a hello-world project (~330Mb) so to transport your project you probably will use some kind of archivation for cocos2d dir in your project. Also it was harder to install cocos2d-x on windows but after some tricks with cygwin it worked. So as a bad thing i would mention transportability of a project.
I think cocos2d-x is a correct solution to create 2d game with c++.


### 2) <a href="https://github.com/Auburns/FastNoise"> FastNoise </a>
Noise library for modern C++.
Easy to use library with efficient code for this task. Nothing more to say :)


### 3) <a href="https://github.com/effolkronium/random"> random.h Random for modern C++ with convenient API </a>
Great header-only lib for random generation. 
Is simple to use and makes random generation easy by wrapping efficient random into nice interface.
As a lack of functionality - no Discrete Distribution wrapper so I needed to use standard one.


### 4) <a href="https://github.com/dtschump/CImg"> CImg Library </a>
Header-only toolkit for image processing. 
Has huge functional for image generation and transform, that was needed for my project. 
After looking through a tutorial you can use basic functionality with ease.
The only bad thing that I've faced with this lib is non-default support of a png alpha channel, but png.h lib is included to cocos2d-x so after some search I understood how to use it.
Definitely would use it again for similar tasks (as an example in ML or smth.).


