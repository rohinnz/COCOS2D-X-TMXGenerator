#pragma once

#include <iostream>
#include <zlib.h>
#include "cocos2d.h"
USING_NS_CC;

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// base64.h
std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// TMXGenerator.h
std::string generateXMLForTMXTiledMap(CCTMXTiledMap* map);
