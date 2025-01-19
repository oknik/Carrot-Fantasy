// TowerManager.cpp
#include "TowerManager.h"

USING_NS_CC;


Vector<Tower*> TowerManager::towers;

void TowerManager::addTower(Tower* tower)
{
    towers.pushBack(tower);
}

void TowerManager::updateTowers(float dt)
{
    for (Tower* tower : towers)
    {
        //tower->updateTower();  // 你可能需要修改 Tower 类的 updateTower 函数，使其不再需要怪物管理器参数
    }
}

void TowerManager::clearTowers()
{
    for (Tower* tower : towers)
    {
        tower->removeFromParent();
    }
    towers.clear();
}