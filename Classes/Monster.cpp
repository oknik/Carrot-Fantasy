#include "Monster.h"
#include"Data.h"
#include "audio/include/AudioEngine.h"

void Monster::UpdateMonster(float dt)
{
	//实时更新怪物坐标
	m_x = monster->getPosition().x;
	m_y = monster->getPosition().y;
	CCLOG("m_x=%f, m_y=%f", m_x, m_y);
	//判断怪物是否需要更新路径
	if (is_start == true && is_end != true)
	{
		float tolerance = 1.0f;
		Vec2 monsterPosition = monster->getPosition();
		Vec2 currentpoint;
		if (!checkPoints.empty())
		{
			currentpoint = checkPoints.back();
			if (monsterPosition.distance(currentpoint) < tolerance && update_action == true)
			{
				checkPoints.pop_back();
				/*if (dir == LEFT && this->dir == R)
				{
					auto flipAction = ScaleTo::create(0.01f, -1.0f, 1.0f);
					monster->runAction(flipAction);
				}
				if (dir == RIGHT && this->dir == L)
				{
					auto flipAction = ScaleTo::create(0.01f, 1.0f, 1.0f);
					monster->runAction(flipAction);
				}
				if (dir == LEFT) { this->dir = L; }
				if (dir == RIGHT) { this->dir = R; }*/
				this->Move(checkPoints.back());
				update_action = false;
				scheduleOnce([this](float dt)
					{
						update_action = true;
					}, 0.05f, "CheckAgain");
			}
			if (is_decelerate)
			{
				monster->stopActionByTag(1);
				blood->stopActionByTag(2);
				this->Move(checkPoints.back());
				SlowEffect(checkPoints.back());
				scheduleOnce([this](float dt)
					{
						is_decelerate = false;
					}, 2.f, "Decelerate");
			}
			if (is_stop)
			{
				monster->stopActionByTag(1);
				blood->stopActionByTag(2);
				this->Move(checkPoints.back());
				schedule([this](float dt)
					{
						is_stop = false;
						this->Move(checkPoints.back());
					}, 1.f, "Stop");
			}
		}
	}
	//判断是否死亡
	if (isDead())
	{
		Dead();
		firstMoney += m_deadcost;
		secondMoney += m_deadcost;
		scheduleOnce([this](float dt)
			{
				this->RemoveMonster();;
			}, 0.35f, "remove_monster");
	}
	// 判断是否碰到萝卜
	Rect monsterRect = monster->getBoundingBox();
	Rect carrotRect = carrot->getBoundingBox();
	if (monsterRect.intersectsRect(carrotRect)) {
		carrot->hp -= m_attack;
		cocos2d::AudioEngine::play2d("monster/Bite.mp3");
		this->RemoveMonster();
	}
}
void Monster::GetHurt(float damage)
{
	c_blood -= damage;//当前血量减去伤害
	if (c_blood < 0)
	{
		c_blood = 0;
	}
	blood->setPercent(float(c_blood) / m_blood * 100);//更新血条
	blood->setVisible(true);
}
void Monster::Emerge(float dt)
{
	Vector <FiniteTimeAction*> vec;
	auto delay = DelayTime::create(dt);//延时出现
	vec.pushBack(delay);
	scheduleOnce([this](float st)
		{
			is_start = true;
			EmergeAnimate();
		}, dt, "monsterEmerge");
	monster->setOpacity(0);
	blood->setOpacity(0);
	auto in = FadeIn::create(0.1f);
	vec.pushBack(in);
	Sequence* sequence = Sequence::create(vec);
	monster->runAction(sequence);
	blood->runAction(sequence->clone());
	blood->setVisible(false);
}

void Monster::Move(Vec2 checkpoint)
{
	float distance = monster->getPosition().distance(checkpoint);  //计算怪物和检查点之间的距离
	MoveTo* monsterAction = MoveTo::create(distance / c_speed, checkpoint);
	MoveTo* bloodAction = MoveTo::create(distance / c_speed, Vec2(checkpoint.x, checkpoint.y + monster->getBoundingBox().size.height));
	monsterAction->setTag(1);
	bloodAction->setTag(2);
	monster->runAction(monsterAction);
	blood->runAction(bloodAction);
}
bool Monster::init(const std::string& spriteName, float init_x, float init_y, float scale, int speed, int attack, int deadCost)
{
	if (!Node::init())
	{
		return false;
	}
	monster = Sprite::create(spriteName);	// 创建怪物精灵
	if (!monster)
		CCLOG("Failed to load sprite: %s", spriteName.c_str());
	else
	{
		monster->setScale(scale);		//设置怪物大小
		monster->setPosition(init_x, init_y);	//设置位置
		monster->setAnchorPoint(Vec2(0.5, 0));// 设置锚点

		m_speed = speed;	// 设置速度
		c_speed = speed;	//设置当前速度
		m_attack = attack;	//设置攻击力
		m_deadcost = deadCost;	// 设置金币

		this->addChild(monster, 4);		//添加到父节点

		blood = ui::LoadingBar::create("monster/blood.png");// 创建血条

		if (!blood)
		{
			CCLOG("Failed to load sprite: monster/blood.png");
		}
		else
		{
			blood->setPercent(100);	// 初始血量为100%
			blood->setPosition(Vec2(init_x, init_y + monster->getBoundingBox().size.height));	//设置位置
			blood->setAnchorPoint(Vect(0.5, 0));		//设置锚点
			this->addChild(blood, 100);		//添加到父节点

			this->Animating();		//生成怪物动画

			//设置碰撞体
			//physicsbody = PhysicsBody::createBox(monster->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
			//PhysicsMaterial(0.1f, 1.0f, 0.0f) 是一个物理材质，包含了摩擦力、弹性系数和密度等参数。
			//这里设置了摩擦力为 0.1f，弹性系数为 1.0f，密度为 0.0f。
			//physicsbody->setDynamic(true);
			//将物体设置为动态物体，这意味着它会受到力的作用而产生运动。
			//physicsbody->setCategoryBitmask(0x01); 
			//设置物体的类别掩码。类别掩码是一个32位的整数，
			//用于标识物体所属的类别。这个值通常使用位运算来设置，以便在碰撞检测时判断物体之间的关系。
			//physicsbody->setCollisionBitmask(0x02);
			//设置物体的碰撞掩码。碰撞掩码定义了物体可以与哪些类别的物体发生碰撞。
			//physicsbody->setContactTestBitmask(0x03);
			//设置物体的接触测试掩码。接触测试掩码定义了物体可以检测哪些类别的物体。
		}
	}
	return true;
}
bool NormalMonster::init(float init_x, float init_y)
{
	if (!Node::init())
	{
		return false;
	}

	if (!Monster::init("monster/big1.png", init_x, init_y, 1.0f, NORMALSPEED, NORMALATTACK, NORMALMONEY))
	{
		return false;
	}
	return true;
}
bool BigMonster::init(float init_x, float init_y)
{
	if (!Node::init())
	{
		return false;
	}
	if (!Monster::init("monster/big1.png", init_x, init_y, 1.7f, SLOWSPEED, BIGATTACK, BIGMONEY))
	{
		return false;
	}
	return true;
}
bool FastMonster::init(float init_x, float init_y)
{
	if (!Node::init())
	{
		return false;
	}
	if (!Monster::init("monster/fast1.png", init_x, init_y, 1.0f, FASTSPEED, NORMALATTACK, NORMALMONEY))
	{
		return false;
	}
	return true;
}

void Monster::SlowEffect(Vec2 checkpoint)
{
	effect = Sprite::create("tower/shiteffect.png");
	effect->setVisible(true);
	this->addChild(effect);
	effect->setPosition(Vec2(m_x, m_y));
	float distance = monster->getPosition().distance(checkpoint);  //计算怪物和检查点之间的距离
	MoveTo* effectAction = MoveTo::create(distance / c_speed, checkpoint);
	effectAction->setTag(3);
	effect->runAction(effectAction);
}