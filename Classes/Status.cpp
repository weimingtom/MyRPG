//
//  Status.cpp
//  MyRPG
//
//  Created by RocLee on 14-8-11.
//
//

#include "Status.h"
#include "Battle.h"
#include "SelectHero.h"

CCScene* Status::scene(){
    CCScene* pScene=CCScene::create();
    CCLayer* pLayer=Status::create();
    pScene->addChild(pLayer);
    return pScene;
}

bool Status::init(){
    bool sRect=false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        ud=UserData::LoadUserData();
        sRect=true;
    } while (0);
    return sRect;
}

void Status::onEnter(){
    CCLayer::onEnter();
    setupViews();
}

void Status::draw(){
    CCSize winsize=CCDirector::sharedDirector()->getWinSize();
    ccDrawLine(ccp(0, winsize.height*3/4), ccp(winsize.width, winsize.height*3/4));
    
    CHECK_GL_ERROR_DEBUG();
}

CCSize Status::getWinsize(){
    return CCDirector::sharedDirector()->getWinSize();
}

void  Status::setupViews(){
    sp=ud->getSparePoint();
    HP=(int)ud->getHP();
    ATK=(int)ud->getATK();
    DEF=(int)ud->getDEF();
    AGI=(int)ud->getAGI();
    LUK=(int)ud->getLUK();
    
    CCSprite* player=CCSprite::create(ud->getPic());
    player->setPosition(ccp(player->getContentSize().width/2+50, getWinsize().height-player->getContentSize().height/2-100));
    player->setScale(2.0f);
    this->addChild(player);
    
    CCString* stringlevel=CCString::createWithFormat("LEVEL :%d",ud->getLevel());
    CCLabelBMFont* level=CCLabelBMFont::create(stringlevel->getCString(), "myfont1.fnt");
    level->setPosition(ccp(getWinsize().width/2+30, getWinsize().height-100));
    this->addChild(level);
    CCLabelBMFont* pointlabel=CCLabelBMFont::create("SparePoint :", "myfont1.fnt");
    pointlabel->setPosition(ccp(getWinsize().width/2+30, getWinsize().height-200));
    this->addChild(pointlabel);
    
    sparepoint=CCLabelAtlas::create(Battle::IntToChar(sp), "number.png", 24, 34, 48);
    sparepoint->setPosition(ccp(getWinsize().width/2+pointlabel->getContentSize().width/2+50, getWinsize().height-220));
    this->addChild(sparepoint);
    
    //属性标签
    CCLabelBMFont* HPlabel=CCLabelBMFont::create("HP  :", "myfont1.fnt");
    CCLabelBMFont* ATKlabel=CCLabelBMFont::create("ATK :", "myfont1.fnt");
    CCLabelBMFont* DEFlabel=CCLabelBMFont::create("DEF :", "myfont1.fnt");
    CCLabelBMFont* AGIlabel=CCLabelBMFont::create("AGI :", "myfont1.fnt");
    CCLabelBMFont* LUKlabel=CCLabelBMFont::create("LUK :", "myfont1.fnt");
    
    LUKlabel->setPosition(ccp(100, getWinsize().height/5));
    AGIlabel->setPosition(SelectHero::nextLabelPosition(LUKlabel, true, 50));
    DEFlabel->setPosition(SelectHero::nextLabelPosition(AGIlabel, true, 50));
    ATKlabel->setPosition(SelectHero::nextLabelPosition(DEFlabel, true, 50));
    HPlabel->setPosition(SelectHero::nextLabelPosition(ATKlabel, true, 50));
    
    this->addChild(HPlabel);
    this->addChild(ATKlabel);
    this->addChild(DEFlabel);
    this->addChild(AGIlabel);
    this->addChild(LUKlabel);
    
    //数值标签
    statunum1=CCLabelAtlas::create(Battle::IntToChar(HP), "number.png", 24, 34, 48);
    statunum2=CCLabelAtlas::create(Battle::IntToChar(ATK), "number.png", 24, 34, 48);
    statunum3=CCLabelAtlas::create(Battle::IntToChar(DEF), "number.png", 24, 34, 48);
    statunum4=CCLabelAtlas::create(Battle::IntToChar(AGI), "number.png", 24, 34, 48);
    statunum5=CCLabelAtlas::create(Battle::IntToChar(LUK), "number.png", 24, 34, 48);
    
    statunum1->setPosition(HPlabel->getPositionX()+250,HPlabel->getPositionY()-20);
    statunum2->setPosition(HPlabel->getPositionX()+250,ATKlabel->getPositionY()-20);
    statunum3->setPosition(HPlabel->getPositionX()+250,DEFlabel->getPositionY()-20);
    statunum4->setPosition(HPlabel->getPositionX()+250,AGIlabel->getPositionY()-20);
    statunum5->setPosition(HPlabel->getPositionX()+250,LUKlabel->getPositionY()-20);
    
    this->addChild(statunum1,1,31);
    this->addChild(statunum2,1,32);
    this->addChild(statunum3,1,33);
    this->addChild(statunum4,1,34);
    this->addChild(statunum5,1,35);
    
    //存放按钮的ccarray
    buttomarray=CCArray::create();
    buttomarray->retain();
    
    flag=10;
    
    //+-按钮tag0-4是增加按钮,tag5-9是减少按钮
    for (int i=0; i!=5; i++) {
        CCSprite* addsprite=CCSprite::create("add.png");
        addsprite->setPosition(ccp(HPlabel->getPositionX()+150, HPlabel->getPositionY()-i*115));
        this->addChild(addsprite, 1, i);
        buttomarray->addObject(addsprite);
    }
    for (int i=5; i!=10; i++) {
        CCSprite* subsprite=CCSprite::create("sub.png");
        subsprite->setPosition(ccp(statunum1->getPositionX()+150, HPlabel->getPositionY()-(i-5)*115));
        this->addChild(subsprite, 1, i);
        buttomarray->addObject(subsprite);
    }

    CCLOG("buttomarray count:%d",buttomarray->count());
    //注册触摸
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 10, true);
    
    this->scheduleUpdate();
    

}

bool Status::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    CCObject* buttom;
    CCARRAY_FOREACH(buttomarray, buttom){
        CCSprite* sp=(CCSprite*)buttom;
        if (sp->boundingBox().containsPoint(pTouch->getLocation())) {
            flag=sp->getTag();
            CCLOG("Touch Tag:%d",flag);
        }
    }
    return true;
}


void Status::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    flag=10;
}

void Status::update(float t){
    if (atoi(sparepoint->getString())<=sp&&atoi(sparepoint->getString())>0) {
        switch (flag) {
            case 0:
                labelchange(statunum1, true);
                labelchange(sparepoint, false);
                break;
            case 1:
                labelchange(statunum2, true);
                labelchange(sparepoint, false);
                break;
            case 2:
                labelchange(statunum3, true);
                labelchange(sparepoint, false);
                break;
            case 3:
                labelchange(statunum4, true);
                labelchange(sparepoint, false);
                break;
            case 4:
                labelchange(statunum5, true);
                labelchange(sparepoint, false);
                break;
            default:
                break;
        }
    }
    if (atoi(sparepoint->getString())<sp&&atoi(sparepoint->getString())>=0){
        switch (flag) {
            case 5:
                if (atoi(statunum1->getString())>HP) {
                    labelchange(statunum1, false);
                    labelchange(sparepoint, true);
                }
                break;
            case 6:
                if (atoi(statunum2->getString())>ATK) {
                    labelchange(statunum2, false);
                    labelchange(sparepoint, true);
                }
                break;
            case 7:
                if (atoi(statunum3->getString())>DEF) {
                    labelchange(statunum3, false);
                    labelchange(sparepoint, true);
                }
                break;
            case 8:
                if (atoi(statunum4->getString())>AGI) {
                    labelchange(statunum4, false);
                    labelchange(sparepoint, true);
                }
                break;
            case 9:
                if (atoi(statunum5->getString())>LUK) {
                    labelchange(statunum5, false);
                    labelchange(sparepoint, true);
                }
                break;
            default:
                break;
        }
    }
    
    
}

void Status::labelchange(cocos2d::CCLabelAtlas *label, bool isadd){
    const char* str=label->getString();
    if (isadd) {
        label->setString(Battle::IntToChar(atoi(str)+1));
    }
    else label->setString(Battle::IntToChar(atoi(str)-1));
}