/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __MainScene_H__
#define __MainScene_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Field.h"
#include "random.h"
#include "terrainGenerator.h"
//#include "ui/CocosGUI.h"

#include <map>

class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene *createScene();

    virtual bool init();

    virtual void update(float dt) override;


    void onWASD(cocos2d::EventKeyboard::KeyCode keyCode);

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);


private:
    std::vector<std::string> creatureTypes = {"Player", "Obunga", "Meskwaki"};

    static Field *field;
    static float tickTime;

    int fieldStartX;
    int fieldStartY;

    cocos2d::ui::Button *SpeedNormal = cocos2d::ui::Button::create("normal-speed.png", "normal-speed-dis.png");
    cocos2d::ui::Button *SpeedX2 = cocos2d::ui::Button::create("x2-speed.png", "x2-speed-dis.png");
    cocos2d::ui::Button *SpeedX3 = cocos2d::ui::Button::create("x3-speed.png", "x3-speed-dis.png");

    static std::map<cocos2d::EventKeyboard::KeyCode,
            std::chrono::high_resolution_clock::time_point> keys;

    bool isKeyPressed(cocos2d::EventKeyboard::KeyCode code);

    static void setTickTime(float val) {
        tickTime = val;
        Field::setAnimationSpeed(val * 0.5f);
    }

    static void setField(Field *_field) { field = _field; }

    void updateTickSchedule() {
        this->unscheduleUpdate();
        this->unschedule(schedule_selector(Field::gameStep));
        this->scheduleUpdate();
        this->schedule(schedule_selector(Field::gameStep), tickTime);

    }

    void initButtons();

    void buttonChecker(float val, cocos2d::ui::Widget::TouchEventType type);

    void setInactiveButtons();

};


#endif // __MainScene_H__
