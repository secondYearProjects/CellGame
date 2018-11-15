//
// Created by sju on 14.11.18.
//

#include "Creature.h"

#include <random>
#include <ctime>

USING_NS_CC;


Creature::Creature() {
}

bool Creature::init() {
    if (!Sprite::init())
        return false;
    return true;
}

void Creature::onEnter() {
    Node::onEnter();
}

void Creature::moveBy(Vec2 vec, float secs) {
    runAction(MoveBy::create(secs, vec));

}

void Creature::update(float dt) {
    Node::update(dt);
}

void Creature::moveTo(cocos2d::Vec2 vec, float secs) {
    runAction(MoveBy::create(secs, vec));

}

Creature *Creature::createCreatureSprite(int _x, int _y, int _tileSize, int _n, std::string _type, std::string path) {
    auto creature = new Creature();

    if (creature && creature->initWithFile(path)) {
        creature->setX(_x);
        creature->setY(_y);
        creature->setTielSize(_tileSize);
        creature->setN(_n);
        creature->setType(_type);

        creature->autorelease();
        creature->setPosition(Vec2(_x * _tileSize, _y * _tileSize));
        return creature;
    }

    return nullptr;
}

void Creature::walk(int _x, int _y) {
    if (x + _x < 0 || y + _y < 0)
        return;
    if (x + _x > n - 1 || y + _y > n - 1)
        return;

    runAction(MoveBy::create(0.2, Vec2(_x * tileSize, _y * tileSize)));
    x += _x;
    y += _y;
}

void Creature::manage() {
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(-1, 1); // guaranteed unbiased

    int _x = uni(rng);
    int _y = uni(rng);
    walk(_x, _y);

}

void Creature::deathAnimation() {
    setAnchorPoint(Vec2(0.5, 0.5));
    moveBy(Vec2(tileSize / 2, tileSize / 2), 0);
    runAction(ScaleBy::create(5.0, 0.0));

}

void Creature::setPicture(std::string path) {
    setTexture("deadChar.png");
}





