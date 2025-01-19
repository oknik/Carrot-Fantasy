#ifndef __SECONDMAPSCENE_H__
#define __SECONDMAPSCENE_H__

#include"vector"
#include"cocos2d.h"
#include"Monster.h"
#include"Carrot.h"
#include "Manager.h"

//�ڶ��ŵ�ͼ�������ʼλ��
#ifndef INITX2
#define INITX2 510.f
#endif

#ifndef INITY
#define INITY2 90.f
#endif

class SecondMapScene : public cocos2d::Scene {
public:
    //int money = 200;
    int round = 0;
    bool win = 0;
    Carrot* carrot;
    cocos2d::Label* RoundLabel;
    cocos2d::Label* MoneyLabel;
    Manager manager;        //������
    std::vector<Vec2> CheckPoints;    //����
    std::vector<WaveData> waves;    //���ﲨ������

    Sprite* blank;
    ui::Button* pickBottle;
    ui::Button* pickShit;
    ui::Button* pickSnow;
    ui::Button* upgradeButton;
    ui::Button* deleteButton;

    enum GridType { _BOTTLE, _SHIT, _SNOW, _ICE, _STONE, _TREE, _HOUSE, _ICECREAM, _BIGTREE, _NO, _YES };

    int gridMap[9][17] = {
        { _NO, _NO, _NO, _NO, _YES, _YES, _YES, _YES, _YES, _YES, _YES, _NO, _NO, _NO, _NO, _NO, _NO},
        { _NO, _NO, _YES, _YES, _NO, _NO, _NO, _NO, _NO, _ICE, _YES, _ICE, _YES, _YES, _NO, _NO, _NO},
        { _NO, _NO, _YES, _NO, _NO, _YES, _ICE, _ICE, _HOUSE, _HOUSE, _NO, _NO, _NO, _TREE, _YES, _NO, _NO},
        { _NO, _NO, _ICECREAM, _NO, _HOUSE, _HOUSE, _ICE, _YES, _HOUSE, _HOUSE, _NO, _YES, _NO, _TREE, _YES, _NO, _NO},
        { _NO, _NO, _YES, _NO, _HOUSE, _HOUSE, _NO, _NO, _NO, _NO, _NO, _ICECREAM, _NO, _YES, _TREE, _NO, _NO},
        { _NO, _NO, _ICECREAM, _NO, _NO, _NO, _NO, _YES, _STONE, _STONE, _YES, _YES, _NO, _YES, _YES, _NO, _NO},
        { _NO, _NO, _YES, _BIGTREE, _BIGTREE, _YES, _BIGTREE, _BIGTREE, _YES, _YES, _BIGTREE, _BIGTREE, _NO, _YES, _YES, _NO, _NO},
        { _NO, _NO, _YES, _BIGTREE, _BIGTREE, _YES, _BIGTREE, _BIGTREE, _YES, _YES, _BIGTREE, _BIGTREE, _YES, _YES, _YES, _NO, _NO},
        { _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO, _NO},
    };

    Tower* gridTowerMap[9][17];

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
    void secondTryAgain(cocos2d::Ref* sender);
    void updateFail(float dt);
    void updateWin(float dt);
    bool handleGridTouch(Touch* touch, Event* event);
    void onPickBottleTouch(Ref* sender, ui::Widget::TouchEventType type);
    void onPickShitTouch(Ref* sender, ui::Widget::TouchEventType type);
    void onPickSnowTouch(Ref* sender, ui::Widget::TouchEventType type);

    CREATE_FUNC(SecondMapScene);

    /*=================�йع���ĺ���=============*/
    void StartWaves();//�ó������ο�ʼ
    //type ����  blood Ѫ��  count ����  inr ���ּ�� carrot ���ܲ����ɲ�������ȥ  road �����ƶ�·��
    void MonsterWave(const int type, const float blood, const int count, const float inr);
    void SpawnWave(const WaveData& wave);//����һ��
    void NextWaveCallback();    //�ص����ж��Ƿ�������һ��
};

#endif // __SECONDMAPSCENE_H__
