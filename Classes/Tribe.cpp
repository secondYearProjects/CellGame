//
// Created by sju on 14.11.18.
//

#include "Tribe.h"

#include <random>
#include <ctime>

#include <iostream>

USING_NS_CC;


Tribe::Tribe() {
    people.reserve(startPeopleCount);
    for (int i = 0; i < startPeopleCount; i++) {
        people.emplace_back(Person(startSpecialPoints));
    }

    // to breed
    bool hasFemale = false;
    for (auto &person:people) {
        if (!person.male) {
            hasFemale = true;
            break;
        }
    }
    if (!hasFemale) {
        people[0].male = false;
    }

    for (auto person:people) {
        std::cout << person.name << ": "
                  << person.attributes.Strength << " "
                  << person.attributes.Perception << " "
                  << person.attributes.Endurance << " "
                  << person.attributes.Charisma << " "
                  << person.attributes.Intelligence << " "
                  << person.attributes.Agility << " "
                  << person.attributes.Luck << "\n";
    }
    std::cout << std::endl;
}

bool Tribe::init() {
    if (!Sprite::init())
        return false;
    return true;
}

void Tribe::onEnter() {
    Node::onEnter();
}

void Tribe::moveBy(Vec2 vec, float secs) {
    runAction(MoveBy::create(secs, vec));

}

void Tribe::update(float dt) {
    Node::update(dt);
}

void Tribe::moveTo(cocos2d::Vec2 vec, float secs) {
    runAction(MoveBy::create(secs, vec));

}

Tribe *Tribe::createCreatureSprite(int _x, int _y, int _tileSize, int _n, std::string _type, std::string path) {
    auto tribe = new Tribe();

    if (tribe && tribe->initWithFile(path)) {
        tribe->setX(_x);
        tribe->setY(_y);
        tribe->setTileSize(_tileSize);
        tribe->setN(_n);
        tribe->setType(_type);

        tribe->autorelease();
        tribe->setPosition(Vec2(_x * _tileSize, _y * _tileSize));
        return tribe;
    }

    return nullptr;
}

void Tribe::walk(int _x, int _y) {
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

void Tribe::manage() {
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(-1, 1); // guaranteed unbiased

    int _x = uni(rng);
    int _y = uni(rng);
    walk(_x, _y);

}

void Tribe::deathAnimation() {
    setTexture("deadChar.png");

    runAction(FadeOut::create(animationSpeed * 10.0f));
    field->removeCreature(x, y, this);
}

void Tribe::setX(int _x) { x = _x; }

void Tribe::setY(int _y) { y = _y; }

void Tribe::setN(int _n) { n = _n; }

void Tribe::setType(const std::string &_type) { type = _type; }

void Tribe::setTileSize(int _tileSize) { tileSize = _tileSize; }

int Tribe::getX() const { return x; }

int Tribe::getY() const { return y; }

int Tribe::getN() const { return n; }

std::string Tribe::getType() { return type; }

int Tribe::getTitleSize() const { return tileSize; }


bool Tribe::stepAvailable(int _x, int _y) {
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

int Tribe::getHealth() const { return health; }

void Tribe::changeHealthBy(int value) {
    health = (value + health);
    if (health > maxHealth)
        health = maxHealth;

    auto tintTo = TintTo::create(animationSpeed, 255,
                                 254.0f * (static_cast<float >(health) / static_cast<float >(maxHealth)),
                                 254.0f * (static_cast<float >(health) / static_cast<float >(maxHealth)));
    this->runAction(tintTo);
}

CreatureActions Tribe::step() {
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
    if (!field->getTile(x, y).tribes.empty()) {
        for (int i = 0; i < field->getTile(x, y).tribes.size(); i++) {
            auto &enemy = field->getTile(x, y).tribes[i];

            if (enemy->getType() != this->getType()) {
                actions.fight = true;
                actions.fightDamage += enemy->getPower();
                //log("fight");
            } else if (enemy->getType() == this->getType() && enemy != this) {
                if (!isPregnant) {
                    isPregnant = true;
                    //log("preg");
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

float Tribe::animationSpeed = 0.1f;

int Tribe::getPower() const { return power; }

void Tribe::breed() {

}

terrainGenerator::Terrain *Tribe::field = nullptr;




