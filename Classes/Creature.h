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

    static Creature* createCreatureSprite(cocos2d::Vec2 position, std::string path);

    virtual void onEnter();

    virtual void update(float dt) override;

    void moveBy(cocos2d::Vec2 vec, float secs);

    void moveTo(cocos2d::Vec2 vec, float secs);


private:
// Constructor private to make misuse of this class difficult.
    Creature();    // Constructor declared but defined in the cpp file.
    ~Creature() {}  // Destructor declared and defined here.
};


#endif //TEMPLATECPP_CREATURE_H
