//
// Created by sju on 14.11.18.
//

#include "Creature.h"


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

Creature *Creature::createCreatureSprite(Vec2 position, std::string path) {
    auto creature = new Creature();
    if(creature && creature->initWithFile(path))
    {
        creature->autorelease();
        creature->setPosition(position);
        return creature;
    }
}



