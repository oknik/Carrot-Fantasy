//萝卜基类Carrot
#ifndef __CARROT_H__
#define __CARROT_H__

#include "cocos2d.h"

class Carrot : public cocos2d::Sprite {
public:
    int hp; //萝卜的生命值
    cocos2d::Label* HPLabel;//显示萝卜的生命值的标签

    virtual bool init();
    static Carrot* createCarrot();
    static cocos2d::Sprite* createHP();
    void updateCarrotSprite(); 
    cocos2d::Label* createHPLabel();
    void updateHPLabel();

    CREATE_FUNC(Carrot);

};

#endif // __CARROT_H__
