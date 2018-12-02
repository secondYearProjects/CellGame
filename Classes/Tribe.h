//
// Created by sju on 14.11.18.
//



#ifndef TEMPLATECPP_CREATURE_H
#define TEMPLATECPP_CREATURE_H

#include "cocos2d.h"
#include "terrainGenerator.h"
#include "Person.h"
#include "random.h"

#include <memory>


class Field;

struct CreatureActions {
    bool breed = false;
    bool fight = false;
    int fightDamage = 0;
};

class Tribe : public cocos2d::Sprite {
public:
    CREATE_FUNC(Tribe);

    virtual bool init();

    virtual void manage();

    static Tribe *
    createCreatureSprite(int x, int y, int tileSize, int n, std::string _type, std::string path);

    virtual void onEnter();

    virtual void update(float dt) override;

    void moveBy(cocos2d::Vec2 vec, float secs);

    void moveTo(cocos2d::Vec2 vec, float secs);

    void changeHealthBy(int value);

    void changeHealthTo(int value);

    virtual void deathAnimation();

    CreatureActions step();

    void walk(int _x, int _y);

    static void setAnimationSpeed(float speed) { Tribe::animationSpeed = speed; }

    static void setParrentTerrain(terrainGenerator::Terrain *_field) { Tribe::field = _field; }

    static terrainGenerator::Terrain *getParrentTerrain() { return Tribe::field; }

    int getX() const;

    int getY() const;

    int getN() const;

    int getHealth() const;

    int getPower() const;

    int peopleCount() const { return people.size(); }

    std::string getType();

    int getTitleSize() const;

    static void setTribeParams(int _startPeopleCount, int _startSpecialPoints, int _walkLimit) {
        Tribe::startPeopleCount = _startPeopleCount;
        Tribe::startSpecialPoints = _startSpecialPoints;
        Tribe::walkLimit = _walkLimit;
    }

protected:
    void feed(Person &person, std::size_t foodAmount);

    int x, y;
    int tileSize;
    int n;
    std::string type = "Obunga";
    int health = 100;
    int maxHealth = 100;

    int attackPower = 1;

    int food = 10;

    std::vector<Person> people;

    static int startPeopleCount;
    static int startSpecialPoints;

    static int walkLimit;

    static float animationSpeed;
    static terrainGenerator::Terrain *field;

private:
    cocos2d::Label *peopleLabel;

    Tribe();

    virtual ~Tribe() {}

    void setX(int _x);

    void setY(int _y);

    void setN(int _n);

    void setType(const std::string &_type);

    void setTileSize(int _tileSize);

    bool stepAvailable(int _x, int _y);

    void updateHealth();

    void updateAttack();
};


#endif //TEMPLATECPP_CREATURE_H
