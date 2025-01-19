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
        //tower->updateTower();  // �������Ҫ�޸� Tower ��� updateTower ������ʹ�䲻����Ҫ�������������
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