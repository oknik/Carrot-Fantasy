#include"Carrot.h"
#include"cocos2d.h"

//��ʼ��
bool Carrot::init() {
    if (!Sprite::initWithFile("test/Carrot_2.png")) {
        return false;
    }

    hp = 10;//��ʼ���ܲ�������
    HPLabel = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", hp), "fonts/Marker Felt.ttf", 10);//��ʼ����ʾ�ܲ��������ı�ǩ
    if (HPLabel) {
        HPLabel->setPosition(cocos2d::Vec2(800, 520));
        HPLabel->setTextColor(cocos2d::Color4B::BLACK);
    }

    this->schedule([this](float dt) {//ÿ0.1�����һ�θ����ܲ���ͼ�����ʾ�ܲ��������ı�ǩ
        this->updateCarrotSprite();
        this->updateHPLabel();
        }, 0.1f, "update_carrot_hp");

    return true;
}
//�����ܲ�carrot
Carrot* Carrot::createCarrot() {
    auto carrot = new Carrot();
    if (carrot && carrot->init()) {
        carrot->autorelease();//���Զ��ͷ��ڴ棬�����ֶ��ͷ��ڴ�
        return carrot;
    }
    CC_SAFE_DELETE(carrot);//��ȫ��ɾ����carrot
    return nullptr;
}
//������ʾ�ܲ��������ı�����hpBG
cocos2d::Sprite* Carrot::createHP() {
    auto hpBG = new Carrot();
    if (hpBG && hpBG->initWithFile("test/HP.png")) {
        hpBG->autorelease();
        return hpBG;
    }
    CC_SAFE_DELETE(hpBG);
    return nullptr;
}
//������ʾ�ܲ��������ı�ǩlabel������ʾcarrot->hp��ֵ
cocos2d::Label* Carrot::createHPLabel() {
    auto label = cocos2d::Label::createWithTTF(cocos2d::StringUtils::format("%d", hp), "fonts/Marker Felt.ttf", 10);
    if (!label) {
        CCLOG("Error creating HP Label");
    }
    return label;
}
//�����ܲ������������ܲ���ͼ��
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
//�����ܲ�������������ʾ�ܲ��������ı�ǩ
void Carrot::updateHPLabel() {
    if (HPLabel) {
        HPLabel->setString(cocos2d::StringUtils::format("%d", hp));
    }
}




