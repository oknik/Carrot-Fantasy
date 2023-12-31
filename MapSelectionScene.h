#pragma once

#ifndef __MAPSELECTIONSCENE_H__
#define __MAPSELECTIONSCENE_H__

#include "cocos2d.h"

class MapSelectionScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void backToMain(cocos2d::Ref* sender);
    void gotoFirstMap(cocos2d::Ref* sender);
    void gotoSecondMap(cocos2d::Ref* sender);
    CREATE_FUNC(MapSelectionScene);
};

#endif // __MAPSELECTIONSCENE_H__
