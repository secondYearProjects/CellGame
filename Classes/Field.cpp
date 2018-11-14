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
    createField();

    return true;
}

void Field::createField() {
    int n = 20;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            auto spriteTmp = Sprite::create("dibilcell.png");

            this->addChild(spriteTmp);
            spriteTmp->setAnchorPoint(Vec2(0, 0));
            spriteTmp->setPosition(Vec2(j * 30, i*30));
            spriteTmp->setScale(30 / spriteTmp->getContentSize().width);
            //spriteTmp->setAnchorPoint(Vec2(0,0));


        }
    }
}

void Field::onEnter() {
    Node::onEnter();
}

void Field::moveBy(Vec2 vec) {
    runAction(MoveTo::create(2, vec));
}




