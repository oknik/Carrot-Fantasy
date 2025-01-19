#include"Tower.h"
#include"cocos2d.h"

USING_NS_CC;

#define BOTTLE 0
#define SHIT   1
#define SNOW   2

/*************************一堆各炮塔、各等级的特性参数*************************/
const std::string chooseTower[3] = { "tower/pickBottle.png","tower/pickshit.png","tower/picksnow.png" };//建造炮塔时需要的图片
const std::string Towers1[3] = { "tower/bottle1.png","tower/shit1.png","tower/snow1.png" };//-----------------------------储存炮塔名称
const std::string Towers2[3] = { "tower/bottle2.png","tower/shit2.png","tower/snow2.png" };//-----------------------------储存炮塔名称
const std::string Towers3[3] = { "tower/bottle3.png","tower/shit3.png","tower/snow3.png" };//-----------------------------储存炮塔名称
const std::string bases[3] = { "tower/bottlebase.png","tower/shitbase.png","tower/snowbase.png" };//----------------------各个炮塔的底座
const std::string bullets[3] = { "tower/tBottle-6.png","tower/tShit-1.png","tower/tSnow-1.png" };//-----------------------各个炮塔的子弹
const std::string upgrade_tower[3] = { "tower/bottle","tower/shit","tower/snow" };
const int construct_costs[3] = { 100,120,160 };//-----------------------------------------建造炮塔所需的金币
const int upgrade_costs[3][2] = { {180,220},
                                  {220,260},
                                  {260,320} };//----------------------------------------升级炮塔所需的金币
const int delete_costs[3][3] = { {80,176,224},
                                 {96,208,384},
                                 {128,352,432} };
const int attack_powers[3][3] =               //--------------------------------------------各等级攻击时的强度
{ {5,10,15},
 {4,7,10} ,
 {70,3,4} };
const float attack_ranges[3][3] =           //--------------------------------------------各等级攻击的范围
{ {100,200,300},
{100,120,150} ,
{130,200,350} };
const float bullet_speeds[3] = { 500,200,0 };		//子弹的发射速度
const float bullet_intervals[3][3] =                 //子弹发射的间隔
{ {0.2,0.15,0.12},
{1,0.9,0.8},
{5,1,0.8} };


bool Tower::init(const std::string& filename, Tower* tower, Vec2 position, int choice)
{
    int tag = 1;
    if (!Sprite::initWithFile(filename))
        return false;
    AudioEngine::play2d("tower/Build.ogg");
    //tower->base->setTexture("tower/bottlebase.png");//--------------------初始化炮台底座
    tower->level = 0;//------------------------------------------初始化等级为0级
    tower->tag = choice;//---------------------------------------初始化炮塔标签，代表它是瓶子/便便/雪花
    tower->construct_cost = construct_costs[choice];//-----------初始化炮塔建造所需金币
    tower->upgrade_cost = upgrade_costs[choice][tower->level];//-初始化升级炮塔所需金币
    tower->attack_power = attack_powers[tower->tag][tower->level];//---------初始化攻击强度
    tower->attack_range = attack_ranges[tower->tag][tower->level];//---------初始化攻击范围
    tower->bullet_speed = bullet_speeds[tower->tag];//---------初始化子弹速度
    tower->bullet_interval = bullet_intervals[tower->tag][tower->level];//------------初始化攻击间隔
    //tower->base->setPosition(position);//------------------------底座置于指定位置
    tower->setPosition(position);//------------------------------炮台置于指定位置

    return true;
}

Bottle* Bottle::construct(Vec2 position, int choice)
{
    Bottle* bottle = new Bottle();
    //成功建造一个炮塔，对其进行初始化
    if (bottle && bottle->init(Towers1[choice], bottle, position, choice))
    {
        return bottle;
    }
    return nullptr;
}
Snow* Snow::construct(Vec2 position, int choice)
{
    Snow* snow = new Snow();
    //成功建造一个炮塔，对其进行初始化
    if (snow && snow->init(Towers1[choice], snow, position, choice))
    {
        return snow;
    }
    return nullptr;
}
Shit* Shit::construct(Vec2 position, int choice)
{
    Shit* shit = new Shit();
    //成功建造一个炮塔，对其进行初始化
    if (shit && shit->init(Towers1[choice], shit, position, choice))
    {
        return shit;
    }
    return nullptr;
}

int Tower::remove() {
    int _tag = this->tag;
    int _level = this->level;
    AudioEngine::play2d("tower/delete.mp3");
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);

    return delete_costs[_tag][_level];
}

void Tower::upgrade(Tower* tower)
{
    int _tag = this->tag;
    int _level = this->level;

    if (_tag == BOTTLE) {
        if (_level == 0)
            this->setTexture("tower/bottle2.png");
        if (_level == 1)
            this->setTexture("tower/bottle3.png");
    }
    if (_tag == SHIT) {
        if (_level == 0)
            this->setTexture("tower/shit2.png");
        if (_level == 1)
            this->setTexture("tower/shit3.png");
    }
    if (_tag == SNOW) {
        if (_level == 0)
            this->setTexture("tower/snow2.png");
        if (_level == 1)
            this->setTexture("tower/snow3.png");
    }
    /*if (this) {
        if (this->tag == BOTTLE) {
            if (this->level == 0)
                this->bullet->setTexture("tower/bottle2bullet.png");
            if (this->level == 1)
                this->bullet->setTexture("tower/bottle3bullet.png");
        }
        if (this->tag == SHIT) {
            if (this->level == 0)
                this->bullet->setTexture("tower/shit2bullet.png");
            if (this->level == 1)
                this->bullet->setTexture("tower/shit3bullet.png");
        }
    }*/
    tower->level++;//-------------------------------------------------炮塔级数+1
    tower->upgrade_cost = upgrade_costs[tower->tag][tower->level];//--更新升级炮塔所需金币
    tower->attack_power = attack_powers[tower->tag][tower->level];//--------------更新攻击强度
    tower->attack_range = attack_ranges[tower->tag][tower->level];//--------------更新攻击范围}
}
void Tower::attack(Vector<Monster*> monsters)
{
    getTargetMonster(monsters);
    if (!target_monsters.empty())
    {
        rotateToMonster();
        shoot();
    }
}
/*=============瓶子攻击==============*/
void Bottle::getTargetMonster(Vector<Monster*> monsters)
{
    target_monsters.clear();
    for (Monster* monster : monsters)
    {
        float distance = this->getPosition().distance(monster->Position());  //计算炮塔和怪物的距离
        if (distance < attack_range && monster->isStart() == true && monster->isEnd() == false)
        {
            target_monsters.pushBack(monster);
            break;
        }
    }
}
void Bottle::shoot() {
    if (!target_monsters.empty())
    {
        Monster* targetMonster = target_monsters.back();
        bullet = Sprite::create(bullets[tag]);
        bullet->setScale(0.4f);
        /*Vec2 offset = Vec2(this->getContentSize().width * 0.5f, 0);
        offset.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(this->getRotation()));*/
        Vec2 direction = targetMonster->Position() - this->getPosition();
        float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());
        bullet->setRotation(-angle);
        bullet->setPosition(this->getPosition()/*+offset*/);
        schedule([this](float dt)
            {
                this->updateBullet();
            }, 0.01f, "updateBullet");
        float distance = this->getPosition().distance(targetMonster->Position());
        MoveTo* moveto = MoveTo::create(float(distance / bullet_speed), targetMonster->Position());
        if (this->getParent()) {
            this->getParent()->addChild(bullet, 10);  // 添加到父节点
        }
        bullet->runAction(moveto);
    }
}
void Bottle::rotateToMonster()
{
    if (!target_monsters.empty())
    {
        Monster* targetMonster = target_monsters.back();
        Vec2 direction = targetMonster->Position() - this->getPosition();
        float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());
        this->setRotation(-angle);
    }
}
void Bottle::updateBullet()
{
    if (bullet && !target_monsters.empty())
    {
        Monster* targetMonster = target_monsters.back();
        Rect bulletRect = Rect(
            0,
            0,
            bullet->getContentSize().width * bullet->getScale(),
            bullet->getContentSize().height * bullet->getScale()
        );
        if (bulletRect.intersectsRect(targetMonster->getBoundingBox()))
        {
            targetMonster->GetHurt(attack_powers[tag][level]);
            unschedule("updateBullet");
            scheduleOnce([this](float dt)
                {
                    bullet->removeFromParent();
                }, 0.1f, "removebullet");
        }
    }
}
void Bottle::attackAnimation()
{
    Vector<SpriteFrame*> images;
    images.pushBack(SpriteFrame::create("tower/tSnow-1.png", Rect(0, 0, 250, 250)));
    images.pushBack(SpriteFrame::create("tower/tSnow-2.png", Rect(0, 0, 280, 280)));
    images.pushBack(SpriteFrame::create("tower/tSnow-3.png", Rect(0, 0, 310, 310)));
    images.pushBack(SpriteFrame::create("tower/tSnow-4.png", Rect(0, 0, 330, 330)));
    images.pushBack(SpriteFrame::create("tower/tSnow-5.png", Rect(0, 0, 340, 340)));
    images.pushBack(SpriteFrame::create("tower/tSnow-6.png", Rect(0, 0, 390, 390)));
    images.pushBack(SpriteFrame::create("tower/tSnow-7.png", Rect(0, 0, 400, 400)));
}
/*=============便便攻击==============*/
void Shit::getTargetMonster(Vector<Monster*> monsters)
{
    target_monsters.clear();
    for (Monster* monster : monsters)
    {
        float distance = this->getPosition().distance(monster->Position());  //计算炮塔和怪物的距离
        if (distance < attack_range && monster->isStart() == true && monster->isEnd() == false)
        {
            target_monsters.pushBack(monster);
            break;
        }
    }
}
void Shit::shoot() {
    if (!target_monsters.empty()) {
        Monster* targetMonster = target_monsters.back();
        bullet = Sprite::create(bullets[tag]);
        bullet->setScale(2.f);
        Vec2 direction = targetMonster->Position() - this->getPosition();
        float angle = CC_RADIANS_TO_DEGREES(direction.getAngle());
        bullet->setRotation(-angle);
        bullet->setPosition(this->getPosition());
        schedule([this](float dt)
            {
                this->updateBullet();
            }, 0.1f, "updateBullet");
        float distance = this->getPosition().distance(targetMonster->Position());
        MoveTo* moveto = MoveTo::create(float(distance / bullet_speed), targetMonster->Position());
        if (this->getParent()) {
            this->getParent()->addChild(bullet, 10);  // 添加到父节点
        }
        bullet->runAction(moveto);
    }
}
void Shit::updateBullet()
{
    if (bullet && !target_monsters.empty())
    {
        Monster* targetMonster = target_monsters.back();
        Rect bulletRect = Rect(
            0,
            0,
            bullet->getContentSize().width * bullet->getScale(),
            bullet->getContentSize().height * bullet->getScale()
        );
        if (bulletRect.intersectsRect(targetMonster->getBoundingBox()))
        {
            targetMonster->GetHurt(attack_powers[tag][level]);
            targetMonster->SlowSpeed();
            unschedule("updateBullet");
            scheduleOnce([this](float dt)
                {
                    bullet->removeFromParent();
                }, 0.1f, "removebullet");
        }
    }
}
void Shit::attackAnimation()
{
    Vector<SpriteFrame*> images;
    images.pushBack(SpriteFrame::create("tower/tShit-1.png", Rect(0, 0, 250, 250)));
    images.pushBack(SpriteFrame::create("tower/tShit-2.png", Rect(0, 0, 280, 280)));
}
/*=============雪花攻击==============*/
void Snow::getTargetMonster(Vector<Monster*> monsters)
{
    target_monsters.clear();
    for (Monster* monster : monsters)
    {
        float distance = this->getPosition().distance(monster->Position());  //计算炮塔和怪物的距离
        if (distance < attack_range && monster->isStart() == true && monster->isEnd() == false)
        {
            target_monsters.pushBack(monster);
        }
    }
}
void Snow::shoot() {
    if (!target_monsters.empty()) {
        bullet = Sprite::create(bullets[tag]);
        bullet->setScale(1.f);
        bullet->setPosition(this->getPosition());
        attackAnimation();
        schedule([this](float dt)
            {
                this->updateBullet();
            }, 0.1f, "updateBullet");
        if (this->getParent()) {
            this->getParent()->addChild(bullet, 10);  // 添加到父节点
        }
    }
}
void Snow::updateBullet()
{
    if (bullet && !target_monsters.empty())
    {
        Rect bulletRect = Rect(
            0,
            0,
            bullet->getContentSize().width * bullet->getScale(),
            bullet->getContentSize().height * bullet->getScale()
        );
        for (auto p : target_monsters)
        {
            if (bulletRect.intersectsRect(p->getBoundingBox()))
            {
                p->GetHurt(attack_powers[tag][level]);
                p->Stop();
                unschedule("updateBullet");
                scheduleOnce([this](float dt)
                    {
                        bullet->removeFromParent();
                    }, 0.35f, "removebullet");
            }
        }

    }
}
void Snow::attackAnimation()
{
    Vector<SpriteFrame*> images;
    images.pushBack(SpriteFrame::create("tower/tSnow-1.png", Rect(0, 0, 250, 250)));
    images.pushBack(SpriteFrame::create("tower/tSnow-2.png", Rect(0, 0, 280, 280)));
    images.pushBack(SpriteFrame::create("tower/tSnow-3.png", Rect(0, 0, 310, 310)));
    images.pushBack(SpriteFrame::create("tower/tSnow-4.png", Rect(0, 0, 330, 330)));
    images.pushBack(SpriteFrame::create("tower/tSnow-5.png", Rect(0, 0, 340, 340)));
    images.pushBack(SpriteFrame::create("tower/tSnow-6.png", Rect(0, 0, 390, 390)));
    images.pushBack(SpriteFrame::create("tower/tSnow-7.png", Rect(0, 0, 400, 400)));
    Animation* animation = Animation::createWithSpriteFrames(images, 0.05f);
    Animate* animate = Animate::create(animation);
    bullet->runAction(animate);
}