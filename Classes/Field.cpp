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
    /*
    std::pair<std::string, std::string> pathes[] = {std::make_pair("grass", "grass.png"),
                                                    std::make_pair("dirt", "dirt.png"),
                                                    std::make_pair("water", "water.png")};
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, 2); // guaranteed unbiased
    for (int i = 0; i < _n; i++) {
        for (int j = 0; j < _n; j++) {
            int tmp = uni(rd);
            auto tileSprite = TileSprite::createTileSprite(i, j, tileSize, _n, pathes[tmp].first, pathes[tmp].second);
            //auto spriteTmp = Sprite::create("grass.png");

            this->addChild(tileSprite);
            tileSprite->setZOrder(-10);
            tileSprite->setAnchorPoint(Vec2(0, 0));
            //tileSprite->setPosition(Vec2(j * tileSize, i * tileSize));
            tileSprite->setScale(tileSize / tileSprite->getContentSize().width);
        }
    }
     */

    terrain = new terrainGenerator::Terrain(n, 1337, tileSize);
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
    //if (currentScale * scaleFactor < MAX_SCALE && currentScale * scaleFactor > MIN_SCALE) {
    runAction(ScaleBy::create(duration, scaleFactor));
    currentScale *= scaleFactor;
    //}
}

void Field::addCreature(int x, int y, const std::string &type) {

    auto newCreature = Creature::createCreatureSprite(x % n, y % n, tileSize, n, type, "character.png", *terrain);

    if (type == "lizzard")
        newCreature->setTexture("lizzard.png");

    newCreature->setScale(tileSize / newCreature->getContentSize().width);
    newCreature->setAnchorPoint(Vec2(0, 0));

    addChild(newCreature);
    creatures.push_back(newCreature);
    terrain->addCreature(x, y, newCreature);
}

Creature *Field::bornCreature(int x, int y, const std::string &type) {
    auto newCreature = Creature::createCreatureSprite(x % n, y % n, tileSize, n, type, "character.png", *terrain);
    if (type == "lizzard")
        newCreature->setTexture("lizzard.png");

    newCreature->setScale(tileSize / newCreature->getContentSize().width);
    newCreature->setAnchorPoint(Vec2(0, 0));

    addChild(newCreature);

    return newCreature;
}

void Field::gameStep(float dt) {
    creatures.erase(std::remove_if(creatures.begin(), creatures.end(), [=](Creature *creature) {
        //if(terrain->getTile(creature->getX(),creature->getY()).creatures.size()>1)
        if (creature->getHealth() < 0) {
            creature->deathAnimation();
            return true;
        }
        return false;
    }), creatures.end());

    std::vector<Creature *> newBorn;

    for (int i = 0; i < creatures.size(); i++) {
        auto &creature = creatures[i];

        CreatureActions action = creature->step();
        if (action.breed) {
            log("breed %s", creature->getType().c_str());
            newBorn.push_back(bornCreature(creature->getX(), creature->getY(), creature->getType()));

        }
        if (action.fight) {
            creature->changeHealthBy(-action.fightDamage);
        }
    }

    creatures.erase(std::remove_if(creatures.begin(), creatures.end(), [=](Creature *creature) {
        //if(terrain->getTile(creature->getX(),creature->getY()).creatures.size()>1)
        if (creature->getHealth() < 0) {
            creature->deathAnimation();
            return true;
        }
        return false;
    }), creatures.end());

    for (int i = 0; i < newBorn.size(); i++) {

        creatures.push_back(newBorn[i]);
        terrain->terrainMap[newBorn[i]->getX()][newBorn[i]->getY()].creatures.push_back(newBorn[i]);
        //terrain->breedCreature(newBorn[i]->getX(), newBorn[i]->getY(), newBorn[i]);

    }
}


terrainGenerator::Terrain &Field::getTerrain() { return *terrain; }


std::vector<Creature *> Field::creatures;

