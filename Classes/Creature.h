//
// Created by sju on 14.11.18.
//

#ifndef TEMPLATECPP_CREATURE_H
#define TEMPLATECPP_CREATURE_H

#include "cocos2d.h"


class Creature : public cocos2d::Sprite {
public:
    CREATE_FUNC(Creature);

    virtual bool init();

    static Creature *createCreatureSprite(int x, int y, int tileSize, int n, std::string path);

    virtual void onEnter();

    virtual void update(float dt) override;

    void moveBy(cocos2d::Vec2 vec, float secs);

    void moveTo(cocos2d::Vec2 vec, float secs);

    void setX(int _x) { x = _x; }

    void setY(int _y) { y = _y; }

    void setN(int _n) { n = _n; }

    void setTielSize(int _tileSize) { tileSize = _tileSize; }

    virtual void manage();

    void walk(int _x, int _y);

private:
// Constructor private to make misuse of this class difficult.
    Creature();    // Constructor declared but defined in the cpp file.
    ~Creature() {}  // Destructor declared and defined here.

    int x, y;
    int tileSize;
    int n;
    int health = 100;
};


#endif //TEMPLATECPP_CREATURE_H
