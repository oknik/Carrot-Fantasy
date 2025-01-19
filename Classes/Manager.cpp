//�������
#include "Manager.h"

void Manager::Update(float dt)
{
    Vector<Monster*> monstersToRemove;  // ���ڱ�����Ҫɾ���Ĺ���
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
    for (auto monster : monstersToRemove)// һ����ɾ����Ҫɾ���Ĺ���
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
    if (monsters.empty())   //���ν���
    {
        return true;
    }
    else        //����δ����
    {
        return false;
    }
}

Vector<Monster*> Manager::monsters;
Vector<Tower*> Manager::towers;