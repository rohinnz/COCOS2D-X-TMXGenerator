COCOS2D-X-TMXGenerator
======================

Generates the xml for a TMX Tiled Map from a CCTMXTiledMap object

Very easy to use. Just one function call!

``` c++
    // 1. Load your map or create a new one
    CCTMXTiledMap* mymap = CCTMXTiledMap::create("level01.tmx");

    // 2. Edit map
    
    // 3. Generate XML
    std::string xml_output = generateXMLForTMXTiledMap(mymap);
    
    // 4. Write XML file or store in database
```
