//
// Created by sju on 15.11.18.
//

#include "Tile.h"

USING_NS_CC;

TileSprite::TileSprite() {

}

TileSprite *TileSprite::createTileSprite(int x, int y, int tileSize, int n, std::string _type, std::string path) {
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


