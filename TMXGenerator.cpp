#include "TMXGenerator.h"

// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// base64.cpp

/*
 base64.cpp and base64.h
 
 Copyright (C) 2004-2008 René Nyffenegger
 
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 
 */

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
// TMXGenerator.cpp

/*
 TMXGenerator.cpp and TMXGenerator.h
 
 Copyright (C) 2013 Rohin Knight
  
 This source code is provided 'as-is', without any express or implied
 warranty. In no event will the author be held liable for any damages
 arising from the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
 1. The origin of this source code must not be misrepresented; you must not
 claim that you wrote the original source code. If you use this source code
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original source code.
 
 3. This notice may not be removed or altered from any source distribution.
 
 Rohin Knight rohin.knight@gmail.com
 
 */

std::string generateXMLForTMXTiledMap(CCTMXTiledMap* map)
{
    std::ostringstream is;

    //
    // -- Lambda function to add properties
    //
    auto addPropertiesToStream = [] (CCDictionary* properties, std::ostringstream& is)
    {
        is << "<properties>" << std::endl;
        
        CCDictElement* pElement = NULL;
        CCDICT_FOREACH(properties, pElement)
        {
            CCString* value = dynamic_cast<CCString*>(pElement->getObject());
            is << "<property name=\"" << pElement->getStrKey() << "\" value=\"" << value->getCString() << "\" />" << std::endl;
        }
        
        is << "</properties>" << std::endl;        
    };
    
    //
    // -- Map XML
    //
    {
        std::string orientation = "orthogonal";   // TODO: Handle different orientations
        
        is << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r<!DOCTYPE map SYSTEM \"http://mapeditor.org/dtd/1.0/map.dtd\">" << std::endl;
        is << "<map version=\"1.0\" orientation=\"" << orientation
            << "\" width=\"" << static_cast<int>(map->getMapSize().width) << "\" height=\"" << static_cast<int>(map->getMapSize().height)
            << "\" tilewidth=\"" << static_cast<int>(map->getTileSize().width) << "\" tileheight=\"" << static_cast<int>(map->getTileSize().height) << "\">" << std::endl;
        
        addPropertiesToStream(map->getProperties(), is);
    }
    //
    // -- Tile Sets to XML
    //
    {
        CCDictionary* tilesets = new CCDictionary();
        
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(map->getChildren(), pObj)
        {
            CCTMXLayer* layer = dynamic_cast<CCTMXLayer*>(pObj);
            if(layer)
            {
                CCTMXTilesetInfo* tilesetInfo = layer->getTileSet();
                tilesets->setObject(tilesetInfo, tilesetInfo->m_sName);
            }
        }
        
        CCDictElement* pElement = NULL;
        CCDICT_FOREACH(tilesets, pElement)
        {
            CCTMXTilesetInfo* tilesetInfo = dynamic_cast<CCTMXTilesetInfo*>(pElement->getObject());
            
            is  << "<tileset firstgid=\"" << tilesetInfo->m_uFirstGid << "\" name=\"" << tilesetInfo->m_sName
                << "\" tilewidth=\"" << static_cast<int>(tilesetInfo->m_tTileSize.width) << "\" tileheight=\"" << static_cast<int>(tilesetInfo->m_tTileSize.height)
                << "\" spacing=\"" << tilesetInfo->m_uSpacing << "\" margin=\"" << tilesetInfo->m_uMargin << "\">" << std::endl;
            
            
            // NOTE: This only stores the filename.
            
            std::string path(tilesetInfo->m_sSourceImage);
            std::string filename = path.substr(path.find_last_of("\\/") + 1);
            
            is  << "<image source=\"" << filename //tilesetInfo->m_sSourceImage
                << "\" width=\"" << tilesetInfo->m_tImageSize.width << "\" height=\"" << tilesetInfo->m_tImageSize.height << "\" />" << std::endl;
            
            is  << "</tileset>" << std::endl;
        }
        
        CC_SAFE_DELETE(tilesets);
    }
    //
    // -- Layers to XML
    //
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(map->getChildren(), pObj)
        {
            CCTMXLayer* layer = dynamic_cast<CCTMXLayer*>(pObj);
            if (layer)
            {
                int layerWidth = static_cast<int>(layer->getLayerSize().width);
                int layerHeight = static_cast<int>(layer->getLayerSize().height);
                                 
                
                is  << "<layer name=\"" << layer->getLayerName() << "\" width=\"" << layerWidth << "\" height=\"" << layerHeight
                    << "\" visible=\"" << static_cast<int>(layer->isVisible()) << "\">" << std::endl;
                
                addPropertiesToStream(layer->getProperties(), is);

                std::string encoding = "base64";   // TODO: Handle different encoding
                std::string compression = "zlib";  // TODO: Handle different compression
                
                is << "<data encoding=\"" << encoding << "\" compression=\"" << compression << "\">" << std::endl;
                
                
                unsigned int* tiles = layer->getTiles();
                
                //
                // -- Compress with zlib
                //
                uLong ucompSize = (uLong)(layerWidth * layerHeight * sizeof(unsigned int));
                uLong* buffer = (uLong*)malloc(ucompSize);
                uLong compSize = compressBound(ucompSize);
                
                // Deflate
                compress((Bytef *)buffer, &compSize, (Bytef *)tiles, ucompSize);
                
                // -- Base64 Encode
                std::string encoded = base64_encode((unsigned char*)buffer, (unsigned int)compSize);
                is << encoded << std::endl;
                
                free(buffer);                                           
                
                is << "</data>" << std::endl;
                
                // TODO: Handle rotation data
                
                is << "</layer>" << std::endl;
            }
        }
    }
    //
    // -- Object Groups to XML
    //
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(map->getObjectGroups(), pObj)
        {
            CCTMXObjectGroup* objectGroup = dynamic_cast<CCTMXObjectGroup*>(pObj);
            CCAssert(objectGroup != NULL, "Invalid object group!");
            
            
            int mapWidth = static_cast<int>(map->getMapSize().width);    // TODO: Get width and height from object group
            int mapHeight = static_cast<int>(map->getMapSize().height);
            
            
            is << "<objectgroup name=\"" << objectGroup->getGroupName() << "\" width=\"" << mapWidth << "\" height=\"" << mapHeight << "\">";
            addPropertiesToStream(objectGroup->getProperties(), is);

            CCObject* pObj2;
            CCARRAY_FOREACH(objectGroup->getObjects(), pObj2)
            {
                CCDictionary* object = dynamic_cast<CCDictionary*>(pObj2);
                
                is << "<object";
                
                CCDictElement* pElement = NULL;
                CCDICT_FOREACH(object, pElement)
                {
                    CCString* value = dynamic_cast<CCString*>(pElement->getObject());
                    std::string key(pElement->getStrKey());
                    
                    is << " " << key << "=\"";
                    
                    if (key == "y") {  // Covert back to Tiled coordinate system
                        
                        is << (mapHeight * map->getTileSize().height) - atof(value->getCString()) << "\"";
                    }
                    else {
                        is << value->getCString() << "\"";
                    }
                    
                    //is << " " << key << "=\"" << value << "\"";
                }
                
                is << " />" << std::endl;
            }
            
            is << "</objectgroup>" << std::endl;
        }
    }
    //
    // --
    //
    is << "</map>";
    return is.str();
}
