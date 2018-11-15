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

void Field::createField(int _n) {
    this->n = _n;
    for (int i = 0; i < _n; i++) {
        for (int j = 0; j < _n; j++) {
            auto spriteTmp = Sprite::create("grass.png");

            this->addChild(spriteTmp);
            spriteTmp->setZOrder(-10);
            spriteTmp->setAnchorPoint(Vec2(0, 0));
            spriteTmp->setPosition(Vec2(j * tileSize, i * tileSize));
            spriteTmp->setScale(tileSize / spriteTmp->getContentSize().width);
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
    //if (currentScale * scaleFactor < MAX_SCALE && currentScale * scaleFactor > MIN_SCALE) {
    runAction(ScaleBy::create(duration, scaleFactor));
    currentScale *= scaleFactor;
    //}
}

void Field::addCreature(int x, int y, const std::string &type) {

    auto newCreature = Creature::createCreatureSprite(x % n, y % n, tileSize, n, type, "character.png");

    if (type == "lizzard")
        newCreature->setTexture("lizzard.png");

    newCreature->setScale(tileSize / newCreature->getContentSize().width);
    newCreature->setAnchorPoint(Vec2(0, 0));

    addChild(newCreature);
    creatures.push_back(newCreature);

}

void Field::gameStep(float dt) {
    for (auto creature:creatures) {
        creature->manage();
    }
    for (auto creature:creatures) {
        int x = creature->getX();
        int y = creature->getY();

        for (auto enemy:creatures) {

            // TODO: here
            creatures.erase(std::remove_if(creatures.begin(), creatures.end(), [creature, x, y](Creature *enemy) {
                                               if (enemy != creature && x == enemy->getX() && y == enemy->getY() &&
                                                   enemy->getType() != creature->getType()) {
                                                   //enemy->deathAnimation();
                                                   //enemy->removeFromParent();
                                                   enemy->setTexture("deadChar.png");
                                                   enemy->deathAnimation();
                                                   enemy->setZOrder(-1);
                                                   return true;

                                               }
                                               if (enemy != creature && x == enemy->getX() && y == enemy->getY() &&
                                                   enemy->getType() == creature->getType()) {
                                                   log("breed");
                                                   // addCreature(x + 1, y, enemy->getType());
                                               }
                                               return false;
                                           }

            ), creatures.end());

        }
    }

}

std::vector<Creature *> Field::creatures;

