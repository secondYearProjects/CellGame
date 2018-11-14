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

    CCLayerColor *backGround = CCLayerColor::create(Color4B(255, 255, 255, 255));

    backGround->setPosition(Vec2(0, 0));

    Scene::addChild(backGround);


    field = Field::create();
    field->setAnchorPoint(Vec2(0, 0));


    Scene::addChild(field);


    //field->moveBy(Vec2(100,100));

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(MainScene::onWASD, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();

    return true;
}

void MainScene::onWASD(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:
            field->moveBy(Vec2(0, 20));
            auto listener = EventListenerKeyboard::create();
            listener->onKeyPressed = CC_CALLBACK_2(MainScene::onWASD, this);

            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
            break;
    }


}

void MainScene::update(float dt) {

}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
exit(0);
#endif

/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

//EventCustom customEndEvent("game_scene_close_event");
//_eventDispatcher->dispatchEvent(&customEndEvent);


