//
// Created by sju on 12.11.18.
//

#ifndef TEMPLATECPP_FIELD_H
#define TEMPLATECPP_FIELD_H

#include "cocos2d.h"
#include "Tribe.h"
#include "terrainGenerator.h"
#include "random.h"

#include <vector>
#include <string>

#define MAX_SCALE 3.11
#define MIN_SCALE 0.52


class Field : public cocos2d::Node {
public:
    CREATE_FUNC(Field);

    virtual bool init();

    virtual void onEnter();

    virtual void update(float dt) override;

    //terrainGenerator::Terrain &getTerrain();

    void moveBy(cocos2d::Vec2 vec, float secs);

    void moveTo(cocos2d::Vec2 vec, float secs);

    void scaleBy(float duration, float scaleFactor);

    void gameStep(float dt);

    void createField(int n);

    terrainGenerator::TileType getTileType(int _x, int _y);

    void addTribe(int x, int y, const std::string &type);

    static void setAnimationSpeed(float val) {
        AnimationSpeed = val;
        Tribe::setAnimationSpeed(val);
    }

    int getN() const { return n; }

private:

    Field();

    ~Field() { delete terrain; }

    int tileSize = 20;
    int n;
    float currentScale = 1.0;
    static terrainGenerator::Terrain *terrain;
    static std::vector<Tribe *> tribes;

    static const int chunkSize = 10;

    static float AnimationSpeed;

    static void setTerrain(terrainGenerator::Terrain *_terrain) { Field::terrain = _terrain; }

};


#endif //TEMPLATECPP_FIELD_H
