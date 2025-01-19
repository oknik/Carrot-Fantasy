#include "Monster.h"
#include"Data.h"
#include "audio/include/AudioEngine.h"

void Monster::UpdateMonster(float dt)
{
	//ʵʱ���¹�������
	m_x = monster->getPosition().x;
	m_y = monster->getPosition().y;
	CCLOG("m_x=%f, m_y=%f", m_x, m_y);
	//�жϹ����Ƿ���Ҫ����·��
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
	//�ж��Ƿ�����
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
	// �ж��Ƿ������ܲ�
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
	c_blood -= damage;//��ǰѪ����ȥ�˺�
	if (c_blood < 0)
	{
		c_blood = 0;
	}
	blood->setPercent(float(c_blood) / m_blood * 100);//����Ѫ��
	blood->setVisible(true);
}
void Monster::Emerge(float dt)
{
	Vector <FiniteTimeAction*> vec;
	auto delay = DelayTime::create(dt);//��ʱ����
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
	float distance = monster->getPosition().distance(checkpoint);  //�������ͼ���֮��ľ���
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
	monster = Sprite::create(spriteName);	// �������ﾫ��
	if (!monster)
		CCLOG("Failed to load sprite: %s", spriteName.c_str());
	else
	{
		monster->setScale(scale);		//���ù����С
		monster->setPosition(init_x, init_y);	//����λ��
		monster->setAnchorPoint(Vec2(0.5, 0));// ����ê��

		m_speed = speed;	// �����ٶ�
		c_speed = speed;	//���õ�ǰ�ٶ�
		m_attack = attack;	//���ù�����
		m_deadcost = deadCost;	// ���ý��

		this->addChild(monster, 4);		//��ӵ����ڵ�

		blood = ui::LoadingBar::create("monster/blood.png");// ����Ѫ��

		if (!blood)
		{
			CCLOG("Failed to load sprite: monster/blood.png");
		}
		else
		{
			blood->setPercent(100);	// ��ʼѪ��Ϊ100%
			blood->setPosition(Vec2(init_x, init_y + monster->getBoundingBox().size.height));	//����λ��
			blood->setAnchorPoint(Vect(0.5, 0));		//����ê��
			this->addChild(blood, 100);		//��ӵ����ڵ�

			this->Animating();		//���ɹ��ﶯ��

			//������ײ��
			//physicsbody = PhysicsBody::createBox(monster->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
			//PhysicsMaterial(0.1f, 1.0f, 0.0f) ��һ��������ʣ�������Ħ����������ϵ�����ܶȵȲ�����
			//����������Ħ����Ϊ 0.1f������ϵ��Ϊ 1.0f���ܶ�Ϊ 0.0f��
			//physicsbody->setDynamic(true);
			//����������Ϊ��̬���壬����ζ�������ܵ��������ö������˶���
			//physicsbody->setCategoryBitmask(0x01); 
			//���������������롣���������һ��32λ��������
			//���ڱ�ʶ����������������ֵͨ��ʹ��λ���������ã��Ա�����ײ���ʱ�ж�����֮��Ĺ�ϵ��
			//physicsbody->setCollisionBitmask(0x02);
			//�����������ײ���롣��ײ���붨���������������Щ�������巢����ײ��
			//physicsbody->setContactTestBitmask(0x03);
			//��������ĽӴ��������롣�Ӵ��������붨����������Լ����Щ�������塣
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
	float distance = monster->getPosition().distance(checkpoint);  //�������ͼ���֮��ľ���
	MoveTo* effectAction = MoveTo::create(distance / c_speed, checkpoint);
	effectAction->setTag(3);
	effect->runAction(effectAction);
}