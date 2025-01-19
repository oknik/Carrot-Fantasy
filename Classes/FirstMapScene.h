//第一张地图的类
#ifndef __FIRSTMAPSCENE_H__
#define __FIRSTMAPSCENE_H__

#include"vector"
#include"cocos2d.h"
#include"Monster.h"
#include"Carrot.h"
#include"Manager.h"
#include"Tower.h"

//该地图怪物入口位置
#define INITX1 200.f
#define INITY1 405.f

struct WaveData
{
    int type;       //怪物种类
    int count;      //怪物波次
    int health;     //怪物生命值
    float interval;    //出现间隔
    std::string name;   //怪物波次标签
    WaveData(int type, int count, int health, float interval, const std::string& name)
    {
        this->type = type;
        this->count = count;
        this->health = health;
        this->interval = interval;
        this->name = name;
    }
};

class FirstMapScene : public cocos2d::Scene {
public:
    int round = 1;//波次
    bool win = 0;//输赢
    Carrot* carrot;//萝卜
    cocos2d::Label* RoundLabel;//波次标签，放在这个便于不断更新
    cocos2d::Label* MoneyLabel;//金币标签
    Manager manager;//管理器
    std::vector<Vec2> CheckPoints;//检查点
    std::vector<WaveData> waves;//怪物波次数据

    Sprite* blank;//要种炮塔的那个精灵
    ui::Button* pickBottle;//选择种瓶子的按钮
    ui::Button* pickShit;//选择种便便的按钮
    ui::Button* pickSnow;//选择种雪花的按钮
    ui::Button* upgradeButton;//选择升级的按钮
    ui::Button* deleteButton;//选择卖出的按钮

    enum Grid_Type { _BOTTLE, _SHIT, _SNOW, _ICE, _BIGICE, _TREE, _HOUSE, _ICECREAM, _NO, _YES };//枚举出地图中所有可能的情况，包括不能种的地方、可以种的地方、障碍物

    int grid_Map[9][17] = {//地图数组
        { _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO},
        { _NO, _NO, _NO, _NO, _NO, _BIGICE, _BIGICE, _YES, _YES, _ICE, _YES, _YES, _YES, _NO, _NO, _NO, _NO},
        { _NO, _NO, _YES, _YES, _YES, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _YES, _YES, _YES, _NO, _NO},
        { _NO, _NO, _YES, _YES, _TREE, _NO, _ICECREAM, _YES, _YES, _BIGICE, _BIGICE, _NO, _TREE, _YES, _YES, _NO, _NO},
        { _NO, _NO, _YES, _YES, _YES, _NO, _YES, _YES, _ICE, _YES, _YES, _NO, _YES, _YES, _YES, _NO, _NO},
        { _NO, _NO, _YES, _TREE, _YES, _NO, _ICE, _ICE, _YES, _ICE, _YES, _NO, _YES, _TREE, _YES, _NO, _NO},
        { _NO, _NO, _YES, _NO, _NO, _NO, _YES, _YES, _YES, _HOUSE, _HOUSE, _NO, _NO, _NO, _YES, _NO, _NO},
        { _NO, _NO, _YES, _YES, _ICE, _YES, _ICE, _YES, _YES, _HOUSE, _HOUSE, _YES, _YES, _YES, _YES, _NO, _NO},
        { _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO},
    };

    Tower* grid_TowerMap[9][17];//一个Tower*类的地图数组，可以从地面得到某个格子里的炮塔进行操作

    static cocos2d::Scene* createScene();
    virtual bool init();
    cocos2d::Label* createRoundLabel();
    void updateRoundLabel(float dt);
    cocos2d::Label* createMoneyLabel();
    void updateMoneyLabel(float dt);
    void backToMain(cocos2d::Ref* sender);
    void changeSpeed(cocos2d::Ref* sender);
    void pause(cocos2d::Ref* sender);
    void removeAllMonster();
    void removeAllTower();
    void addMonsterToScene(Monster* monster)
    {
        manager.AddMonster(monster);
        monster->SetCarrot(carrot);
        monster->setCheckPoints(CheckPoints);
        addChild(monster, 10);
    }
    void firstTryAgain(cocos2d::Ref* sender);
    void updateFail(float dt);
    void updateWin(float dt);
    bool handleGridTouch(Touch* touch, Event* event);
    void onPickBottleTouch(Ref* sender, ui::Widget::TouchEventType type);
    void onPickShitTouch(Ref* sender, ui::Widget::TouchEventType type);
    void onPickSnowTouch(Ref* sender, ui::Widget::TouchEventType type);

    CREATE_FUNC(FirstMapScene);

    /*=================有关怪物的函数=============*/    
    void StartWaves();//该场景波次开始
    //type 种类  blood 血量  count 个数  inr 出现间隔 carrot 把萝卜当成参数传进去  road 怪物移动路径
    void MonsterWave(const int type, const float blood, const int count, const float inr);
    void SpawnWave(const WaveData& wave);//生成一波
    void NextWaveCallback();    //回调，判断是否生成下一波
};

#endif // __FIRSTMAPSCENE_H__
