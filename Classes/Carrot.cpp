#include"Carrot.h"
#include"cocos2d.h"

//初始化
bool Carrot::init() {
    if (!Sprite::initWithFile("test/Carrot_2.png")) {
        return false;
    }

    hp = 10;//初始化萝卜的生命
    HPLabel = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", hp), "fonts/Marker Felt.ttf", 10);//初始化显示萝卜的生命的标签
    if (HPLabel) {
        HPLabel->setPosition(cocos2d::Vec2(800, 520));
        HPLabel->setTextColor(cocos2d::Color4B::BLACK);
    }

    this->schedule([this](float dt) {//每0.1秒调用一次更新萝卜的图像和显示萝卜的生命的标签
        this->updateCarrotSprite();
        this->updateHPLabel();
        }, 0.1f, "update_carrot_hp");

    return true;
}
//创建萝卜carrot
Carrot* Carrot::createCarrot() {
    auto carrot = new Carrot();
    if (carrot && carrot->init()) {
        carrot->autorelease();//会自动释放内存，不用手动释放内存
        return carrot;
    }
    CC_SAFE_DELETE(carrot);//安全地删除掉carrot
    return nullptr;
}
//创建显示萝卜的生命的背景板hpBG
cocos2d::Sprite* Carrot::createHP() {
    auto hpBG = new Carrot();
    if (hpBG && hpBG->initWithFile("test/HP.png")) {
        hpBG->autorelease();
        return hpBG;
    }
    CC_SAFE_DELETE(hpBG);
    return nullptr;
}
//创建显示萝卜的生命的标签label，会显示carrot->hp的值
cocos2d::Label* Carrot::createHPLabel() {
    auto label = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", hp), "fonts/Marker Felt.ttf", 10);
    if (!label) {
        CCLOG("Error creating HP Label");
    }
    return label;
}
//根据萝卜的生命更新萝卜的图像
void Carrot::updateCarrotSprite() {
    if (hp >= 10) {
        this->setTexture("test/Carrot_2.png");
    }
    if (hp <= 9 && hp >= 7) {
        this->setTexture("test/Carrot_3.png");
    }
    if (hp <= 6 && hp >= 4) {
        this->setTexture("test/Carrot_4.png");
    }
    if (hp <= 3) {
        this->setTexture("test/Carrot_5.png");
    }
}
//根据萝卜的生命更新显示萝卜的生命的标签
void Carrot::updateHPLabel() {
    if (HPLabel) {
        HPLabel->setString(cocos2d::StringUtils::format("%d", hp));
    }
}




