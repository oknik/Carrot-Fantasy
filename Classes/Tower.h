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
* ��̨�Ļ��࣬������������̨�Ĺ�ͬ����
****************************************/
class Tower :public Sprite {
public:
	Sprite* base;        //�����ĵ�����ȡ������������
	Sprite* bullet;		//�����������ӵ�
	Vector<Monster*> target_monsters;		//��������Ŀ��
	int tag;             //��Ŵ������ĸ�����(0��bottle|1��shit|2��snow)
	int level;           //�ȼ�
	int construct_cost;  //���������ң�ȡ������������
	int upgrade_cost;    //���������ң�ȡ������������
	int attack_power;    //����ǿ��,ȡ���������ȼ�
	float attack_range;    //������Χ,ȡ���������ȼ�
	float bullet_speed = 100;	 //�ӵ��ķ����ٶ�
	float bullet_interval=0.2f;		//�ӵ�����ļ��
	float lastShootTime=0.0f;	//������һ�ο��ڵ�ʱ��
public:
	virtual bool init(const std::string& filename, Tower* tower, Vec2 position, int choice);		//��ʼ��	static Tower* construct(Vec2 position, int choice);
	int remove();  //ɾ������
	virtual void upgrade(Tower* tower); //����
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