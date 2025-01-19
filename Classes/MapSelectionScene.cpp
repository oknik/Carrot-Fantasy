//ѡ���ͼ����
#include"audio/include/AudioEngine.h"
#include"Data.h"
#include"HelloWorldScene.h"
#include"MapSelectionScene.h"
#include"FirstMapScene.h"
#include"SecondMapScene.h"

//��������
cocos2d::Scene* MapSelectionScene::createScene() {
    cocos2d::AudioEngine::play2d("test/Select.mp3");
    auto scene = MapSelectionScene::create();
    return scene;
}
//������ҳ��
void MapSelectionScene::backToMain(cocos2d::Ref* sender) {
    int selectId = cocos2d::AudioEngine::play2d("test/Select.mp3", false);
    this->schedule([=](float dt) {
        if (cocos2d::AudioEngine::getState(selectId) != cocos2d::AudioEngine::AudioState::PLAYING) {
            cocos2d::AudioEngine::stopAll(); 
            this->unschedule("checkAudioFinished");
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.0f, HelloWorld::createScene()));
        }
        }, "checkAudioFinished");
}
//��һ�ŵ�ͼ
void MapSelectionScene::gotoFirstMap(cocos2d::Ref* sender) {
    int selectId = cocos2d::AudioEngine::play2d("test/Select.mp3", false);
    this->schedule([=](float dt) {
        if (cocos2d::AudioEngine::getState(selectId) != cocos2d::AudioEngine::AudioState::PLAYING) {
            cocos2d::AudioEngine::stopAll();
            this->unschedule("checkAudioFinished");
            auto firstMapScene = FirstMapScene::createScene();
            firstMapScene->setTag(111);
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.0f, FirstMapScene::createScene()));
        }
        }, "checkAudioFinished");
}
//�ڶ��ŵ�ͼ
void MapSelectionScene::gotoSecondMap(cocos2d::Ref* sender) {
    int selectId = cocos2d::AudioEngine::play2d("test/Select.mp3", false);
    this->schedule([=](float dt) {
        if (cocos2d::AudioEngine::getState(selectId) != cocos2d::AudioEngine::AudioState::PLAYING) {
            cocos2d::AudioEngine::stopAll();
            this->unschedule("checkAudioFinished");
            auto secondMapScene = SecondMapScene::createScene();
            cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(1.0f, SecondMapScene::createScene()));
        }
        }, "checkAudioFinished");
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MapSelectionScene::init() {

    if (!Scene::init()) {
        return false;
    }

    float currentTimeScale = cocos2d::Director::getInstance()->getScheduler()->getTimeScale();
    if (currentTimeScale != 1.0f) {
        cocos2d::Director::getInstance()->getScheduler()->setTimeScale(1.0f);
    }
    //����
    auto sprite = cocos2d::Sprite::create("test/SelectBG.png");
    if (sprite) {
        sprite->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
        sprite->setScale(1000.0f / sprite->getContentSize().width, 600.0f / sprite->getContentSize().height);//����ͼƬ��С��Ҫ�����п��Ӧ
        this->addChild(sprite);
    }
    else {
        CCLOG("Failed to load sprite");
    }
    //����
    auto label = cocos2d::Label::createWithTTF("Map Selection", "fonts/Marker Felt.ttf", 48);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setPosition(500, 550);//��ǩ��λ��
        this->addChild(label, 1);
    }

    //�������ذ�ť
    auto BackItem = cocos2d::MenuItemImage::create(
        "test/BackNormal.png",
        "test/BackSelected.png",
        CC_CALLBACK_1(MapSelectionScene::backToMain, this));
    if (BackItem) {
        BackItem->setPosition(cocos2d::Vec2(100, 100));
        auto menu = cocos2d::Menu::create(BackItem, nullptr); 
        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
    }
    else {
        CCLOG("Failed to load back button"); 
    }

    //������ͼ��ť
    auto FirstMapButton = cocos2d::MenuItemImage::create(
        "test/Map1Normal.png",   
        "test/Map1Selected.jpg", 
        CC_CALLBACK_1(MapSelectionScene::gotoFirstMap, this)); 

    if (FirstMapButton) {
        FirstMapButton->setPosition(cocos2d::Vec2(250, 300));
        auto menu = cocos2d::Menu::create(FirstMapButton, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
    }
    else {
        CCLOG("Failed to load map button"); 
    }

    auto SecondMapButton = cocos2d::MenuItemImage::create(
        "test/Map2Normal.png", // ����״̬��ͼƬ
        "test/Map2Selected.jpg", // ���ʱ״̬��ͼƬ
        CC_CALLBACK_1(MapSelectionScene::gotoSecondMap, this));

    if (!isFirstMapWon) {
        // �����һ����ͼδ��Ӯ�ã��ڶ�����ͼ��ť���ֱ�����״̬
        SecondMapButton->setEnabled(false); // ���ð�ť
        SecondMapButton->setOpacity(128); // ����͸��������ʾ��������״̬
    }

    if (SecondMapButton) {
        SecondMapButton->setPosition(cocos2d::Vec2(750, 300));
        auto menu = cocos2d::Menu::create(SecondMapButton, nullptr);
        menu->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(menu);
    }
    else {
        CCLOG("Failed to load map button"); 
    }

    return true;
}
