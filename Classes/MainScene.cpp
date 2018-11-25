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

#include "MainScene.h"


#include "SimpleAudioEngine.h"

#include "terrainGenerator.h"


USING_NS_CC;

Scene *MainScene::createScene() {
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainScene::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }

    setTickTime(0.1f);


    CCLayerColor *backGround = CCLayerColor::create(Color4B(255, 255, 255, 255));

    backGround->setPosition(Vec2(0, 0));

    Scene::addChild(backGround);

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, 8); // guaranteed unbiased
    std::uniform_int_distribution<int> booly(0, 1);
    setField(Field::create());
    field->createField(100);
    for (int i = 0; i < 80; i++) {
        if (booly(rd))
            field->addCreature(uni(rng), uni(rng), "lizzard");
        else
            field->addCreature(uni(rng), uni(rng), "human");
    }


    field->setAnchorPoint(Vec2(0.5, 0.5));


    Scene::addChild(field);


    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *event) {
        // If a key already exists, do nothing as it will already have a time stamp
        // Otherwise, set's the timestamp to now
        if (keys.find(keyCode) == keys.end()) {
            keys[keyCode] = std::chrono::high_resolution_clock::now();
        }
    };

    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);

    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event) {
        // remove the key.  std::map.erase() doesn't care if the key doesnt exist
        keys.erase(keyCode);
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);


    auto mouseEvent = EventListenerMouse::create();
    mouseEvent->onMouseScroll = [=](Event *event) {
        EventMouse *e = (EventMouse *) event;
        if (e->getScrollY() > 0)
            field->scaleBy(0.2, 0.6);
        else
            field->scaleBy(0.2, 1.4);
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent, this);


    this->scheduleUpdate();
    this->schedule(schedule_selector(Field::gameStep), tickTime);

    return true;
}

void MainScene::onWASD(cocos2d::EventKeyboard::KeyCode keyCode) {
    float time = 0.2;
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
            field->moveBy(Vec2(0, -10), time);
            break;
        case EventKeyboard::KeyCode::KEY_S:
            field->moveBy(Vec2(0, 10), time);
            break;
        case EventKeyboard::KeyCode::KEY_A:
            field->moveBy(Vec2(10, 0), time);
            break;
        case EventKeyboard::KeyCode::KEY_D:
            field->moveBy(Vec2(-10, 0), time);
            break;

        case EventKeyboard::KeyCode::KEY_EQUAL:
            field->scaleBy(time, 1.4);
            break;
        case EventKeyboard::KeyCode::KEY_MINUS:
            field->scaleBy(time, 0.6);
            break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            setTickTime(1000.0f);
            updateTickSchedule();
            break;
        case EventKeyboard::KeyCode::KEY_1:
            setTickTime(1.0);
            updateTickSchedule();
            break;
        case EventKeyboard::KeyCode::KEY_2:
            setTickTime(0.5);
            updateTickSchedule();
            break;
        case EventKeyboard::KeyCode::KEY_3:
            setTickTime(0.1);
            updateTickSchedule();
            break;
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
            break;

    }


}

void MainScene::update(float dt) {
    Node::update(dt);

    for (auto key:keys) {
        onWASD(key.first);
    }
}

bool MainScene::isKeyPressed(cocos2d::EventKeyboard::KeyCode code) {
    // Check if the key is currently pressed by seeing it it's in the std::map keys
    // In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
    if (keys.find(code) != keys.end())
        return true;
    return false;
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
exit(0);
#endif

/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

//EventCustom customEndEvent("game_scene_close_event");
//_eventDispatcher->dispatchEvent(&customEndEvent);


std::map<cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> MainScene::keys;

float MainScene::tickTime = 0.1f;

Field * MainScene::field = nullptr;

