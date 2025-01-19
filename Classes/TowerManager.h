#pragma once
#include "Tower.h"

class TowerManager : public cocos2d::Node
{
private:
    static Vector<Tower*> towers;

public:
    ~TowerManager() { clearTowers(); }

    void addTower(Tower* tower);
    void updateTowers(float dt);
    bool isEmpty() const { return towers.empty(); }

    auto begin() { return towers.begin(); }
    auto end() { return towers.end(); }

    void clearTowers();
};

