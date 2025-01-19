//管理怪物
#include "Manager.h"

void Manager::Update(float dt)
{
    Vector<Monster*> monstersToRemove;  // 用于保存需要删除的怪物
    for (auto monster : monsters)
    {
        if (monster && !monster->isEnd())
        {
            monster->UpdateMonster(dt);
        }
        else
        {
            monstersToRemove.pushBack(monster);
        }
    }
    for (auto monster : monstersToRemove)// 一次性删除需要删除的怪物
    {
        monsters.eraseObject(monster);
        monster->removeFromParentAndCleanup(true);
    }
    float currentTime = Director::getInstance()->getTotalFrames() * Director::getInstance()->getAnimationInterval();
    for (auto tower : towers)
    {
        if (currentTime - tower->getTime() >= tower->getInterval())
        {
            tower->attack(monsters);
            tower->setTime(currentTime);
        }
    }
}

bool Manager::CheckWave()
{
    if (monsters.empty())   //波次结束
    {
        return true;
    }
    else        //波次未结束
    {
        return false;
    }
}

Vector<Monster*> Manager::monsters;
Vector<Tower*> Manager::towers;