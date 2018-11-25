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
        creature->setTileSize(_tileSize);
        creature->setN(_n);
        creature->setType(_type);

        creature->autorelease();
        creature->setPosition(Vec2(_x * _tileSize, _y * _tileSize));
        return creature;
    }

    return nullptr;
}

void Creature::walk(int _x, int _y) {
    int newX = x + _x;
    int newY = y + _y;
    if (!stepAvailable(_x, _y))
        return;

    field->removeCreature(x, y, this);
    field->addCreature(newX, newY, this);

    runAction(MoveBy::create(animationSpeed, Vec2(_x * tileSize, _y * tileSize)));
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
    setTexture("deadChar.png");

    runAction(FadeOut::create(animationSpeed * 10.0f));
    field->removeCreature(x, y, this);
}

void Creature::setX(int _x) { x = _x; }

void Creature::setY(int _y) { y = _y; }

void Creature::setN(int _n) { n = _n; }

void Creature::setType(const std::string &_type) { type = _type; }

void Creature::setTileSize(int _tileSize) { tileSize = _tileSize; }

int Creature::getX() const { return x; }

int Creature::getY() const { return y; }

int Creature::getN() const { return n; }

std::string Creature::getType() { return type; }

int Creature::getTitleSize() const { return tileSize; }


bool Creature::stepAvailable(int _x, int _y) {
    int newX = x + _x;
    int newY = y + _y;
    if (newX < 0 || newY < 0) {
        //log("border");
        return false;
    }
    if (newX > n - 1 || newY > n - 1) {
        //log("border");
        return false;
    }
    if (field->getTile(newX, newY).type == terrainGenerator::TileType::water) {
        //log("into water");
        return false;
    }
    return true;
}

int Creature::getHealth() const { return health; }

void Creature::changeHealthBy(int value) {
    health = (value + health);
    if (health > maxHealth)
        health = maxHealth;

    auto tintTo = TintTo::create(animationSpeed, 255, 254.0f * (static_cast<float >(health) / static_cast<float >(maxHealth)),
                                 254.0f * (static_cast<float >(health) / static_cast<float >(maxHealth)));
    this->runAction(tintTo);
}

CreatureActions Creature::step() {
    CreatureActions actions;
    actions.breed = false;
    actions.fight = false;
    actions.fightDamage = 0;

    if (hungrySteps > stepsToHunger)
        changeHealthBy((hungrySteps - stepsToHunger) * stepHealthChange);
    else {
        changeHealthBy(stepHeal);
        if (isPregnant)
            breedingSteps++;
    }


    // Pregnant check
    if (breedingSteps > stepsToBreed) {
        actions.breed = true;
        breedingSteps = 0;
        isPregnant = false;
    }

    // Fight & Breed
    if (!field->getTile(x, y).creatures.empty()) {
        for (int i = 0; i < field->getTile(x, y).creatures.size(); i++) {
            auto &enemy = field->getTile(x, y).creatures[i];

            if (enemy->getType() != this->getType()) {
                actions.fight = true;
                actions.fightDamage += enemy->getPower();
                log("fight");
            } else if (enemy->getType() == this->getType() && enemy != this) {
                if (!isPregnant) {
                    isPregnant = true;
                    log("preg");
                }
            }
        }
    }

    // Food check
    if (field->getTile(x, y).type == terrainGenerator::TileType::grass) {
        changeHealthBy(grassHeal);
        hungrySteps = 0;
    } else {
        hungrySteps++;
    }

    // Custom manage function
    manage();

    return actions;
}

float Creature::animationSpeed = 0.1f;

int Creature::getPower() const { return power; }

void Creature::breed() {

}

terrainGenerator::Terrain *Creature::field = nullptr;




