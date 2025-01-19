//’œ∞≠ŒÔ¿‡
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include"cocos2d.h"

class Object : public cocos2d::Sprite {
public:
    int hp;

    virtual void onHit(int damage) {
        hp -= damage;
        if (hp <= 0) {
            this->destroy();
        }
    }

    virtual void destroy() {
        this->removeFromParentAndCleanup(true);
    }
};

class Tree : public Object {
public:
    static Tree* createTree() {
        auto tree = new Tree();
        if (tree && tree->initWithFile("test/Tree.png")) {
            tree->autorelease();
            tree->hp = 100;
            return tree;
        }
        CC_SAFE_DELETE(tree);
        return nullptr;
    }
};

class BigTree : public Object {
public:
    static BigTree* createBigTree() {
        auto bigTree = new BigTree();
        if (bigTree && bigTree->initWithFile("test/BigTree.png")) {
            bigTree->autorelease();
            bigTree->hp = 200;
            return bigTree;
        }
        CC_SAFE_DELETE(bigTree);
        return nullptr;
    }
};

class Ice : public Object {
public:
    static Ice* createIce() {
        auto ice = new Ice();
        if (ice && ice->initWithFile("test/Ice.png")) {
            ice->autorelease();
            ice->hp = 100;
            return ice;
        }
        CC_SAFE_DELETE(ice);
        return nullptr;
    }
};

class BigIce : public Object {
public:
    static BigIce* createBigIce() {
        auto bigIce = new BigIce();
        if (bigIce && bigIce->initWithFile("test/BigIce.png")) {
            bigIce->autorelease();
            bigIce->hp = 200;
            return bigIce;
        }
        CC_SAFE_DELETE(bigIce);
        return nullptr;
    }
};

class Icecream : public Object {
public:
    static Icecream* createIcecream() {
        auto icecream = new Icecream();
        if (icecream && icecream->initWithFile("test/Icecream.png")) {
            icecream->autorelease();
            icecream->hp = 100;
            return icecream;
        }
        CC_SAFE_DELETE(icecream);
        return nullptr;
    }
};

class House : public Object {
public:
    static House* createHouse() {
        auto house = new House();
        if (house && house->initWithFile("test/House.png")) {
            house->autorelease();
            house->hp = 300;
            return house;
        }
        CC_SAFE_DELETE(house);
        return nullptr;
    }
};

class Stone : public Object {
public:
    static House* createStone() {
        auto stone = new House();
        if (stone && stone->initWithFile("test/Stone.png")) {
            stone->autorelease();
            stone->hp = 300;
            return stone;
        }
        CC_SAFE_DELETE(stone);
        return nullptr;
    }
};


#endif // __OBJECT_H__
