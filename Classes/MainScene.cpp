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

using Random = effolkronium::random_static;

Scene *MainScene::createScene() {
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MainScene::init() {
    // 1. Scene context init first
    if (!Scene::init()) {
        return false;
    }

    setTickTime(0.5f);

    auto backGround = cocos2d::LayerColor::create(Color4B(255, 255, 255, 255));

    backGround->setPosition(Vec2(0, 0));
    backGround->setGlobalZOrder(-10);

    Scene::addChild(backGround);

    int n = 50;

    setField(Field::create());
    field->createField(n);

    for (auto &creatureType:creatureTypes) {
        for (int i = 0; i < 8; i++) {
            for (int tryCount = 0; tryCount < 100; tryCount++) {
                int xSpawn = Random::get(0, field->getN() - 1);
                int ySpawn = Random::get(0, field->getN() - 1);
                if (field->getTileType(xSpawn, ySpawn) != terrainGenerator::TileType::water) {
                    field->addTribe(xSpawn, ySpawn, creatureType);
                    break;
                }
            }
        }
    }

    field->setAnchorPoint(Vec2(0.5, 0.5));


    Scene::addChild(field);


    auto eventListener = EventListenerKeyboard::create();
    eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event) {
        // If a key already exists, do nothing as it will already have a time stamp
        // Otherwise, set's the timestamp to now
        if (keys.find(keyCode) == keys.end()) {
            keys[keyCode] = std::chrono::high_resolution_clock::now();
        }
    };

    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);

    eventListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event *event) {
        // remove the key.  std::map.erase() doesn't care if the key doesnt exist
        keys.erase(keyCode);
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    auto TribeInfoUpdate = EventListenerCustom::create("updateInfo",[=](EventCustom *event){
        TribeInfoLabel->setString(static_cast<char*>(event->getUserData()));
    });////////here

    _eventDispatcher->addEventListenerWithSceneGraphPriority(TribeInfoUpdate, this);


    auto mouseEvent = EventListenerMouse::create();
    mouseEvent->onMouseScroll = [&](Event *event) {
        EventMouse *e = (EventMouse *) event;
        if (e->getScrollY() > 0)
            field->scaleBy(0.2, 0.6);
        else
            field->scaleBy(0.2, 1.4);
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseEvent, this);

    this->scheduleUpdate();
    this->schedule(schedule_selector(Field::gameStep), tickTime);

    initButtons();

    auto tribeInfoBox = cocos2d::Sprite::create("uiBox.png");
    tribeInfoBox->setAnchorPoint(Vec2(0, 0));
    tribeInfoBox->setScale(300 / tribeInfoBox->getContentSize().width,
                           800 / tribeInfoBox->getContentSize().height);
    tribeInfoBox->setPosition(Scene::getPositionX() + 800, Scene::getPositionY());
    addChild(tribeInfoBox);
    tribeInfoBox->setGlobalZOrder(10);


    TribeInfoLabel = cocos2d::Label::createWithSystemFont("Tribe Info: ", "Arial", 22);
    TribeInfoLabel->setColor(Color3B::BLACK);
    TribeInfoLabel->setAnchorPoint(Vec2(0, 1));
    TribeInfoLabel->setWidth(180);
    addChild(TribeInfoLabel);
    TribeInfoLabel->setGlobalZOrder(11);
    TribeInfoLabel->setPosition(
            tribeInfoBox->getPositionX() + 10,
            tribeInfoBox->getPositionY() + 690);

    fieldStartX = field->getPositionX();
    fieldStartY = field->getPositionY();

    return true;
}

void MainScene::onWASD(cocos2d::EventKeyboard::KeyCode keyCode) {
    float time = 0.2;
    float fieldScale = field->getScale();
    fieldScale = fieldScale > 1.0 ? fieldScale : 2.0f;

    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_W:

            field->moveBy(Vec2(0, -10 * fieldScale), time);
            break;
        case EventKeyboard::KeyCode::KEY_S:
            field->moveBy(Vec2(0, 10 * fieldScale), time);
            break;
        case EventKeyboard::KeyCode::KEY_A:
            field->moveBy(Vec2(10 * fieldScale, 0), time);
            break;
        case EventKeyboard::KeyCode::KEY_D:
            field->moveBy(Vec2(-10 * fieldScale, 0), time);
            break;

        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            field->moveBy(Vec2(0, -10 * fieldScale), time);
            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            field->moveBy(Vec2(0, 10 * fieldScale), time);
            break;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            field->moveBy(Vec2(10 * fieldScale, 0), time);
            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            field->moveBy(Vec2(-10 * fieldScale, 0), time);
            break;

//        case EventKeyboard::KeyCode::KEY_Q:
//            field->moveTo(Vec2(fieldStartX, fieldStartY), time);
//            break;

        case EventKeyboard::KeyCode::KEY_EQUAL:
            field->scaleBy(time, 1.4);
            break;
        case EventKeyboard::KeyCode::KEY_MINUS:
            field->scaleBy(time, 0.6);
            break;

        case EventKeyboard::KeyCode::KEY_SPACE:
            setTickTime(10000.0f);
            updateTickSchedule();
            setInactiveButtons();
            break;
        case EventKeyboard::KeyCode::KEY_1:
            setTickTime(0.5f);
            updateTickSchedule();
            setInactiveButtons();
            SpeedNormal->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(255, 255, 255)));
            break;
        case EventKeyboard::KeyCode::KEY_2:
            setTickTime(0.3f);
            updateTickSchedule();
            setInactiveButtons();
            SpeedX2->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(255, 255, 255)));
            break;
        case EventKeyboard::KeyCode::KEY_3:
            setTickTime(0.1f);
            updateTickSchedule();
            setInactiveButtons();
            SpeedX3->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(255, 255, 255)));
            break;

        case EventKeyboard::KeyCode::KEY_ESCAPE:
            Director::getInstance()->end();
        default:
            break;

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
    if (keys.find(code) != keys.end())
        return true;
    return false;
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
exit(0);
#endif

void MainScene::setInactiveButtons() {
    SpeedNormal->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(200, 200, 200)));
    SpeedX2->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(200, 200, 200)));
    SpeedX3->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(200, 200, 200)));
}

void MainScene::buttonChecker(float val, cocos2d::ui::Widget::TouchEventType type) {
    setInactiveButtons();
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            setTickTime(val);
            updateTickSchedule();
            break;
        default:
            break;
    }
}

void MainScene::initButtons() {
    int buttonSize = 40;

    SpeedNormal->setAnchorPoint(Vec2(0, 0));
    SpeedX2->setAnchorPoint(Vec2(0, 0));
    SpeedX3->setAnchorPoint(Vec2(0, 0));

    SpeedNormal->setScale(buttonSize / SpeedNormal->getContentSize().width);
    SpeedX2->setScale(buttonSize / SpeedX2->getContentSize().width);
    SpeedX3->setScale(buttonSize / SpeedX3->getContentSize().width);

    float buttonX = this->getPositionX() + this->getContentSize().width - 40;
    float buttonY = this->getPositionY() + 20;

    SpeedNormal->setPosition(Vec2(buttonX - buttonSize * 3, buttonY));
    SpeedX2->setPosition(Vec2(buttonX - buttonSize * 2, buttonY));
    SpeedX3->setPosition(Vec2(buttonX - buttonSize * 1, buttonY));

    SpeedNormal->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(255, 255, 255)));
    SpeedX2->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(200, 200, 200)));
    SpeedX3->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(200, 200, 200)));

    SpeedNormal->addTouchEventListener([&](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        buttonChecker(0.5f, type);
        SpeedNormal->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(255, 255, 255)));
    });
    SpeedX2->addTouchEventListener([&](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        buttonChecker(0.3f, type);
        SpeedX2->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(255, 255, 255)));
    });
    SpeedX3->addTouchEventListener([&](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        buttonChecker(0.1f, type);
        SpeedX3->runAction(cocos2d::TintTo::create(0, cocos2d::Color3B(255, 255, 255)));
    });

    this->addChild(SpeedNormal);
    this->addChild(SpeedX2);
    this->addChild(SpeedX3);

    SpeedNormal->setGlobalZOrder(20);
    SpeedX2->setGlobalZOrder(20);
    SpeedX3->setGlobalZOrder(20);
}

std::map<cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> MainScene::keys;

float MainScene::tickTime = 0.1f;

Field *MainScene::field = nullptr;