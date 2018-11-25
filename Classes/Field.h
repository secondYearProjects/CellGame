//
// Created by sju on 12.11.18.
//

#ifndef TEMPLATECPP_FIELD_H
#define TEMPLATECPP_FIELD_H

#include "cocos2d.h"
#include "Tribe.h"
#include "terrainGenerator.h"

#include <vector>

#define MAX_SCALE 5.0
#define MIN_SCALE 0.05


class Field : public cocos2d::Node {
public:
    CREATE_FUNC(Field);

    virtual bool init();

    virtual void onEnter();

    virtual void update(float dt) override;

    //terrainGenerator::Terrain &getTerrain();

    void moveBy(cocos2d::Vec2 vec, float secs);

    void scaleBy(float duration, float scaleFactor);

    void gameStep(float dt);

    void createField(int n);

    void addCreature(int x, int y, const std::string &type);

    static void setAnimationSpeed(float val) {
        AnimationSpeed = val;
        Tribe::setAnimationSpeed(val);
    }

private:
    // Constructor private to make misuse of this class difficult.
    Field();    // Constructor declared but defined in the cpp file.
    ~Field() { delete terrain; }  // Destructor declared and defined here.

    int tileSize = 30;
    int n;
    float currentScale = 1.0;
    static terrainGenerator::Terrain *terrain;
    static std::vector<Tribe *> creatures;

    static float AnimationSpeed;

    static void setTerrain(terrainGenerator::Terrain *_terrain) { Field::terrain = _terrain; }

};


#endif //TEMPLATECPP_FIELD_H
