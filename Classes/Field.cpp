#include <utility>

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

    terrainGenerator::Terrain *newTerrain = new terrainGenerator::Terrain(n, 1337, tileSize);

    Field::setTerrain(newTerrain);
    Tribe::setParrentTerrain(newTerrain);

    auto terrainImage = Sprite::create(terrain->getTextureName());
    terrainImage->setAnchorPoint(Vec2(0, 0));
    terrainImage->setScale(_n * tileSize / terrainImage->getContentSize().width);
    terrainImage->setZOrder(-10);
    this->addChild(terrainImage);


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
    float fieldScale = this->getScale();
    if (fieldScale * scaleFactor < MAX_SCALE && fieldScale * scaleFactor > MIN_SCALE) {
        runAction(ScaleBy::create(duration, scaleFactor));
    }
}

void Field::addCreature(int x, int y, const std::string &type) {

    auto newCreature = Tribe::createCreatureSprite(x % n, y % n, tileSize, n, type, "character.png");
    if (type == "Obunga")
        newCreature->setTexture("lizzard.png");

    newCreature->setScale(tileSize / newCreature->getContentSize().width);
    newCreature->setAnchorPoint(Vec2(0, 0));
    newCreature->setZOrder(1);

    addChild(newCreature);

    creatures.push_back(newCreature);
    terrain->addCreature(x, y, newCreature);
}

void Field::gameStep(float dt) {
    creatures.erase(std::remove_if(creatures.begin(), creatures.end(), [=](Tribe *creature) {
        //if(terrain->getTile(creature->getX(),creature->getY()).creatures.size()>1)
        if (creature->getHealth() < 0) {
            creature->deathAnimation();
            return true;
        }
        return false;
    }), creatures.end());

    struct BreedStruct {
        BreedStruct(int _x, int _y, std::string _type) : x(_x), y(_y), type(std::move(_type)) {}

        int x;
        int y;
        std::string type;
    };

    std::vector<BreedStruct> newBorn;

    for (int i = 0; i < creatures.size(); i++) {
        auto &creature = creatures[i];

        CreatureActions action = creature->step();
        if (action.fight) {
            creature->changeHealthBy(-action.fightDamage);
        }
    }

    creatures.erase(std::remove_if(creatures.begin(), creatures.end(), [=](Tribe *creature) {
        //if(terrain->getTile(creature->getX(),creature->getY()).creatures.size()>1)
        if (creature->getHealth() < 0) {
            creature->deathAnimation();
            return true;
        }
        return false;
    }), creatures.end());

}


//terrainGenerator::Terrain &Field::getTerrain() { return *terrain; }


std::vector<Tribe *> Field::creatures;

terrainGenerator::Terrain *Field::terrain = nullptr;

float Field::AnimationSpeed = 0.1f;