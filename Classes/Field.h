//
// Created by sju on 12.11.18.
//

#ifndef TEMPLATECPP_FIELD_H
#define TEMPLATECPP_FIELD_H

#include "cocos2d.h"


class Field : public cocos2d::Node
{
public:
    CREATE_FUNC(Field);
    virtual bool init();
    virtual void onEnter();
    void moveBy(cocos2d::Vec2 vec);

private:
    // Constructor private to make misuse of this class difficult.
    Field();    // Constructor declared but defined in the cpp file.
    ~Field(){}  // Destructor declared and defined here.
    void createField();

    float myFloatVar;
    int myIntVar;
};


#endif //TEMPLATECPP_FIELD_H
