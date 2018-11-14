//
// Created by sju on 12.11.18.
//

#include "Field.h"


USING_NS_CC;

Field::Field() {
}

bool Field::init() {
    if (!Node::init())
        return false;

    return true;
}

void Field::createField(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            auto spriteTmp = Sprite::create("dibilcell.png");

            this->addChild(spriteTmp);
            spriteTmp->setAnchorPoint(Vec2(0, 0));
            spriteTmp->setPosition(Vec2(j * tileSize, i * tileSize));
            spriteTmp->setScale(tileSize / spriteTmp->getContentSize().width);
            //spriteTmp->setAnchorPoint(Vec2(0,0));


        }
    }
}

void Field::onEnter() {
    Node::onEnter();
}

void Field::moveBy(Vec2 vec, float secs) {
    runAction(MoveBy::create(secs, vec));

}

void Field::update(float dt) {
    Node::update(dt);
}

void Field::scaleBy(float duration, float scaleFactor) {
    if (currentScale * scaleFactor < MAX_SCALE && currentScale * scaleFactor > MIN_SCALE) {
        runAction(ScaleBy::create(duration, scaleFactor));
        currentScale *= scaleFactor;
    }
}

void Field::addCreature(int x, int y) {
    auto newCreature = Creature::createCreatureSprite(Vec2(x*tileSize,y*tileSize),"character.png");

    newCreature->setScale(tileSize / newCreature->getContentSize().width);
    newCreature->setAnchorPoint(Vec2(0,0));

    addChild(newCreature);

}


std::vector<Creature*> Field::creatures;

