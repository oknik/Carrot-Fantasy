//��һ�ŵ�ͼ����
#ifndef __FIRSTMAPSCENE_H__
#define __FIRSTMAPSCENE_H__

#include"vector"
#include"cocos2d.h"
#include"Monster.h"
#include"Carrot.h"
#include"Manager.h"
#include"Tower.h"

//�õ�ͼ�������λ��
#define INITX1 200.f
#define INITY1 405.f

struct WaveData
{
    int type;       //��������
    int count;      //���ﲨ��
    int health;     //��������ֵ
    float interval;    //���ּ��
    std::string name;   //���ﲨ�α�ǩ
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
    int round = 1;//����
    bool win = 0;//��Ӯ
    Carrot* carrot;//�ܲ�
    cocos2d::Label* RoundLabel;//���α�ǩ������������ڲ��ϸ���
    cocos2d::Label* MoneyLabel;//��ұ�ǩ
    Manager manager;//������
    std::vector<Vec2> CheckPoints;//����
    std::vector<WaveData> waves;//���ﲨ������

    Sprite* blank;//Ҫ���������Ǹ�����
    ui::Button* pickBottle;//ѡ����ƿ�ӵİ�ť
    ui::Button* pickShit;//ѡ���ֱ��İ�ť
    ui::Button* pickSnow;//ѡ����ѩ���İ�ť
    ui::Button* upgradeButton;//ѡ�������İ�ť
    ui::Button* deleteButton;//ѡ�������İ�ť

    enum Grid_Type { _BOTTLE, _SHIT, _SNOW, _ICE, _BIGICE, _TREE, _HOUSE, _ICECREAM, _NO, _YES };//ö�ٳ���ͼ�����п��ܵ���������������ֵĵط��������ֵĵط����ϰ���

    int grid_Map[9][17] = {//��ͼ����
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

    Tower* grid_TowerMap[9][17];//һ��Tower*��ĵ�ͼ���飬���Դӵ���õ�ĳ����������������в���

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

    /*=================�йع���ĺ���=============*/    
    void StartWaves();//�ó������ο�ʼ
    //type ����  blood Ѫ��  count ����  inr ���ּ�� carrot ���ܲ����ɲ�������ȥ  road �����ƶ�·��
    void MonsterWave(const int type, const float blood, const int count, const float inr);
    void SpawnWave(const WaveData& wave);//����һ��
    void NextWaveCallback();    //�ص����ж��Ƿ�������һ��
};

#endif // __FIRSTMAPSCENE_H__
