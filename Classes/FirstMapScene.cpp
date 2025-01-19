#include"audio/include/AudioEngine.h"
#include"Data.h"
#include"HelloWorldScene.h"
#include"MapSelectionScene.h"
#include"FirstMapScene.h"
#include"Object.h"
#include"Tower.h"
#include<vector>
USING_NS_CC;
//���⴦��
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
//��������
Scene* FirstMapScene::createScene() {
    auto scene = FirstMapScene::create();
    firstMoney = 10000;
    return scene;
}
//�������α�ǩ
cocos2d::Label* FirstMapScene::createRoundLabel() {
    RoundLabel = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d      10 rounds", round), "fonts/Marker Felt.ttf", 24);
    if (!RoundLabel) {
        CCLOG("Error creating HP Label");
    }
    return RoundLabel;
}
//���²��α�ǩ
void FirstMapScene::updateRoundLabel(float dt) {
    if (RoundLabel) {
        RoundLabel->setString(cocos2d::StringUtils::format("%d      10 rounds", round));
    }
}
//������ұ�ǩ
cocos2d::Label* FirstMapScene::createMoneyLabel() {
    MoneyLabel = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", firstMoney), "fonts/Marker Felt.ttf", 24);
    if (!MoneyLabel) {
        CCLOG("Error creating HP Label");
    }
    return MoneyLabel;
}
//���½�ұ�ǩ
void FirstMapScene::updateMoneyLabel(float dt) {
    if (MoneyLabel) {
        MoneyLabel->setString(cocos2d::StringUtils::format("%d", firstMoney));
    }
}

//������ҳ��
void FirstMapScene::backToMain(cocos2d::Ref* sender) {
    int selectId = cocos2d::AudioEngine::play2d("test/Select.mp3", false);//�����true or false��ѭ������˼
    this->schedule([=](float dt) {
        if (cocos2d::AudioEngine::getState(selectId) != cocos2d::AudioEngine::AudioState::PLAYING) {
            cocos2d::AudioEngine::stopAll();
            this->unschedule("checkAudioFinished");
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.0f, HelloWorld::createScene()));// ����Ƶ������Ϻ�ִ���л������Ĳ���
        }
        }, "checkAudioFinished");
}
//��ͣ��ָ�
void FirstMapScene::pause(cocos2d::Ref* sender) {
    if (Director::getInstance()->isPaused()) {
        Director::getInstance()->resume(); // �ָ�����������
    }
    else {
        Director::getInstance()->pause(); // ��ͣ����������
    }
}
//�Ƴ����й���
void FirstMapScene::removeAllMonster() {
    auto children = this->getChildren();
    for (auto child : children) {
        auto monster = dynamic_cast<Monster*>(child);//��Monster*��ľ��Ƴ�
        if (monster) {
            this->removeChild(monster);
        }
    }
}
//�Ƴ���������
void FirstMapScene::removeAllTower() {
    Vector<Node*> children = this->getChildren();
    for (const auto& child : children) {
        if (child->getTag() == 100) {//�ñ�ǩ������
            this->removeChild(child);
        }
    }
}
//����һ��
void FirstMapScene::firstTryAgain(cocos2d::Ref* sender) {
    cocos2d::AudioEngine::play2d("test/Select.mp3", false);
    carrot->hp = 10;//��ʼ����Щ��ֵ�Լ���ͼ
    firstMoney = 200;
    round = 1;
    win = 0;
    grid_Map[1][7] = grid_Map[1][8] = grid_Map[1][10] = grid_Map[1][11] = grid_Map[1][12] = _YES;
    grid_Map[2][2] = grid_Map[2][3] = grid_Map[2][4] = grid_Map[2][12] = grid_Map[2][13] = grid_Map[2][14] = _YES;
    grid_Map[3][2] = grid_Map[3][3] = grid_Map[3][7] = grid_Map[3][8] = grid_Map[3][13] = grid_Map[3][14] = _YES;
    grid_Map[4][2] = grid_Map[4][3] = grid_Map[4][4] = grid_Map[4][6] = grid_Map[4][7] = grid_Map[4][9] = grid_Map[4][10] = grid_Map[4][12] = grid_Map[4][13] = grid_Map[4][14] = _YES;
    grid_Map[5][2] = grid_Map[5][4] = grid_Map[5][8] = grid_Map[5][10] = grid_Map[5][12] = grid_Map[5][14] = _YES;
    grid_Map[6][2] = grid_Map[6][6] = grid_Map[6][7] = grid_Map[6][8] = grid_Map[6][14] = _YES;
    grid_Map[7][2] = grid_Map[7][3] = grid_Map[7][5] = grid_Map[7][7] = grid_Map[7][8] = grid_Map[7][11] = grid_Map[7][12] = grid_Map[7][13] = grid_Map[7][14] = _YES;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 17; j++) {
            grid_TowerMap[i][j] = nullptr;
        }
    }

    this->removeChildByName("FailSprite");//�Ƴ���������о���
    this->removeChildByName("WinSprite");
    this->removeChildByName("TryAgainMenu");
    this->removeChildByName("SelectMenu");
    this->removeAllMonster();//�Ƴ���һ�صľ���
    this->removeAllTower();

    scheduleOnce([this](float dt)//��ʼ��������
        {
            StartWaves();
        }, 0.f, "StartWaves");
}
//���ϸ����Ƿ�����
void FirstMapScene::updateFail(float dt) {
    auto failSprite = this->getChildByName("FailSprite");//������������
    auto tryAgainMenu = this->getChildByName("TryAgainMenu");
    auto selectMenu = this->getChildByName("SelectMenu");

    if (carrot->hp == 0 && !failSprite && !tryAgainMenu) {//���˾���ʾ���㳡��
        cocos2d::AudioEngine::play2d("test/Lose.ogg", false);
        failSprite = Sprite::create("test/Fail.png");
        failSprite->setPosition(Vec2(500, 300));
        failSprite->setName("FailSprite");
        this->addChild(failSprite, 11);
        Director::getInstance()->getActionManager()->pauseAllRunningActions();//ֹͣ���еĶ���
        unschedule("update_monsters");
        unschedule("checkWave");

        auto failRoundLabel = Label::createWithTTF(StringUtils::format("%d    10", round), "fonts/Marker Felt.ttf", 18);//�ִα�ǩ
        if (failRoundLabel) {
            failRoundLabel->setPosition(Vec2(526, 317));
            this->addChild(failRoundLabel, 11);
        }

        auto failMapLabel = Label::createWithTTF("1", "fonts/Marker Felt.ttf", 18);//�ؿ���ǩ
        if (failMapLabel) {
            failMapLabel->setPosition(Vec2(443, 282));
            this->addChild(failMapLabel, 11);
        }

        auto selectItem = MenuItemImage::create(//ѡ���ͼ�İ�ť
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

        auto tryAgainItem = MenuItemImage::create(//����һ�εİ�ť
            "test/TryAgainNormal.png",
            "test/TryAgainSelected.png",
            CC_CALLBACK_1(FirstMapScene::firstTryAgain, this));

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
//���ϸ����Ƿ�Ӯ�ˣ������������updateFail������ͬ
void FirstMapScene::updateWin(float dt) {
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
            CC_CALLBACK_1(FirstMapScene::firstTryAgain, this));

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
//�����ٶȣ��ü�ʱ��������
void FirstMapScene::changeSpeed(Ref* sender) {
    float currentTimeScale = Director::getInstance()->getScheduler()->getTimeScale();
    if (currentTimeScale == 1.0f) {
        Director::getInstance()->getScheduler()->setTimeScale(1.5f);
    }
    else {
        Director::getInstance()->getScheduler()->setTimeScale(1.0f);
    }
}

//������������صĲ���
bool FirstMapScene::handleGridTouch(Touch* touch, Event* event) {
    Vec2 touchPos = touch->getLocation();//�õ������λ�õĸ����������
    int col = (touchPos.x) / 60;
    int row = (touchPos.y) / 68;
    Grid_Type type = static_cast<Grid_Type>(grid_Map[row][col]);

    if (type == _ICE || type == _BIGICE || type == _TREE || type == _HOUSE || type == _ICECREAM) {//���ϰ���Ļ�������һЩ��ť
        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);
        upgradeButton->setVisible(false);
        deleteButton->setVisible(false);
    }
    if (type == _NO) {//�ǲ�����ֲ�ĵط�����ʾ��������һЩ��ť
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
    if (type == _YES) {//�ǿ�����ֲ�ĵط���ʾ��ֲ����ز���
        upgradeButton->setVisible(false);
        deleteButton->setVisible(false);
        blank->setVisible(true);
        pickBottle->setVisible(true);
        pickShit->setVisible(true);
        pickSnow->setVisible(true);

        blank->setPosition(Vec2(col * 60 + 25, row * 68 + 20));
        pickBottle->setPosition(Vec2(col * 60 + 25 - 45, row * 68 + 20 + 45));
        pickShit->setPosition(Vec2(col * 60 + 25, row * 68 + 20 + 45));
        pickSnow->setPosition(Vec2(col * 60 + 25 + 45, row * 68 + 20 + 45));

        pickBottle->addTouchEventListener(CC_CALLBACK_2(FirstMapScene::onPickBottleTouch, this));//�ж��Ƿ�����ƿ��
        pickShit->addTouchEventListener(CC_CALLBACK_2(FirstMapScene::onPickShitTouch, this));//�ж��Ƿ����ֱ��
        pickSnow->addTouchEventListener(CC_CALLBACK_2(FirstMapScene::onPickSnowTouch, this));//�ж��Ƿ�����ѩ��

    }
    if (type == _BOTTLE || type == _SHIT || type == _SNOW) {//���Ѿ���ֲ�ĵط��������������Ƴ�����ز���
        auto tower = grid_TowerMap[row][col];
        Vec2 Pos = tower->getPosition();
        int col = static_cast<int>(Pos.x / 60);
        int row = static_cast<int>(Pos.y / 68);
        //�ж�Ҫ��ʾ�İ�ť�Ƕ���Ǯ�İ�ť
        if (type == _BOTTLE) {
            if (tower->level == 0) {
                if (firstMoney >= 180) {
                    upgradeButton->loadTextureNormal("tower/upgrade180.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/180.png");
                }
                deleteButton->loadTextureNormal("tower/80.png");
            }
            if (tower->level == 1) {
                if (firstMoney >= 220) {
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
                if (firstMoney >= 220) {
                    upgradeButton->loadTextureNormal("tower/upgrade220.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/220.png");
                }
                deleteButton->loadTextureNormal("tower/96.png");
            }
            if (tower->level == 1) {
                if (firstMoney >= 220) {
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
                if (firstMoney >= 260) {
                    upgradeButton->loadTextureNormal("tower/upgrade260.png");
                }
                else {
                    upgradeButton->loadTextureNormal("tower/260.png");
                }
                deleteButton->loadTextureNormal("tower/128.png");
            }
            if (tower->level == 1) {
                if (firstMoney >= 320) {
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
        //��ʾ��ť
        upgradeButton->setVisible(true);
        upgradeButton->setPosition(Vec2(tower->getPositionX() - 25, tower->getPositionY() + 45));
        deleteButton->setVisible(true);
        deleteButton->setPosition(Vec2(tower->getPositionX() + 25, tower->getPositionY() + 45));
        //�������߼�
        upgradeButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType eventType) {
            if (eventType == ui::Widget::TouchEventType::ENDED) {
                if (type == _BOTTLE) {
                    if (tower->level == 1) {
                        if (firstMoney >= 220) {
                            tower->upgrade(tower);
                            firstMoney -= 220;
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
                    if (tower->level == 0) {
                        if (firstMoney >= 180) {//�ж��Ƿ���Ǯ��ֲ���еĻ�����ֲ
                            tower->upgrade(tower);
                            firstMoney -= 180;
                          AudioEngine::play2d("tower/Up.ogg");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                        else {//ûǮ��ֲ����
                            AudioEngine::play2d("test/Wrong.mp3");
                            upgradeButton->setVisible(false);
                            deleteButton->setVisible(false);
                        }
                    }
                }
                if (type == _SHIT) {
                    if (tower->level == 1) {
                        if (firstMoney >= 260) {
                            tower->upgrade(tower);
                            firstMoney -= 260;
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
                    if (tower->level == 0) {
                        if (firstMoney >= 220) {
                            tower->upgrade(tower);
                            firstMoney -= 220;
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
                    if (tower->level == 1) {
                        if (firstMoney >= 320) {
                            tower->upgrade(tower);
                            firstMoney -= 320;
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
                    if (tower->level == 0) {
                        if (firstMoney >= 260) {
                            tower->upgrade(tower);
                            firstMoney -= 260;
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
                    firstMoney += tower->remove();//�Ƴ������½��
                    grid_Map[row][col] = _YES;
                    //grid_TowerMap[row][col] = tower;
                    deleteButton->setVisible(false);
                }
            }
            else {
                upgradeButton->setVisible(false);
                deleteButton->setVisible(false);
            }
            });
        //����һЩ��ť
        blank->setVisible(false);
        pickBottle->setVisible(false);
        pickShit->setVisible(false);
        pickSnow->setVisible(false);

    }
    return true;
}
//����������������
void FirstMapScene::onPickBottleTouch(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        Vec2 blankPos = blank->getPosition();
        int col = static_cast<int>(blankPos.x / 60);
        int row = static_cast<int>(blankPos.y / 68);
        if (firstMoney >= 100) {
            AudioEngine::play2d("test/Build.ogg");
            auto bottle = Bottle::construct(blankPos, BOTTLE);
            if (bottle) {//��ƿ����ӵ�������
                bottle->setTag(100);
                this->addChild(bottle, 0);
                manager.AddTower(bottle);
            }
            firstMoney -= 100;//���½��
            grid_Map[row][col] = _BOTTLE;//���µ�ͼ
            grid_TowerMap[row][col] = bottle;
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
void FirstMapScene::onPickShitTouch(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        Vec2 blankPos = blank->getPosition();
        int col = static_cast<int>(blankPos.x / 60);
        int row = static_cast<int>(blankPos.y / 68);
        if (firstMoney >= 120) {
            AudioEngine::play2d("test/Build.ogg");
            auto shit = Shit::construct(blankPos, SHIT);
            if (shit) {
                shit->setTag(100);
                shit->setPosition(blankPos);
                this->addChild(shit, 0);
                manager.AddTower(shit);
            }
            firstMoney -= 120;
            grid_Map[row][col] = _SHIT;
            grid_TowerMap[row][col] = shit;
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
void FirstMapScene::onPickSnowTouch(Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
        Vec2 blankPos = blank->getPosition();
        int col = static_cast<int>(blankPos.x / 60);
        int row = static_cast<int>(blankPos.y / 68);
        if (firstMoney >= 180) {
            AudioEngine::play2d("test/Build.ogg");
            auto snow = Snow::construct(blankPos, SNOW);
            if (snow) {
                snow->setTag(100);
                snow->setPosition(blankPos);
                manager.AddTower(snow);
                this->addChild(snow, 0);
            }
            firstMoney -= 180;
            grid_Map[row][col] = _SNOW;
            grid_TowerMap[row][col] = snow;
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

bool FirstMapScene::init() {
    if (!Scene::init()) {
        return false;
    }
/*=================�������===============*/
    cocos2d::AudioEngine::stopAll();
    cocos2d::AudioEngine::play2d("test/PlayBGM.mp3", true);

    auto sprite = cocos2d::Sprite::create("test/SelectBG.png");//0�㣬����ͼƬ
    if (sprite == nullptr)
    {
        problemLoading("'test/SelectBG.png'");
    }
    else
    {
        sprite->setScale(1000.0f / sprite->getContentSize().width, 600.0f / sprite->getContentSize().height);//����ͼƬ��С��Ҫ�����п��Ӧ
        sprite->setPosition(cocos2d::Vec2(500, 300));
        this->addChild(sprite, 0);
    }

    auto path = cocos2d::Sprite::create("test/Path1.png");//0�㣬·��
    if (path == nullptr)
    {
        problemLoading("'test/Path1.png'");
    }
    else
    {
        path->setScale(1000.0f / path->getContentSize().width, 600.0f / path->getContentSize().height);//����ͼƬ��С��Ҫ�����п��Ӧ
        path->setPosition(cocos2d::Vec2(500, 300));
        this->addChild(path, 0);
    }

    auto upBoard = cocos2d::Sprite::create("test/UpBoard.png");//0�㣬״̬��
    if (upBoard == nullptr)
    {
        problemLoading("'test/UpBoard.png'");
    }
    else
    {
        upBoard->setPosition(cocos2d::Vec2(500, 571));
        this->addChild(upBoard, 0);
    }

    auto upBoard2 = cocos2d::Sprite::create("test/UpBoard2.png");//0�㣬������
    if (upBoard == nullptr)
    {
        problemLoading("'test/UpBoard2.png'");
    }
    else
    {
        upBoard2->setPosition(cocos2d::Vec2(537, 571));
        this->addChild(upBoard2, 0);
    }

    auto moneyLabel = createMoneyLabel();//0�㣬�����
    if (moneyLabel) {
        moneyLabel->setPosition(cocos2d::Vec2(200, 575));
        this->addChild(moneyLabel, 0);
    }
    else {
        CCLOG("Error creating Label");
    }

    auto roundLabel = createRoundLabel();//0�㣬������Ϣ
    if (roundLabel) {
        roundLabel->setPosition(cocos2d::Vec2(540, 575));
        this->addChild(roundLabel, 0);
    }
    else {
        CCLOG("Error creating Round Label");
    }
   
    auto speedUpItem1 = MenuItemImage::create("test/Speed_1.png", "test/Speed_1.png"); //�����ٰ�ť
    auto speedUpItem2 = MenuItemImage::create("test/Speed_2.png", "test/Speed_2.png");
    auto speedUp = MenuItemToggle::createWithCallback(CC_CALLBACK_1(FirstMapScene::changeSpeed, this), speedUpItem1, speedUpItem2, nullptr);
    if (speedUp) {
        speedUp->setPosition(Vec2(730, 570));
        auto menu = Menu::create(speedUp, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);
    }
    
    
    auto pauseItem1 = MenuItemImage::create("test/Pause.png", "test/Pause.png");//��ͣ��ť
    auto pauseItem2 = MenuItemImage::create("test/Restart.png", "test/Restart.png");
    auto pauseItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(FirstMapScene::pause, this), pauseItem1, pauseItem2, nullptr);
    if (pauseItem) {
        pauseItem->setPosition(Vec2(800, 570));
        auto menu = Menu::create(pauseItem, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu);
    }

    auto BackItem = cocos2d::MenuItemImage::create(//���ز˵���ť
        "test/BackNormal.png",
        "test/BackSelected.png",
        CC_CALLBACK_1(FirstMapScene::backToMain, this));
    if (BackItem) {
        BackItem->setPosition(cocos2d::Vec2(850, 575));
        auto menu = cocos2d::Menu::create(BackItem, nullptr); // �����˵�����ӷ��ذ�ť
        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
    }
    else {
        CCLOG("Failed to load back button"); // �������ʧ�ܣ����������Ϣ
    }
    //�ܲ�Ѫ��
    auto hpSprite = Carrot::createHP();
    hpSprite->setPosition(cocos2d::Vec2(810, 520));
    this->addChild(hpSprite);
    //�ܲ�
    carrot = Carrot::createCarrot();
    carrot->setPosition(cocos2d::Vec2(810, 485));
    this->addChild(carrot);
    //�ܲ�Ѫ��
    auto HPLabel = carrot->HPLabel;
    if (HPLabel) {
        HPLabel->setPosition(Vec2(800, 520));
        HPLabel->setTextColor(Color4B::BLACK);
        this->addChild(HPLabel);
    }
    carrot->schedule([this](float dt) {
        this->carrot->updateCarrotSprite();
        this->carrot->updateHPLabel();
        }, 0.1f, "update_carrot_hp");

    //������
    this->schedule(CC_SCHEDULE_SELECTOR(FirstMapScene::updateRoundLabel), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(FirstMapScene::updateMoneyLabel), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(FirstMapScene::updateWin), 0.1f);
    this->schedule(CC_SCHEDULE_SELECTOR(FirstMapScene::updateFail), 0.1f);

    //�ܲ�����
    auto hpButton = cocos2d::MenuItemImage::create("test/Update.png", "test/Update.png", [=](cocos2d::Ref* sender) {
        if (firstMoney >= 500) {
            carrot->hp += 1;
            firstMoney -= 500;
            carrot->updateCarrotSprite();//�޸��ܲ�ͼ��
            moneyLabel->setString(cocos2d::StringUtils::format("%d", firstMoney));//�޸�moneyLable
            cocos2d::AudioEngine::play2d("test/Select.mp3", false);
            cocos2d::AudioEngine::play2d("tower/Up.ogg", false);
        }
        else {
            cocos2d::AudioEngine::play2d("test/Wrong.mp3", false);
        }
        });
    if (hpButton) {
        hpButton->setPosition(cocos2d::Vec2(850, 470));
        auto menu = cocos2d::Menu::create(hpButton, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
        auto Label = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("500"), "fonts/Marker Felt.ttf", 10);
        if (Label) {
            Label->setPosition(cocos2d::Vec2(852, 455));
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

/*=====================���񻮷�=====================*/
    //��ͼ����ϰ��ﶼ�Ž�ȥ��Ϊ��������׼��
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 17; col++) {
            // ����ÿ�����ӵ�λ��
            float posX = col * 60 + 25;   // ���ӵ����� X ����
            float posY = row * 68 + 20; // ���ӵ����� Y ����
            if (row == 1 && col == 5) {
                col++;
            }
            if (row == 1 && col == 6) {
                posX += 30;
            }
            if (row == 3 && col == 9) {
                col++;
            }
            if (row == 3 && col == 10) {
                posX += 30;
            }
            if (row == 6 && col == 9) {
                col++;
            }
            if (row == 6 && col == 10) {
                col++;
            }
            if (row == 7 && col == 9) {
                col++;
            }
            if (row == 7 && col == 10) {
                posX += 30;
                posY -= 40;
            }

            std::string filename;
            switch (grid_Map[row][col]) {
            case _ICE:
                filename = "test/Ice.png";
                break;
            case _BIGICE:
                filename = "test/BigIce.png";
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
    //Ϊ��������׼��
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
    deleteButton = ui::Button::create("tower/80.png");
    this->addChild(deleteButton, 1);
    //�������ļ�����
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FirstMapScene::handleGridTouch, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


    /*=======================�����ǹ���=========================*/

    schedule(CC_SCHEDULE_SELECTOR(FirstMapScene::update), 1.0f);

    //����·����Ϣ
    CheckPoints.push_back(Vec2(804.f, 405.f));
    CheckPoints.push_back(Vec2(692.f, 405.f));
    CheckPoints.push_back(Vec2(692.f, 145.f));
    CheckPoints.push_back(Vec2(318.f, 145.f));
    CheckPoints.push_back(Vec2(318.f, 405.f));
    CheckPoints.push_back(Vec2(INITX1, INITY1));

    //���ﲨ����Ϣ
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

void FirstMapScene::StartWaves()
{
    SpawnWave(waves[0]);//��ʼ���ɹ���
}
void FirstMapScene::MonsterWave(const int type, const float blood, const int count, const float inr)
{
    std::map<int, std::function<Monster* ()>> monsterCreators;
    monsterCreators[NORMALMONSTER] = [&]() { return NormalMonster::create(blood, INITX1, INITY1); };
    monsterCreators[FASTMONSTER] = [&]() { return FastMonster::create(blood, INITX1, INITY1); };
    monsterCreators[BIGMONSTER] = [&]() { return BigMonster::create(blood, INITX1, INITY1); };
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

void FirstMapScene::SpawnWave(const WaveData& wave)
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
void FirstMapScene::NextWaveCallback()
{
    if (round < waves.size())
    {
        CCLOG("Current wave completed. Starting next wave.");//�����ʾ��һ��
        win = 1;
        isFirstMapWon = 1;
        round++; //�ִμ�һ
        if (win != 1) {
            SpawnWave(waves[round - 1]);// ������һ��
        }
    }
    else
    {
        win = 1;
        isFirstMapWon = 1;
        CCLOG("All waves completed.");//���в��ν���
    }
}



















