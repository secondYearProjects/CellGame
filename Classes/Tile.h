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

    static TileSprite *createTileSprite(int x, int y, int tileSize, int n, std::string _type, std::string path);

    virtual void onEnter();

    virtual void update(float dt) override;

private:
    TileSprite();
    ~TileSprite();
    int x, y, tileSize;
    std::string type = "human";
};


#endif //TEMPLATECPP_TILE_H
