//
// Created by sju on 14.11.18.
//

#include "Tribe.h"

#include <random>
#include <ctime>

#include <iostream>

USING_NS_CC;

using Random = effolkronium::random_static;

Tribe::Tribe() {
    people.reserve(startPeopleCount);

    auto mouseEvent = EventListenerTouchOneByOne::create();
    mouseEvent->onTouchBegan = [=](Touch *touch, Event *event) {
        cocos2d::Vec2 p = touch->getLocation();
        cocos2d::Rect rect = this->getBoundingBox();
        if (rect.containsPoint(p)) {
            // so you touched the Sprite, do something about it
            log("touched %s", type.c_str());
            return true;
        }
        return false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent, this);


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

    updateHealth();

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

    peopleLabel = cocos2d::Label::createWithSystemFont("5", "Arial", 24);
    peopleLabel->setTextColor(cocos2d::Color4B(20, 20, 20, 255));
    peopleLabel->setAnchorPoint(Vec2(-1.95f, -0.6f));
    peopleLabel->setAlignment(TextHAlignment::CENTER);
    peopleLabel->setString(std::to_string(peopleCount()));
    addChild(peopleLabel);

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
    if (std::abs(_x) > walkLimit || std::abs(_y) > walkLimit)
        return;

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
    log("%s", type.c_str());
    log("food %i", food);
    for (auto &person:people) {
        //if (person.hunger < 0 || person.hunger < Person::hungerPerStep + 2) {
        if (person.getHunger() < 10)
            feed(person, 5);
        person.stats();
    }
    log("food %i", food);
    std::cout << std::endl;
    //if (field->getTile(x, y).type == terrainGenerator::TileType::grass)
    //    return;

    int _x = Random::get<int>(-walkLimit, walkLimit);
    int _y = Random::get<int>(-walkLimit, walkLimit);
    walk(_x, _y);

}

void Tribe::deathAnimation() {
    setTexture("deadChar.png");
    peopleLabel->setString(std::to_string(peopleCount()));
    removeAllChildren();
    field->removeCreature(x, y, this);

    auto fade = FadeOut::create(animationSpeed * 10.0f);
    auto seq = Sequence::create(fade, [=] { this->removeFromParent(); }, nullptr);
    runAction(seq);
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

void Tribe::changeHealthTo(int value) {
    health = value;
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

    for (auto &person:people) {
        person.step();
    }

    updateHealth();

    // Fight & Breed
//    if (!field->getTile(x, y).tribes.empty()) {
//        for (int i = 0; i < field->getTile(x, y).tribes.size(); i++) {
//            auto &enemy = field->getTile(x, y).tribes[i];
//
//            if (enemy->getType() != this->getType()) {
//                actions.fight = true;
//                actions.fightDamage += enemy->getPower();
//                //log("fight");
//            }
//        }
//    }

    // Food check
    if (field->getTile(x, y).type == terrainGenerator::TileType::grass) {
        food += people.size() * 3;
    }

    people.erase(std::remove_if(people.begin(), people.end(), [=](Person &person) {
        return (person.health <= 0);
    }), people.end());
    updateHealth();
    //updateAttack();
    peopleLabel->setString(std::to_string(peopleCount()));

    // Custom manage function
    manage();

    return actions;
}

float Tribe::animationSpeed = 0.1f;

int Tribe::getPower() const { return attackPower; }

terrainGenerator::Terrain *Tribe::field = nullptr;

void Tribe::updateHealth() {
    int newHealth = 0;
    int newMaxHealth = 0;
    for (auto &person:people) {
        if (person.health > 0) {
            newHealth += person.health;
            newMaxHealth += person.maxHealth;
        }
    }
    maxHealth = newMaxHealth;
    changeHealthTo(newHealth);
}

void Tribe::updateAttack() {
    int newAttack = 0;
    for (auto &person:people) {
        if (person.health > 0) {
            newAttack += person.attack();
        }
    }
    attackPower = newAttack;
}


int Tribe::startPeopleCount = 5;
int Tribe::startSpecialPoints = 10;

int Tribe::walkLimit = 1;

void Tribe::feed(Person &person, std::size_t foodAmount) {
    if (food >= foodAmount) {
        person.eat(foodAmount);
        food -= foodAmount;
    } else {
        person.eat(food);
        food = 0;
    }
}


