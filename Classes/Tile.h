//
// Created by sju on 15.11.18.
//

#ifndef TEMPLATECPP_TILE_H
#define TEMPLATECPP_TILE_H

#include "cocos2d.h"

class TileSprite : public cocos2d::Sprite {
public:
    CREATE_FUNC(TileSprite);

    virtual bool init();

    int getX() const { return x; }

    int getY() const { return y; }

    int getTileSize() const { return tileSize; }

    std::string getType() const { return type; }

    std::string getPath() const { return path; }


    void setX(int _x) { x = _x; }

    void setY(int _y) { y = _y; }

    void setN(int _n) { n = _n; }

    void setTileSize(int _tileSize) { tileSize = _tileSize; }

    void setType(const std::string &_type) { type = _type; }

    void setPath(const std::string &_path) { path = _path; }


    static TileSprite *createTileSprite(int x, int y, int tileSize, int n, std::string _type, std::string path);

    virtual void onEnter();

    virtual void update(float dt) override;

private:
    TileSprite();

    ~TileSprite() {};

    int x, y, tileSize, n;
    std::string type = "grass";
    std::string path = "grass.png";
};


#endif //TEMPLATECPP_TILE_H
