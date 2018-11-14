//
// Created by sju on 12.11.18.
//

#ifndef TEMPLATECPP_FIELD_H
#define TEMPLATECPP_FIELD_H

#include "cocos2d.h"

#define MAX_SCALE 8.0
#define MIN_SCALE 0.05

class Field : public cocos2d::Node
{
public:
    CREATE_FUNC(Field);
    virtual bool init();
    virtual void onEnter();
    virtual void update(float dt) override;
    void moveBy(cocos2d::Vec2 vec, float secs);
    void scaleBy(float duration, float scaleFactor);

    void createField(int n);

private:
    // Constructor private to make misuse of this class difficult.
    Field();    // Constructor declared but defined in the cpp file.
    ~Field(){}  // Destructor declared and defined here.

    int tileSize = 60;
    float myFloatVar;
    int myIntVar;
    float currentScale = 1.0;
};


#endif //TEMPLATECPP_FIELD_H
