#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "Tower.h"

USING_NS_CC;

class Manager :public Node
{
private:
	static Vector<Monster*> monsters;
	static Vector<Tower*> towers;
public:
	~Manager() { monsters.clear(); towers.clear(); }
	void AddMonster(Monster* monster)
	{
		monsters.pushBack(monster);
	}
	void AddTower(Tower* tower)
	{
		towers.pushBack(tower);
	}
	void Update(float dt);
	bool CheckWave();
};