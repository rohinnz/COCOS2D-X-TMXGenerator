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

### Limitations

The following (but not limited to) features still need to be added:

* Support for different orientations. Currently only supports orthogonal.
* Saving relative path for tileset image. Currently just saves the filename.
* Handle different encodings and compression. Currently only supports base64 with zlib
* Handle tile rotation

See TODO notes in code.

### cocos2d-iphone

If you're using cocos2d-iphone then check out the Objective-C version by slycrel:

https://github.com/cocos2d/cocos2d-iphone-extensions/tree/master/Extensions/TMXGenerator
