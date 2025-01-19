#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"audio/include/AudioEngine.h"
#include "Carrot.h"
#include <vector>

USING_NS_CC;

#ifndef __MONSTER_SCENE_H__
#define __MONSTER_SCENE_H__

//怪物有不同类型，用一下宏定义
#define NORMALMONSTER 0
#define FASTMONSTER 1
#define BIGMONSTER 2
//怪物有不同速度
#define SLOWSPEED  40
#define NORMALSPEED 70
#define FASTSPEED 150
//怪物咬萝卜会掉不同的血
#define NORMALATTACK 1
#define BIGATTACK 2
//怪物会爆金币
#define NORMALMONEY 14
#define BIGMONEY 168
//方向的宏定义
#define RIGHT 'r'
#define LEFT 'l'
#define UP 'u'
#define DOWN 'd'
//怪物朝向
enum { L, R };



//Monster基类的声明
class Monster;
//NormalMonster类的声明
class NormalMonster;
//BigMonster类的声明
class BigMonster;
//FastMonster类的声明
class FastMonster;
//Monster基类的定义

class Monster :public Node
{
private:
	//通过Path生成动作实例

protected:
	float m_blood;		//满血血量
	float c_blood;		//当前血量	
	int m_speed;		//速度
	int c_speed;		//当前速度
	float m_x, m_y;		//怪物坐标
	bool dir = R;		//怪物面朝的方向
	int m_attack;		//咬萝卜掉多少血
	int m_deadcost;		//挂了爆多少金币
	bool is_start = false;	//该怪物是否已经出现
	bool is_end = false;	//该怪物是否应该消失
	bool update_action = true;		//怪物是否需要更新动作
	bool is_decelerate = false;		//怪物是否处于减速状态
	bool is_stop = false;			//怪物是否处于冰冻状态

	Sprite* monster;	//怪物
	ui::LoadingBar* blood;	//血条
	Carrot* carrot;		//萝卜
	Sprite* effect;		//特效
	std::vector<Vec2> checkPoints;	//检查点
	//PhysicsBody* physicsbody;		//碰撞体
public:
	//SpriteName：图片名称	init_x：起点x坐标	init_y：起点y坐标	scale：大小	speed：速度	attack：攻击力	deadCost：金币
	bool init(const std::string& spriteName, float init_x, float init_y, float scale, int speed, int attack, int deadCost);
	//更新怪物状态
	void UpdateMonster(float dt);
	//怪物受伤
	void GetHurt(float damage);
	//判断怪物是否死亡
	bool isDead() { return c_blood <= 0; }
	//判断怪物是否已经出现
	bool isStart() { return is_start; }
	//判断怪物是否应该消失
	bool isEnd() { return is_end; }
	//怪物出现
	void Emerge(float dt);
	//怪物移动
	void Move(Vec2 checkpoint);
	//怪物减速
	void SlowSpeed() {
		is_decelerate = true;
		if (c_speed == m_speed) {
			c_speed = m_speed * 0.5f;
		}
		scheduleOnce([this](float dt)
			{
				c_speed = m_speed;
				effect->setVisible(false);
				effect->removeFromParent();
			}, 2.f, "ReviveSpeed");
	}
	//怪物减速特效
	void SlowEffect(Vec2 checkpoint);
	//怪物停止
	void Stop()
	{
		is_stop = 1;
		c_speed = 0.01;
		scheduleOnce([this](float dt)
			{
				c_speed = m_speed;
			}, 2.f, "ReviveSpeed");
	}
	//设置检查点
	void setCheckPoints(std::vector<Vec2> checkpoints) { checkPoints = checkpoints; }
	//返回怪物的坐标
	Vec2 Position() { return Vec2(m_x, m_y); }
	//设置萝卜
	void SetCarrot(Carrot* _carrot) { carrot = _carrot; }
	//删除怪物
	void RemoveMonster()
	{
		is_end = true;
	}
	//怪物出现
	void EmergeAnimate()
	{
		//出现动画没有资源，那个资源我好像不太能用
		Vector<SpriteFrame*> emerge;
		emerge.pushBack(emerge);
		Animation* animation = Animation::createWithSpriteFrames(emerge, 0.1f);
		cocos2d::Animate* animate = Animate::create(animation);
		monster->runAction(animate);
		//播放怪物出现音效
		AudioEngine::play2d("monster/emerge.ogg", false);
	}
	//怪物的动画
	virtual void Animating() = 0;
	//怪物的死亡动画
	void DeadAnimate()
	{
		//创建动画
		Vector<SpriteFrame*> deadFrames;
		deadFrames.pushBack(SpriteFrame::create("monster/dead1.png", Rect(0, 0, 128, 128)));
		deadFrames.pushBack(SpriteFrame::create("monster/dead2.png", Rect(0, 0, 128, 128)));
		deadFrames.pushBack(SpriteFrame::create("monster/dead3.png", Rect(0, 0, 128, 128)));
		deadFrames.pushBack(SpriteFrame::create("monster/dead4.png", Rect(0, 0, 128, 128)));
		deadFrames.pushBack(SpriteFrame::create("monster/dead5.png", Rect(0, 0, 128, 128)));
		deadFrames.pushBack(SpriteFrame::create("monster/dead6.png", Rect(0, 0, 128, 128)));
		deadFrames.pushBack(SpriteFrame::create("monster/dead7.png", Rect(0, 0, 128, 128)));
		Animation* animation = Animation::createWithSpriteFrames(deadFrames, 0.3f);
		Animate* animate = Animate::create(animation);
		monster->runAction(RepeatForever::create(animate));
	}
	//怪物死亡
	virtual void Dead() = 0;
};

//普通怪物
class NormalMonster :public Monster
{
public:
	//构造函数
	NormalMonster(float health) { m_blood = health; c_blood = health; }
	//创建怪物
	static NormalMonster* create(float health, float init_x, float init_y) {
		NormalMonster* pRet = new(std::nothrow) NormalMonster(health);
		if (pRet && pRet->init(init_x, init_y)) {

			pRet->autorelease();
			return pRet;
		}
		else {
			delete pRet; pRet = nullptr; return nullptr;
		}
	};
	//初始化
	bool init(float init_x, float init_y);
	//怪物死亡
	void Dead()
	{
		DeadAnimate();
		is_end = true;
		AudioEngine::play2d("monster/mon2.ogg", false);//播放音效
	}
	//怪物本身的动画
	void Animating()
	{
		Vector<SpriteFrame*> animFrames;
		animFrames.pushBack(SpriteFrame::create("monster/normal1.png", Rect(0, 0, 68, 61)));
		animFrames.pushBack(SpriteFrame::create("monster/normal2.png", Rect(0, 0, 62, 61)));
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		Animate* animate = Animate::create(animation);
		monster->runAction(RepeatForever::create(animate));
	}

};

//大怪物
class BigMonster :public Monster
{
public:
	//构造函数
	BigMonster(float health) { m_blood = health; c_blood = health; }
	//创建怪物
	static BigMonster* create(float health, float init_x, float init_y) {
		BigMonster* pRet = new(std::nothrow) BigMonster(health);
		if (pRet && pRet->init(init_x, init_y)) {
			pRet->autorelease();
			return pRet;
		}
		else {
			delete pRet; pRet = nullptr; return nullptr;
		}
	};
	//初始化
	bool init(float init_x, float init_y);
	//怪物死亡
	void Dead()
	{
		DeadAnimate();
		is_end = true;
		AudioEngine::play2d("monster/mon6.ogg", false);//播放音效
	}
	//怪物本身的动画
	void Animating()
	{
		Vector<SpriteFrame*> animFrames;
		animFrames.pushBack(SpriteFrame::create("monster/big1.png", Rect(0, 0, 51, 54)));
		animFrames.pushBack(SpriteFrame::create("monster/big2.png", Rect(0, 0, 46, 54)));
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		Animate* animate = Animate::create(animation);
		monster->runAction(RepeatForever::create(animate));
	}
};

//跑得快的怪物
class FastMonster :public Monster
{
public:
	//构造函数
	FastMonster(float health) { m_blood = health; c_blood = health; }
	//创建怪物
	static FastMonster* create(float health, float init_x, float init_y) {
		FastMonster* pRet = new(std::nothrow) FastMonster(health);
		if (pRet && pRet->init(init_x, init_y)) {
			pRet->autorelease(); return pRet;
		}
		else {
			delete pRet; pRet = nullptr; return nullptr;
		}
	};
	//初始化
	bool init(float init_x, float init_y);
	//怪物死亡
	void Dead()
	{
		DeadAnimate();
		is_end = true;
		AudioEngine::play2d("monster/mon1.ogg", false);//播放音效
	}
	//怪物本身的动画
	void Animating()
	{
		Vector<SpriteFrame*> animFrames;
		animFrames.pushBack(SpriteFrame::create("monster/fast1.png", Rect(0, 0, 48, 51)));
		animFrames.pushBack(SpriteFrame::create("monster/fast2.png", Rect(0, 0, 50, 51)));
		Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		Animate* animate = Animate::create(animation);
		monster->runAction(RepeatForever::create(animate));
	}
};

#endif