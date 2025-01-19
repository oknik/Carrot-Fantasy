#include"audio/include/AudioEngine.h"
#include"Data.h"
#include"HelloWorldScene.h"
#include"MapSelectionScene.h"
#include"FirstMapScene.h"
#include"SecondMapScene.h"
#include"Object.h"
#include"Tower.h"
#include<vector>
USING_NS_CC;
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
cocos2d::Scene* SecondMapScene::createScene() {
    auto scene = SecondMapScene::create();
    secondMoney = 200;
    return scene;
}
cocos2d::Label* SecondMapScene::createRoundLabel() {
    RoundLabel = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d      10 rounds", round), "fonts/Marker Felt.ttf", 24);
    if (!RoundLabel) {
        CCLOG("Error creating HP Label");
    }
    return RoundLabel;
}
void SecondMapScene::updateRoundLabel(float dt) {
    if (RoundLabel) {
        RoundLabel->setString(cocos2d::StringUtils::format("%d      10 rounds", round));
    }

}
cocos2d::Label* SecondMapScene::createMoneyLabel() {
    MoneyLabel = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", secondMoney), "fonts/Marker Felt.ttf", 24);
    if (!MoneyLabel) {
        CCLOG("Error creating HP Label");
    }
    return MoneyLabel;
}
void SecondMapScene::updateMoneyLabel(float dt) {
    if (MoneyLabel) {
        MoneyLabel->setString(cocos2d::StringUtils::format("%d", secondMoney));
    }

}

void SecondMapScene::backToMain(cocos2d::Ref* sender) {
    int selectId = cocos2d::AudioEngine::play2d("test/Select.mp3", false);//这里的true or false是循环的意思
    this->schedule([=](float dt) {
        if (cocos2d::AudioEngine::getState(selectId) != cocos2d::AudioEngine::AudioState::PLAYING) {
            cocos2d::AudioEngine::stopAll();
            this->unschedule("checkAudioFinished");
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.0f, HelloWorld::createScene()));// 在音频播放完毕后执行切换场景的操作
        }
        }, "checkAudioFinished");
}
void SecondMapScene::pause(cocos2d::Ref* sender) {
    if (Director::getInstance()->isPaused()) {
        Director::getInstance()->resume(); // 恢复场景的运行
    }
    else {
        Director::getInstance()->pause(); // 暂停场景的运行
    }
}
void SecondMapScene::removeAllMonster() {
    auto children = this->getChildren();
    for (auto child : children) {
        auto monster = dynamic_cast<Monster*>(child);
        if (monster) {
            this->removeChild(monster);
        }
    }
}
void SecondMapScene::removeAllTower() {
    Vector<Node*> children = this->getChildren();
    for (const auto& child : children) {
        if (child->getTag() == 100) {
            this->removeChild(child);
        }
    }
}

void SecondMapScene::secondTryAgain(cocos2d::Ref* sender) {
    cocos2d::AudioEngine::play2d("test/Select.mp3", false);
    carrot->hp = 10;
    secondMoney = 200;
    round = 1;
    win = 0;
    gridMap[0][4] = gridMap[0][5] = gridMap[0][6] = gridMap[0][7] = gridMap[0][8] = gridMap[0][9] = gridMap[0][10] = _YES;
    gridMap[1][2] = gridMap[1][3] = gridMap[1][10] = gridMap[1][12] = gridMap[1][13] = _YES;
    gridMap[2][2] = gridMap[2][5] = gridMap[2][14] = _YES;
    gridMap[3][7] = gridMap[3][11] = gridMap[3][14] = _YES;
    gridMap[4][2] = gridMap[4][13] = _YES;
    gridMap[5][7] = gridMap[5][10] = gridMap[5][11] = gridMap[5][13] = gridMap[5][14];
    gridMap[6][2] = gridMap[6][5] = gridMap[6][8] = gridMap[6][9] = gridMap[6][13] = gridMap[6][14] = _YES;
    gridMap[7][2] = gridMap[7][5] = gridMap[7][8] = gridMap[7][9] = gridMap[7][13] = gridMap[7][14] = _YES;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 17; j++) {
            gridTowerMap[i][j] = nullptr;
        }
    }
    this->removeChildByName("FailSprite");
    this->removeChildByName("WinSprite");
    this->removeChildByName("TryAgainMenu");
    this->removeChildByName("SelectMenu");
    this->removeAllMonster();
    this->removeAllTower();

    scheduleOnce([this](float dt)
        {
            StartWaves();
        }, 0.f, "StartWaves");

}
void SecondMapScene::updateFail(float dt) {
    auto failSprite = this->getChildByName("FailSprite");
    auto tryAgainMenu = this->getChildByName("TryAgainMenu");
    auto selectMenu = this->getChildByName("SelectMenu");

    if (carrot->hp <= 0 && !failSprite && !tryAgainMenu) {
        cocos2d::AudioEngine::play2d("test/Lose.ogg", false);
        failSprite = Sprite::create("test/Fail.png");
        failSprite->setPosition(Vec2(500, 300));
        failSprite->setName("FailSprite");
        this->addChild(failSprite, 11);
        Director::getInstance()->getActionManager()->pauseAllRunningActions();
        unschedule("update_monsters");
        unschedule("checkWave");

        auto failRoundLabel = Label::createWithTTF(StringUtils::format("%d    10", round), "fonts/Marker Felt.ttf", 18);
        if (failRoundLabel) {
            failRoundLabel->setPosition(Vec2(526, 317));
            this->addChild(failRoundLabel, 11);
        }

        auto failMapLabel = Label::createWithTTF("2", "fonts/Marker Felt.ttf", 18);
        if (failMapLabel) {
            failMapLabel->setPosition(Vec2(443, 282));
            this->addChild(failMapLabel, 11);
        }
        auto selectItem = MenuItemImage::create(
            "test/SelectNormal.png",
            "test/SelectSelected.png",
            [](Ref* sender) {
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MapSelectionScene::createScene()));
            }
        );
        if (selectItem) {
            selectItem->setPosition(415, 248);
            auto menu = cocos2d::Menu::create(selectItem, nullptr);
            menu->setPosition(cocos2d::Vec2::ZERO);
            this->addChild(menu, 12);
            menu->setName("SelectMenu");
        }
        else {
            CCLOG("Failed to load select button");
        }

        auto tryAgainItem = MenuItemImage::create(
            "test/TryAgainNormal.png",
            "test/TryAgainSelected.png",
            CC_CALLBACK_1(SecondMapScene::secondTryAgain, this));

        if (tryAgainItem) {
            tryAgainItem->setPosition(Vec2(558, 248));
            auto menu = Menu::create(tryAgainItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 12);
            menu->setName("TryAgainMenu");
        }
        else {
            CCLOG("Failed to load try again button");
        }
    }
}
void SecondMapScene::updateWin(float dt) {
    auto winSprite = this->getChildByName("WinSprite");
    auto tryAgainMenu = this->getChildByName("TryAgainMenu");
    auto selectMenu = this->getChildByName("SelectMenu");

    if (win == 1 && !winSprite && !tryAgainMenu) {
        cocos2d::AudioEngine::play2d("test/Victory.ogg", false);
        winSprite = Sprite::create("test/Win.png");
        winSprite->setPosition(Vec2(500, 300));
        winSprite->setName("WinSprite");
        this->addChild(winSprite, 11);
        Director::getInstance()->getActionManager()->pauseAllRunningActions();

        auto selectItem = MenuItemImage::create(
            "test/SelectNormal.png",
            "test/SelectSelected.png",
            [](Ref* sender) {
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MapSelectionScene::createScene()));
            }
        );
        if (selectItem) {
            selectItem->setPosition(415, 248);
            auto menu = cocos2d::Menu::create(selectItem, nullptr);
            menu->setPosition(cocos2d::Vec2::ZERO);
            this->addChild(menu, 12);
            menu->setName("SelectMenu");
        }
        else {
            CCLOG("Failed to load select button");
        }


        auto tryAgainItem = MenuItemImage::create(
            "test/TryAgainNormal.png",
            "test/TryAgainSelected.png",
            CC_CALLBACK_1(SecondMapScene::secondTryAgain, this));

        if (tryAgainItem) {
            tryAgainItem->setPosition(Vec2(558, 248));
            auto menu = Menu::create(tryAgainItem, nullptr);
            menu->setPosition(Vec2::ZERO);
            this->addChild(menu, 12);
            menu->setName("TryAgainMenu");
        }
        else {
            CCLOG("Failed to load try again button");
        }
    }
}
void SecondMapScene::changeSpeed(Ref* sender) {
    float currentTimeScale = Director::getInstance()->getScheduler()->getTimeScale();
    if (currentTimeScale == 1.0f) {
        Director::getInstance()->getScheduler()->setTimeScale(1.5f);
    }
    else {
        Director::getInstance()->getScheduler()->setTimeScale(1.0f);
    }
}

bool SecondMapScene::handleGridTouch(Touch* touch, Event* event) {
    Vec2 touchPos = touch->getLocation();
    int col = (touchPos.x) / 60;
    int row = (touchPos.y) / 68;
    GridType type = static_cast<GridType>(gridMap[row][col]);

    if (type == _ICE || type == _STONE || type == _TREE || type == _HOUSE || type == _ICECREAM || type == _BIGTREE) {
        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);
        upgradeButton->setVisible(false);
        deleteButton->setVisible(false);
    }
    if (type == _NO) {
        AudioEngine::play2d("test/Wrong.mp3");
        Sprite* warning = Sprite::create("test/Warning.png");
        warning->setPosition(Vec2(col * 60 + 25, row * 68 + 20));
        this->addChild(warning);
        auto delay = DelayTime::create(1.0f);
        auto hide = Hide::create();
        auto sequence = Sequence::create(delay, hide, nullptr);
        warning->runAction(sequence);
        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);
        upgradeButton->setVisible(false);
        deleteButton->setVisible(false);
    }
    if (type == _YES) {
        upgradeButton->setVisible(false);
        deleteButton->setVisible(false);
        blank->setVisible(true);
        pickBottle->setVisible(true);
        pickShit->setVisible(true);
        pickSnow->setVisible(true);

        blank->setPosition(Vec2(col * 60 + 25, row * 68 + 25));
        pickBottle->setPosition(Vec2(col * 60 + 25 - 45, row * 68 + 25 + 45));
        pickShit->setPosition(Vec2(col * 60 + 25, row * 68 + 25 + 45));
        pickSnow->setPosition(Vec2(col * 60 + 25 + 45, row * 68 + 25 + 45));

        pickBottle->addTouchEventListener(CC_CALLBACK_2(SecondMapScene::onPickBottleTouch, this));
        pickShit->addTouchEventListener(CC_CALLBACK_2(SecondMapScene::onPickShitTouch, this));
        pickSnow->addTouchEventListener(CC_CALLBACK_2(SecondMapScene::onPickSnowTouch, this));

    }
    if (type == _BOTTLE || type == _SHIT || type == _SNOW) {
        auto tower = gridTowerMap[row][col];
        Vec2 Pos = tower->getPosition();
        int col = static_cast<int>(Pos.x / 60);
        int row = static_cast<int>(Pos.y / 68);

        if (type == _BOTTLE) {
            if (tower->level == 0) {
                if (secondMoney >= 180) {
                    upgradeButton->loadTextureNormal("tower/upgrade180.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/180.png");
                }
                deleteButton->loadTextureNormal("tower/80.png");
            }
            if (tower->level == 1) {
                if (secondMoney >= 220) {
                    upgradeButton->loadTextureNormal("tower/upgrade220.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/220.png");
                }
                deleteButton->loadTextureNormal("tower/176.png");
            }
            if (tower->level == 2) {
                upgradeButton->loadTextureNormal("tower/max.png");
                deleteButton->loadTextureNormal("tower/224.png");
            }
        }
        if (type == _SHIT) {
            if (tower->level == 0) {
                if (secondMoney >= 220) {
                    upgradeButton->loadTextureNormal("tower/upgrade220.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/220.png");
                }
                deleteButton->loadTextureNormal("tower/96.png");
            }
            if (tower->level == 1) {
                if (secondMoney >= 220) {
                    upgradeButton->loadTextureNormal("tower/upgrade260.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/260.png");
                }
                deleteButton->loadTextureNormal("tower/208.png");
            }
            if (tower->level == 2) {
                upgradeButton->loadTextureNormal("tower/max.png");
                deleteButton->loadTextureNormal("tower/384.png");
            }
        }
        if (type == _SNOW) {
            if (tower->level == 0) {
                if (secondMoney >= 260) {
                    upgradeButton->loadTextureNormal("tower/upgrade260.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/260.png");
                }
                deleteButton->loadTextureNormal("tower/128.png");
            }
            if (tower->level == 1) {
                if (secondMoney >= 320) {
                    upgradeButton->loadTextureNormal("tower/upgrade320.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/320.png");
                }
                deleteButton->loadTextureNormal("tower/352.png");
            }
            if (tower->level == 2) {
                upgradeButton->loadTextureNormal("tower/max.png");
                deleteButton->loadTextureNormal("tower/432.png");
            }
        }
        upgradeButton->setVisible(true);
        upgradeButton->setPosition(Vec2(tower->getPositionX() - 25, tower->getPositionY() + 45));
        deleteButton->setVisible(true);
        deleteButton->setPosition(Vec2(tower->getPositionX() + 25, tower->getPositionY() + 45));

        upgradeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType eventType) {
            if (eventType == ui::Widget::TouchEventType::ENDED) {
                if (type == _BOTTLE) {
                    if (tower->level == 0) {
                        if (secondMoney >= 180) {
                            tower->upgrade(tower);
                            secondMoney -= 180;
                            AudioEngine::play2d("tower/Up.ogg");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                        else {
                            AudioEngine::play2d("test/Wrong.mp3");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                    }
                    if (tower->level == 1) {
                        if (secondMoney >= 220) {
                            tower->upgrade(tower);
                            secondMoney -= 220;
                            AudioEngine::play2d("tower/Up.ogg");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                        else {
                            AudioEngine::play2d("test/Wrong.mp3");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                    }
                }
                if (type == _SHIT) {
                    if (tower->level == 0) {
                        if (secondMoney >= 220) {
                            tower->upgrade(tower);
                            secondMoney -= 220;
                            AudioEngine::play2d("tower/Up.ogg");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                        else {
                            AudioEngine::play2d("test/Wrong.mp3");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                    }
                    if (tower->level == 1) {
                        if (secondMoney >= 260) {
                            tower->upgrade(tower);
                            secondMoney -= 260;
                            AudioEngine::play2d("tower/Up.ogg");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                        else {
                            AudioEngine::play2d("test/Wrong.mp3");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                    }
                }
                if (type == _SNOW) {
                    if (tower->level == 0) {
                        if (secondMoney >= 260) {
                            tower->upgrade(tower);
                            secondMoney -= 260;
                            AudioEngine::play2d("tower/Up.ogg");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                        else {
                            AudioEngine::play2d("test/Wrong.mp3");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                    }
                    if (tower->level == 1) {
                        if (secondMoney >= 320) {
                            tower->upgrade(tower);
                            secondMoney -= 320;
                            AudioEngine::play2d("tower/Up.ogg");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                        else {
                            AudioEngine::play2d("test/Wrong.mp3");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                    }
                }
            }
            });

        deleteButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType eventType) {
            if (eventType == ui::Widget::TouchEventType::ENDED) {
                if (tower) {
                    secondMoney += tower->remove();
                    gridMap[row][col] = _YES;
                    //grid_TowerMap[row][col] = tower;
                    deleteButton->setVisible(false);
                }
            }
            else {
                upgradeButton->setVisible(false);
                deleteButton->setVisible(false);
            }
            });

        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);

    }
    return true;
}
void SecondMapScene::onPickBottleTouch(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        Vec2 blankPos = blank->getPosition();
        int col = static_cast<int>(blankPos.x / 60);
        int row = static_cast<int>(blankPos.y / 68);
        if (secondMoney >= 100) {
            AudioEngine::play2d("test/Build.ogg");
            auto bottle = Bottle::construct(blankPos, BOTTLE);
            if (bottle) {
                bottle->setTag(100);
                this->addChild(bottle, 0);
                manager.AddTower(bottle);
                gridTowerMap[row][col] = bottle;
            }
            secondMoney -= 100;
            gridMap[row][col] = _BOTTLE;
            gridTowerMap[row][col] = bottle;
        }
        else {
            AudioEngine::play2d("test/Wrong.mp3");
        }
        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);
    }
}
void SecondMapScene::onPickShitTouch(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        Vec2 blankPos = blank->getPosition();
        int col = static_cast<int>(blankPos.x / 60);
        int row = static_cast<int>(blankPos.y / 68);
        if (secondMoney >= 120) {
            AudioEngine::play2d("test/Build.ogg");
            auto shit = Shit::construct(blankPos, SHIT);
            if (shit) {
                shit->setTag(100);
                shit->setPosition(blankPos);
                this->addChild(shit, 0);
                manager.AddTower(shit);
            }
            secondMoney -= 120;
            gridMap[row][col] = _SHIT;
            gridTowerMap[row][col] = shit;
        }
        else {
            AudioEngine::play2d("test/Wrong.mp3");
        }
        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);
    }
}
void SecondMapScene::onPickSnowTouch(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        Vec2 blankPos = blank->getPosition();
        int col = static_cast<int>(blankPos.x / 60);
        int row = static_cast<int>(blankPos.y / 68);
        if (secondMoney >= 180) {
            AudioEngine::play2d("test/Build.ogg");
            auto snow = Snow::construct(blankPos, SNOW);
            if (snow) {
                snow->setTag(100);
                snow->setPosition(blankPos);
                manager.AddTower(snow);
                this->addChild(snow, 0);
            }
            secondMoney -= 180;
            gridMap[row][col] = _SNOW;
            gridTowerMap[row][col] = snow;
        }
        else {
            AudioEngine::play2d("test/Wrong.mp3");
        }
        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);
    }
}

bool SecondMapScene::init() {
    if (!Scene::init()) {
        return false;
    }

    cocos2d::AudioEngine::stopAll();
    cocos2d::AudioEngine::play2d("test/PlayBGM.mp3", true);

    auto sprite = cocos2d::Sprite::create("test/SelectBG.png");
    if (sprite == nullptr)
    {
        problemLoading("'test/SelectBG.png'");
    }
    else
    {
        sprite->setScale(1000.0f / sprite->getContentSize().width, 600.0f / sprite->getContentSize().height);//设置图片大小，要跟运行框对应
        sprite->setPosition(cocos2d::Vec2(500, 300));
        this->addChild(sprite, 0);
    }

    auto path = cocos2d::Sprite::create("test/Path2.png");
    if (path == nullptr)
    {
        problemLoading("'test/Path2.png'");
    }
    else
    {
        path->setScale(1000.0f / path->getContentSize().width, 600.0f / path->getContentSize().height);//设置图片大小，要跟运行框对应
        path->setPosition(cocos2d::Vec2(500, 300));
        this->addChild(path, 0);
    }

    auto upBoard = cocos2d::Sprite::create("test/UpBoard.png");
    if (upBoard == nullptr)
    {
        problemLoading("'test/UpBoard.png'");
    }
    else
    {
        upBoard->setPosition(cocos2d::Vec2(500, 571));
        this->addChild(upBoard, 0);
    }

    auto upBoard2 = cocos2d::Sprite::create("test/UpBoard2.png");
    if (upBoard == nullptr)
    {
        problemLoading("'test/UpBoard2.png'");
    }
    else
    {
        upBoard2->setPosition(cocos2d::Vec2(537, 571));
        this->addChild(upBoard2, 0);
    }

    auto moneyLabel = createMoneyLabel();//0层
    if (moneyLabel) {
        moneyLabel->setPosition(cocos2d::Vec2(200, 575));
        this->addChild(moneyLabel, 0);
    }
    else {
        CCLOG("Error creating Label");
    }

    auto roundLabel = createRoundLabel();//0层
    if (roundLabel) {
        roundLabel->setPosition(cocos2d::Vec2(540, 575));
        this->addChild(roundLabel, 0);
    }
    else {
        CCLOG("Error creating Round Label");
    }

    auto speedUpItem1 = MenuItemImage::create("test/Speed_1.png", "test/Speed_1.png");
    auto speedUpItem2 = MenuItemImage::create("test/Speed_2.png", "test/Speed_2.png");
    auto speedUp = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SecondMapScene::changeSpeed, this), speedUpItem1, speedUpItem2, nullptr);
    if (speedUp) {
        speedUp->setPosition(Vec2(730, 570));
        auto menu = Menu::create(speedUp, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);
    }

    auto pauseItem1 = MenuItemImage::create("test/Pause.png", "test/Pause.png");
    auto pauseItem2 = MenuItemImage::create("test/Restart.png", "test/Restart.png");
    auto pauseItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SecondMapScene::pause, this), pauseItem1, pauseItem2, nullptr);
    if (pauseItem) {
        pauseItem->setPosition(Vec2(800, 570));
        auto menu = Menu::create(pauseItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);
    }

    auto BackItem = cocos2d::MenuItemImage::create(
        "test/BackNormal.png",
        "test/BackSelected.png",
        CC_CALLBACK_1(SecondMapScene::backToMain, this));
    if (BackItem) {
        BackItem->setPosition(cocos2d::Vec2(850, 575));
        auto menu = cocos2d::Menu::create(BackItem, nullptr); // 创建菜单并添加返回按钮
        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
    }
    else {
        CCLOG("Failed to load back button"); // 如果创建失败，输出错误信息
    }

    auto hpSprite = Carrot::createHP();
    hpSprite->setPosition(cocos2d::Vec2(755, 520));
    this->addChild(hpSprite);

    carrot = Carrot::createCarrot();
    carrot->setPosition(cocos2d::Vec2(755, 485));
    this->addChild(carrot);

    auto HPLabel = carrot->HPLabel;
    if (HPLabel) {
        HPLabel->setPosition(Vec2(745, 520));
        HPLabel->setTextColor(Color4B::BLACK);
        this->addChild(HPLabel);
    }

    carrot->schedule([this](float dt) {
        this->carrot->updateCarrotSprite();
        this->carrot->updateHPLabel();
        }, 0.1f, "update_carrot_hp");

    this->schedule(CC_SCHEDULE_SELECTOR(SecondMapScene::updateRoundLabel), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(SecondMapScene::updateMoneyLabel), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(SecondMapScene::updateWin), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(SecondMapScene::updateFail), 0.1f);

    auto hpButton = cocos2d::MenuItemImage::create("test/Update.png", "test/Update.png", [=](cocos2d::Ref* sender) {
        if (secondMoney >= 500) {
            carrot->hp += 1;
            secondMoney -= 500;
            carrot->updateCarrotSprite();//修改萝卜图案
            moneyLabel->setString(cocos2d::StringUtils::format("%d", secondMoney));//修改moneyLable
            cocos2d::AudioEngine::play2d("test/Select.mp3", false);
            cocos2d::AudioEngine::play2d("tower/Up.ogg", false);
        }
        else {
            cocos2d::AudioEngine::play2d("test/Wrong.mp3", false);
        }
        });
    if (hpButton) {
        hpButton->setPosition(cocos2d::Vec2(795, 470));
        auto menu = cocos2d::Menu::create(hpButton, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
        auto Label = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("500"), "fonts/Marker Felt.ttf", 10);
        if (Label) {
            Label->setPosition(cocos2d::Vec2(798, 455));
            Label->setTextColor(cocos2d::Color4B::BLACK);
            this->addChild(Label);
        }
        else {
            CCLOG("Error creating Label");
        }
    }
    else {
        CCLOG("Error creating HP Button");
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 17; col++) {
            float posX = col * 60 + 25;   // 格子的中心 X 坐标
            float posY = row * 68 + 25; // 格子的中心 Y 坐标

            if (row == 2 && col == 8) {
                continue;
            }
            if (row == 2 && col == 9) {
                continue;
            }
            if (row == 3 && col == 4) {
                continue;
            }
            if (row == 3 && col == 5) {
                continue;
            }
            if (row == 3 && col == 8) {
                continue;
            }
            if (row == 3 && col == 9) {
                posX -= 30;
                posY -= 40;
            }
            if (row == 4 && col == 4) {
                continue;
            }
            if (row == 4 && col == 5) {
                posX -= 30;
                posY -= 40;
            }
            if (row == 5 && col == 8) {
                continue;
            }
            if (row == 5 && col == 9) {
                posX -= 30;
            }
            if (row == 6 && col == 3) {
                continue;
            }
            if (row == 6 && col == 4) {
                continue;
            }
            if (row == 6 && col == 6) {
                continue;
            }
            if (row == 6 && col == 7) {
                continue;
            }
            if (row == 6 && col == 10) {
                continue;
            }
            if (row == 6 && col == 11) {
                continue;
            }
            if (row == 7 && col == 6) {
                continue;
            }
            if (row == 7 && col == 3) {
                continue;
            }
            if (row == 7 && col == 4) {
                posX -= 30;
                posY -= 40;
            }
            if (row == 7 && col == 7) {
                posX -= 30;
                posY -= 40;
            }
            if (row == 7 && col == 10) {
                continue;
            }
            if (row == 7 && col == 11) {
                posX -= 30;
                posY -= 40;
            }

            std::string filename;
            switch (gridMap[row][col]) {
            case _STONE:
                filename = "test/Stone.png";
                break;
            case _ICE:
                filename = "test/Ice.png";
                break;
            case _BIGTREE:
                filename = "test/BigTree.png";
                break;
            case _ICECREAM:
                filename = "test/Icecream.png";
                break;
            case _TREE:
                filename = "test/Tree.png";
                break;
            case _HOUSE:
                filename = "test/House.png";
                break;

            }
            Sprite* gridSprite = Sprite::create(filename);
            if (gridSprite) {
                gridSprite->setPosition(Vec2(posX, posY));
                this->addChild(gridSprite);
            }
        }
    }

    blank = Sprite::create("tower/blank.png");
    this->addChild(blank, 1);
    pickBottle = ui::Button::create("tower/pickBottle.png");
    pickBottle->setScale(0.6);
    this->addChild(pickBottle, 1);
    pickShit = ui::Button::create("tower/pickShit.png");
    pickShit->setScale(0.6);
    this->addChild(pickShit, 1);
    pickSnow = ui::Button::create("tower/pickSnow.png");
    pickSnow->setScale(0.6);
    this->addChild(pickSnow, 1);
    upgradeButton = ui::Button::create("tower/Upgrade.png");
    this->addChild(upgradeButton, 1);
    deleteButton = ui::Button::create("tower/sell.png");
    this->addChild(deleteButton, 1);

    auto touchListener2 = EventListenerTouchOneByOne::create();
    touchListener2->setSwallowTouches(true);
    touchListener2->onTouchBegan = CC_CALLBACK_2(SecondMapScene::handleGridTouch, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener2, this);


    /*=======================这里是怪物=========================*/

    schedule(CC_SCHEDULE_SELECTOR(SecondMapScene::update), 1.0f);

    //怪物路径信息
    CheckPoints.push_back(Vec2(750, 420));
    CheckPoints.push_back(Vec2(750, 130));
    CheckPoints.push_back(Vec2(630, 130));
    CheckPoints.push_back(Vec2(630, 270));
    CheckPoints.push_back(Vec2(385, 270));
    CheckPoints.push_back(Vec2(385, 330));
    CheckPoints.push_back(Vec2(200, 330));
    CheckPoints.push_back(Vec2(200, 130));
    CheckPoints.push_back(Vec2(260, 130));
    CheckPoints.push_back(Vec2(260, 90));
    CheckPoints.push_back(Vec2(INITX2, INITY2));

    //怪物波次信息
    waves.push_back(WaveData(FASTMONSTER, 20, 10, 1.f, "firstWave"));
    waves.push_back(WaveData(NORMALMONSTER, 30, 10, 1.f, "secondWave"));
    waves.push_back(WaveData(BIGMONSTER, 200, 2, 3.f, "thirdWave"));
    waves.push_back(WaveData(FASTMONSTER, 70, 10, 1.f, "forthWave"));
    waves.push_back(WaveData(FASTMONSTER, 90, 10, 1.f, "fifthWave"));
    waves.push_back(WaveData(FASTMONSTER, 100, 10, 1.f, "sixthWave"));
    waves.push_back(WaveData(NORMALMONSTER, 120, 10, 1.f, "seventhWave"));
    waves.push_back(WaveData(FASTMONSTER, 130, 10, 1.f, "eighthWave"));
    waves.push_back(WaveData(NORMALMONSTER, 120, 10, 1.f, "ninthWave"));
    waves.push_back(WaveData(BIGMONSTER, 300, 8, 3.f, "tenthWave"));

    scheduleOnce([this, myCarrot = carrot](float dt)
        {
            StartWaves();
        }, 2.f, "StartWave");

    return true;
}
void SecondMapScene::StartWaves()
{
    SpawnWave(waves[0]);//开始生成怪物
}
void SecondMapScene::MonsterWave(const int type, const float blood, const int count, const float inr)
{
    std::map<int, std::function<Monster* ()>> monsterCreators;
    monsterCreators[NORMALMONSTER] = [&]() { return NormalMonster::create(blood, INITX2, INITY2); };
    monsterCreators[FASTMONSTER] = [&]() { return FastMonster::create(blood, INITX2, INITY2); };
    monsterCreators[BIGMONSTER] = [&]() { return BigMonster::create(blood, INITX2, INITY2); };
    if (monsterCreators.find(type) != monsterCreators.end())
    {
        for (int i = 0; i < count; i++)
        {
            Monster* monster = monsterCreators[type]();
            addMonsterToScene(monster);
            monster->Emerge(inr * i);

            //auto contactListener = EventListenerPhysicsContact::create();   
            //contactListener->onContactBegin = CC_CALLBACK_1(Monster::onContactBegin, monster); 
            //_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
        }
    }
    schedule([this](float dt)
        {
            manager.Update(dt);
        }, 0.1f, "update");
}

void SecondMapScene::SpawnWave(const WaveData& wave)
{
    scheduleOnce([this, wave](float dt)
        {
            MonsterWave(wave.type, wave.count, wave.health, wave.interval);
            schedule([this, wave](float dt)
                {
                    if (manager.CheckWave())
                    {
                        NextWaveCallback();
                        unschedule("checkWave");
                    }
                    CCLOG("hahahah");
                }, 0.1f, "checkWave");
        }, 5.f, wave.name);
}
void SecondMapScene::NextWaveCallback()
{
    if (round < waves.size())
    {
        CCLOG("Current wave completed. Starting next wave.");//输出提示下一波
        //win = 1;
        round++; //轮次加一
        if (win != 1) {
            SpawnWave(waves[round - 1]);// 生成下一波
        }
    }
    else
    {
        win = 1;
        isFirstMapWon = 1;
        CCLOG("All waves completed.");//所有波次结束
    }
}
