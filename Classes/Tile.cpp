//
// Created by sju on 15.11.18.
//

#include "Tile.h"

USING_NS_CC;

TileSprite::TileSprite() {

}

TileSprite *TileSprite::createTileSprite(int _x, int _y, int _tileSize, int _n, std::string _type, std::string _path) {
    auto tileSprite = new TileSprite();

    if (tileSprite && tileSprite->initWithFile(_path)) {
        tileSprite->setX(_x);
        tileSprite->setY(_y);
        tileSprite->setTileSize(_tileSize);
        tileSprite->setN(_n);
        tileSprite->setType(_type);

        tileSprite->autorelease();
        tileSprite->setPosition(Vec2(_x * _tileSize, _y * _tileSize));
        return tileSprite;
    }

    return nullptr;
}

bool TileSprite::init() {
    if (!Sprite::init())
        return false;
    return true;
}


void TileSprite::onEnter() {
    Node::onEnter();
}

void TileSprite::update(float dt) {
    Node::update(dt);
}


