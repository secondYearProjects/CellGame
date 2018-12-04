#include <utility>

//
// Created by sju on 12.11.18.
//

#include "Field.h"


USING_NS_CC;

using Random = effolkronium::random_static;

Field::Field() {
}

bool Field::init() {
    if (!Node::init())
        return false;

    return true;
}

void Field::createField(int _n) {
    this->n = _n;

    terrainGenerator::Terrain *newTerrain = nullptr;

    std::thread generationThread([&]() {
        newTerrain = new terrainGenerator::Terrain(n, Random::get(0, 2000), tileSize);
    });

    if (generationThread.joinable())
        generationThread.join();

    Field::setTerrain(newTerrain);
    Tribe::setParrentTerrain(newTerrain);
    int lineChunks = n / chunkSize;

    int fieldX = this->getPositionX();
    int fieldY = this->getPositionY();

    int iPos = fieldY + (n - chunkSize) * tileSize;
    for (int i = 0; i < lineChunks; i++) {
        int jPos = 0;
        for (int j = 0; j < lineChunks; j++) {
            int chunkID = j + i * lineChunks;
            auto chunk = Sprite::create(
                    "Chunks/TextureChunks/fieldTexture" + std::to_string(chunkID) + ".png");

            chunk->setAnchorPoint(Vec2(0, 0));
            chunk->setScale(tileSize * chunkSize / chunk->getContentSize().width);
            chunk->setPosition(jPos, iPos);

            this->addChild(chunk);
            chunk->setGlobalZOrder(0);

            jPos += tileSize * chunkSize;
        }
        iPos -= tileSize * chunkSize;
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
    float fieldScale = this->getScale();
    if (fieldScale * scaleFactor < MAX_SCALE && fieldScale * scaleFactor > MIN_SCALE) {
        runAction(ScaleBy::create(duration, scaleFactor));
    }
}

void Field::addTribe(int x, int y, const std::string &type) {

    auto newTribe = Tribe::createCreatureSprite(x % n, y % n, tileSize, n, type, "red.png");

    if (type == "Player")
        newTribe->setTexture("red.png");
    else if (type == "Obunga")
        newTribe->setTexture("blue.png");
    else if (type == "Meskwaki")
        newTribe->setTexture("green.png");

    newTribe->setScale(tileSize / newTribe->getContentSize().width);
    newTribe->setAnchorPoint(Vec2(0, 0));
    newTribe->setGlobalZOrder(1);

    addChild(newTribe);

    tribes.push_back(newTribe);
    terrain->addCreature(x, y, newTribe);
}

void Field::gameStep(float dt) {
    tribes.erase(std::remove_if(tribes.begin(), tribes.end(), [=](Tribe *tribe) {
        if (tribe->peopleCount() == 0) {
            tribe->deathAnimation();
            return true;
        }
        return false;
    }), tribes.end());


    for (auto &tribe : tribes) {
        CreatureActions action = tribe->step();
        //log("recieve %i food", tribe->cannibalValue());
    }

    // FIGHTS CHECK
    std::vector<std::pair<Tribe *, std::vector<Tribe *> > > fights;
    for (auto &tribe:tribes) {
        CreatureActions action = tribe->fightCheck();
        if (action.fight) {
            fights.emplace_back(tribe, action.fightWith);
        }
    }
    // RUN FIGHTS
    for (auto &fightPair:fights) {
        Tribe *fighter = fightPair.first;


        fighter->fightAnimation();

        for (auto &aim:fightPair.second) {
            aim->distributeDamage(fighter->attack());
            aim->updateHealth();
            //log("fight");
            if (aim->getHealth() <= 0) {
                log("death from fight! recieve %i food", aim->cannibalValue());
                fighter->addFood(aim->cannibalValue());
            }
        }
    }

    tribes.erase(std::remove_if(tribes.begin(), tribes.end(), [=](Tribe *tribe) {
        tribe->updateHealth();
        if (tribe->getHealth() <= 0) {
            tribe->deathAnimation();
            return true;
        }
        return false;
    }), tribes.end());

    cocos2d::EventCustom tickInfoUpdate("tickUpdate");

    _eventDispatcher->dispatchEvent(&tickInfoUpdate);

}


//terrainGenerator::Terrain &Field::getTerrain() { return *terrain; }


void Field::moveTo(cocos2d::Vec2 vec, float secs) {
    runAction(MoveTo::create(secs, vec));
}

terrainGenerator::TileType Field::getTileType(int _x, int _y) {
    if (_x >= 0 && _x < n) {
        if (_y > 0 && _y < n) {
            return terrain->getTile(_x, _y).type;
        } else
            return terrainGenerator::TileType::water;
    } else
        return terrainGenerator::TileType::water;
}

std::vector<Tribe *> Field::tribes;

terrainGenerator::Terrain *Field::terrain = nullptr;

float Field::AnimationSpeed = 0.1f;

std::map<std::string, std::string> Field::getFactionsReview() {
    std::map<std::string, std::string> review;

    std::map<std::string, int> peopleCount;

    for (auto &tribe:tribes) {
        peopleCount[tribe->getType()] += tribe->peopleCount();
    }

    for (auto &type:creatureTypes) {
        review[type] = std::to_string(peopleCount[type]);
    }

    return review;
}
