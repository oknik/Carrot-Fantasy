#pragma once
#ifndef _TOWER_H_
#define _TOWER_H_

#define BOTTLE 0
#define SHIT 1
#define SNOW 2

#include"cocos2d.h"
#include "Monster.h"

USING_NS_CC;
/***************************************
* 炮台的基类，包含了所有炮台的共同特性
****************************************/
class Tower :public Sprite {
public:
	Sprite* base;        //炮塔的底座，取决于炮塔种类
	Sprite* bullet;		//创建炮塔的子弹
	Vector<Monster*> target_monsters;		//炮塔攻击目标
	int tag;             //编号代表是哪个炮塔(0：bottle|1：shit|2：snow)
	int level;           //等级
	int construct_cost;  //建造所需金币，取决于炮塔种类
	int upgrade_cost;    //升级所需金币，取决于炮塔种类
	int attack_power;    //攻击强度,取决于炮塔等级
	float attack_range;    //攻击范围,取决于炮塔等级
	float bullet_speed = 100;	 //子弹的发射速度
	float bullet_interval=0.2f;		//子弹发射的间隔
	float lastShootTime=0.0f;	//炮塔上一次开炮的时间
public:
	virtual bool init(const std::string& filename, Tower* tower, Vec2 position, int choice);		//初始化	static Tower* construct(Vec2 position, int choice);
	int remove();  //删除炮塔
	virtual void upgrade(Tower* tower); //升级
	float getTime() { return lastShootTime; }
	void setTime(float time) { lastShootTime = time; }
	float getInterval() { return bullet_interval; }

	void attack(Vector<Monster*> monsters);
	virtual void updateBullet() = 0;
	virtual void shoot() = 0;
	virtual void getTargetMonster(Vector<Monster*> monsters) = 0;
	virtual void rotateToMonster() = 0;
	virtual void attackAnimation() = 0;
};

class Bottle :public Tower
{
public:
	static Bottle* construct(Vec2 position, int choice);
	void getTargetMonster(Vector<Monster*> monsters);
	void shoot();
	void rotateToMonster();
	void updateBullet();
	void attackAnimation();
};

class Shit :public Tower
{
public:
	static Shit* construct(Vec2 position, int choice);
	void getTargetMonster(Vector<Monster*> monsters);
	void shoot();
	void rotateToMonster() {};
	void updateBullet();
	void attackAnimation();
};

class Snow :public Tower
{
public:
	static Snow* construct(Vec2 position, int choice);
	void getTargetMonster(Vector<Monster*> monsters);
	void shoot();
	void rotateToMonster() {};
	void updateBullet();
	void attackAnimation();
};


#endif