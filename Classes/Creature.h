//
// Created by sju on 14.11.18.
//



#ifndef TEMPLATECPP_CREATURE_H
#define TEMPLATECPP_CREATURE_H

#include "cocos2d.h"
#include "terrainGenerator.h"


class Field;

class Creature : public cocos2d::Sprite {
public:
    CREATE_FUNC(Creature);

    virtual bool init();

    static Creature *
    createCreatureSprite(int x, int y, int tileSize, int n, std::string _type, std::string path,
                         terrainGenerator::Terrain &_field);

    virtual void onEnter();

    virtual void update(float dt) override;

    void moveBy(cocos2d::Vec2 vec, float secs);

    void moveTo(cocos2d::Vec2 vec, float secs);

    void setParrentTerrain(terrainGenerator::Terrain &_field);

    void setX(int _x);

    void setY(int _y);

    void setN(int _n);

    void setType(const std::string &_type);

    void setTileSize(int _tileSize);

    virtual void deathAnimation();

    void setPicture(std::string path);

    int getX() const;

    int getY() const;

    int getN() const;

    std::string getType();

    int getTitleSize() const;


    virtual void manage();

    void walk(int _x, int _y, terrainGenerator::Terrain &terrain);

private:
// Constructor private to make misuse of this class difficult.
    Creature();    // Constructor declared but defined in the cpp file.
    ~Creature() {}  // Destructor declared and defined here.

    int x, y;
    int tileSize;
    int n;
    std::string type = "human";
    int health = 100;
    int power = 1;
    terrainGenerator::Terrain *field = nullptr;
};


#endif //TEMPLATECPP_CREATURE_H
