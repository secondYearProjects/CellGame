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

    terrainGenerator::Terrain *newTerrain = new terrainGenerator::Terrain(n, Random::get(0, 2000), tileSize);

    Field::setTerrain(newTerrain);
    Tribe::setParrentTerrain(newTerrain);
    int lineChunks = n / chunkSize;

    // TODO

    int fieldX = this->getPositionX();
    int fieldY = this->getPositionY();

    int iPos = fieldY + (n - chunkSize) * tileSize;
    for (int i = 0; i < lineChunks; i++) {
        int jPos = 0;
        for (int j = 0; j < lineChunks; j++) {
            int chunkID = i + j * lineChunks;
            auto chunk = Sprite::create(
                    "Chunks/TextureChunks/fieldTexture" + std::to_string(chunkID) + ".png");

            chunk->setAnchorPoint(Vec2(0, 0));
            chunk->setScale(tileSize * chunkSize / chunk->getContentSize().width);
            chunk->setPosition(jPos, iPos);

            this->addChild(chunk);

            jPos += tileSize * chunkSize;
        }
        iPos -= tileSize * chunkSize;
    }

//    auto terrainImage = Sprite::create(terrain->getTextureName());
//    terrainImage->setAnchorPoint(Vec2(0, 0));
//    terrainImage->setScale(_n * tileSize / terrainImage->getContentSize().width);
//    terrainImage->setZOrder(-10);
//    this->addChild(terrainImage);


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

    auto newTribe = Tribe::createCreatureSprite(x % n, y % n, tileSize, n, type, "character.png");
    if (type == "Obunga")
        newTribe->setTexture("lizzard.png");

    newTribe->setScale(tileSize / newTribe->getContentSize().width);
    newTribe->setAnchorPoint(Vec2(0, 0));
    newTribe->setZOrder(1);

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
        if (action.fight) {
            //tribe->changeHealthBy(-action.fightDamage);
            // TODO: fight damage distributor
        }
    }

    tribes.erase(std::remove_if(tribes.begin(), tribes.end(), [=](Tribe *tribe) {
        if (tribe->peopleCount() == 0) {
            tribe->deathAnimation();
            return true;
        }
        return false;
    }), tribes.end());

}


//terrainGenerator::Terrain &Field::getTerrain() { return *terrain; }


std::vector<Tribe *> Field::tribes;

terrainGenerator::Terrain *Field::terrain = nullptr;

float Field::AnimationSpeed = 0.1f;

void Field::moveTo(cocos2d::Vec2 vec, float secs) {
    runAction(MoveTo::create(secs, vec));
}
