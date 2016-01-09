//
//  StartScene.cpp
//  LLK
//
//  Created by lerry on 6/12/15.
//
//

#include "StartScene.h"
#include "Resources.h"
#include "SimpleAudioEngine.h"
#include "GameConfig.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* StartGame::createScene()
{
    // 创建一个场景实例
    auto scene = Scene::create();
    
    // 创建StartGame的实例，是一个Layer
    auto Layer = StartGame::create();
    
    // 把StartGame的对象添加到场景里面
    scene->addChild(Layer);
    
    return scene;
}

bool StartGame::init()
{
    // 先调父类的init函数
    // 这里可以知道，init()函数是一个继承下来的函数，而且是一个虚函数，子类可以重写这个函数
    if ( !Layer::init() ) {
        return false;
    }
    
    // 获取屏幕可视区域
    //    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // 创建一个loading的图标
    auto loading = Sprite::create(s_loading_logo);
    
    // 设置位置为中心位置
    loading->setPosition(wSize.width / 2, wSize.height / 2);
    
    // 添加到当前层
    addChild(loading);
    
    // 设置初始透明图
    loading->setOpacity(50);
    // 一个由暗到明的动画
    auto fadeIn = FadeIn::create(3);
    // 创建一个一直重复的动画
    loading->runAction(RepeatForever::create(Sequence::create(fadeIn, fadeIn->reverse() ,nullptr)));
    
    
    // loading resources
    {
        utils::gettime();
        // 帧缓存
        auto spriteFrameCache = SpriteFrameCache::getInstance();
        // 纹理缓存
        auto textureCache = Director::getInstance()->getTextureCache();
        textureCache->addImage(s_backgound);
        textureCache->addImage(s_backgound_cloud1);
        textureCache->addImage(s_backgound_cloud2);
        textureCache->addImage(s_backgound_sea1);
        textureCache->addImage(s_backgound_sea2);
        textureCache->addImage(s_beach_adornment);
        textureCache->addImage(s_island);
        textureCache->addImage(s_button_play);
        textureCache->addImage(s_button_play_s);
        textureCache->addImage(s_time_bars);
        textureCache->addImage(s_time_slot);
        textureCache->addImage(s_game_dialog);
        textureCache->addImage(s_game_leisure);
        
        spriteFrameCache->addSpriteFramesWithFile(s_boat_plist);
        spriteFrameCache->addSpriteFramesWithFile(s_seagull_plist);
        spriteFrameCache->addSpriteFramesWithFile(s_icon_plist);
        
        // 加载声音资源
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_Back2new);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_Back3new);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_class);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(s_music_mainmenu);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_2);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_3);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_4);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_12);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_14);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_15);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_16);
        SimpleAudioEngine::getInstance()->preloadEffect(s_music_19);
        

        scheduleOnce(SEL_SCHEDULE(&StartGame::initUi), 2);
    }
    
    return true;
}

void StartGame::initUi(float t)
{
    // 干掉所有的子节点，这里主要是要干掉loading的精灵和其动作
    removeAllChildren();
    
    // 纹理缓存，预加载的资源可以直接从缓存取出来
    auto textureCache = Director::getInstance()->getTextureCache();
    
    // 帧缓存，直接从缓存取帧
    auto spriteFrameCache = SpriteFrameCache::getInstance();
    
    {// 加载背景
        auto background = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound));
        
        background->setPosition(wSize.width / 2, wSize.height / 2);
        
        addChild(background);
    }
    
    {   // 白云1
        auto cloud1 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));
        // 设置锚点，左下角
        cloud1->setAnchorPoint(Vec2(0, 0));
        
        cloud1->setPosition(0, wSize.height - cloud1->getContentSize().height);
        
        addChild(cloud1);
        
        // 白云倒影
        auto cloudShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud1));

        cloudShadow->setAnchorPoint(Vec2(0, 0));
        
        // 垂直翻转
        cloudShadow->setFlippedY(true);
        
        // 设置透明度
        cloudShadow->setOpacity(40);
        
        cloudShadow->setPosition(40, wSize.height - cloud1->getContentSize().height * 2);
        
        addChild(cloudShadow);
        
        // 白云2
        auto cloud2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_cloud2));
        // 设置锚点，左下角
        cloud2->setAnchorPoint(Vec2(0, 0));
        
        cloud2->setPosition(cloud1->getContentSize().width, wSize.height - cloud2->getContentSize().height);
        
        addChild(cloud2);

        // 岛
        auto land = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
        
        // 设置锚点，右下角
        land->setAnchorPoint(Vec2(1, 0));
        
        land->setPosition(wSize.width - 40, wSize.height - land->getContentSize().height - 47 * 2);
        
        addChild(land);
        
        // 岛倒影
        auto landShadow = Sprite::createWithTexture(textureCache->getTextureForKey(s_island));
        
        landShadow->setAnchorPoint(Vec2(1, 0));
        
        landShadow->setFlippedY(true);
        
        landShadow->setOpacity(40);
        
        landShadow->setPosition(wSize.width - 40, wSize.height - land->getContentSize().height - 78 * 2);
        
        addChild(landShadow);
        
        // 取第一帧
        auto frame = spriteFrameCache->getSpriteFrameByName("sailing_boat1.png");
        
        // 用帧创建一个船精灵
        auto boat = Sprite::createWithSpriteFrame(frame);
        boat->setPosition(wSize.width - boat->getContentSize().width-50*2,230*2);
        
        // 创建一个帧动画
        auto animation = Animation::create();
        
        // 循环取船的序列帧
        for (int i = 1; i < 4; i++) {
            
            char bname[64] = {0};
            
            sprintf(bname, "sailing_boat%d.png", i);
            
            animation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bname));
        }
        
        // 设置每一个动画帧的间隔
        animation->setDelayPerUnit(0.5);
        
        // 动画完毕后保持第一帧
        animation->setRestoreOriginalFrame(true);
        
        addChild(boat);
        
        // 创建一个动作
        auto animate = Animate::create(animation);
        
        boat->runAction(RepeatForever::create(animate));
        
        // 向左移动
        auto moveBy = MoveBy::create(10, Vec2(-240, 0));
        
        // 水平翻转，船掉头
        auto flipxAction = FlipX::create(true);
        
        // 组合动画，船来回游荡，并会掉头
        auto action = Sequence::create(moveBy, flipxAction, moveBy->reverse(), flipxAction->reverse(), nullptr);
        
        boat->runAction(RepeatForever::create(action));
        
        // 第二条船，动画同上，不过是在左边
        auto boat2 = Sprite::createWithSpriteFrame(frame);
        
        boat2->setFlippedX(true);
        
        boat2->setPosition(100, 400);
        
        addChild(boat2);
        
        // 这里不能直接用animate，因为boat1已经使用，这里必须要用另一个动作，所以clone一下
        boat2->runAction(animate->clone());
        
        auto moveBy2 = MoveBy::create(12, Vec2(300, 0));
        
        // 这里的flipx跟boat1相反，所以这里先reverse一下，后面不reverse
        auto action2 = Sequence::create(moveBy2, flipxAction->clone()->reverse(), moveBy2->reverse(), flipxAction->clone(), nullptr);
        
        boat2->runAction(RepeatForever::create(action2));
        
        
        // 沙滩
        auto beach = Sprite::createWithTexture(textureCache->getTextureForKey(s_beach_adornment));
        
        beach->setPosition(wSize.width / 2, beach->getContentSize().height / 2);
        
        addChild(beach);
        
        
        // 海浪1
        auto wave = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea1));
        
        wave->setOpacity(0);
        
        wave->setPosition(wSize.width / 2 , 140 * 2);
        
        addChild(wave);
        
        // 海浪2
        auto wave2 = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound_sea2));
        
        wave2->setPosition(wSize.width / 2 , 100 * 2);
        
        wave2->setOpacity(0);
        
        addChild(wave2);

        
        // 两组动画，知识点比较多
        auto waveMoveBy = MoveBy::create(4.0, Vec2(0, -80));
        
        // 由暗到明
        auto fadeIn = FadeIn::create(3.0);
        
        // 由明到暗
        auto fadeOut = FadeOut::create(1.0);
        
        // 放到初始位置
        auto place1 = Place::create(Vec2(wSize.width / 2 , 140 * 2));
        
        // 同时进行动作，时间长度由最长的动作决定
        auto spawn = Spawn::create(waveMoveBy, Sequence::create(fadeIn, fadeOut, nullptr), nullptr);
        
        // 海浪2的动作
        auto spawn2 = Spawn::create(waveMoveBy->clone()->reverse(), Sequence::create(fadeIn->clone(), fadeOut->clone(), nullptr), nullptr);
        // 海浪2的初始位置
        auto place2 = Place::create(Vec2(wSize.width / 2 , 100 * 2));
        
        // target 动作，指定tartget的动作
        auto targetAction = TargetedAction::create(wave2, Sequence::create(spawn2, place2,nullptr));
        
        // 延迟2s
        auto delay = DelayTime::create(2.0);
        
        wave->runAction(RepeatForever::create(Sequence::create(spawn, targetAction, place1, delay, nullptr)));
        
        
        // 鸟
        
        auto birdFrame = spriteFrameCache->getSpriteFrameByName("seagull1.png");
        
        mBird = Sprite::createWithSpriteFrame(birdFrame);
        
        auto point = Vec2(CCRANDOM_0_1() * wSize.width , 0);
        
        mBird->setPosition(point);
        
        // 创建帧动画
        auto birdAnimation = Animation::create();
        
        // 循环取鸟的序列帧
        for (int i = 1; i < 4; i++) {
            
            char bname[64] = {0};
            
            sprintf(bname, "seagull%d.png", i);
            
            birdAnimation->addSpriteFrame(spriteFrameCache->getSpriteFrameByName(bname));
        }

        // 设置每一个动画帧的间隔
        birdAnimation->setDelayPerUnit(0.3);
        
        // 动画完毕后保持第一帧
        birdAnimation->setRestoreOriginalFrame(true);
        
        addChild(mBird);
        
        // 创建一个动作
        auto birdAnimate = Animate::create(birdAnimation);
        
        mBird->runAction(RepeatForever::create(birdAnimate));
        
        mBird->runAction(MoveBy::create(2, Vec2(CCRANDOM_0_1() * wSize.width , point.y + wSize.height + 100)));
        
        // 启用默认调度器，调用系统update函数来刷新
        scheduleUpdate();
        
        //---------------------------------------------------------
        // 播放按钮
        ///正常显示
        auto play = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play));
        ///被选中
        auto playS = Sprite::createWithTexture(textureCache->getTextureForKey(s_button_play_s));
        
        // 菜单按钮接一个回调函数
        auto menuItem = MenuItemSprite::create(play, play, CC_CALLBACK_1(StartGame::play, this));
        
        auto menu = Menu::create(menuItem, nullptr);
        
        menu->setPosition(wSize.width / 2, wSize.height / 2);
        
        addChild(menu);
    }
    
}

void StartGame::update(float dt)
{
    // 如果海鸥飞出屏幕，就重新设置其位置，重新执行飞的动作
    if (mBird->getPosition().y > wSize.height + 20)
    {
        auto point = Vec2(CCRANDOM_0_1() * wSize.width, 10);
        mBird->setPosition(point);
        mBird->runAction(MoveBy::create(floor(5 * CCRANDOM_0_1()), Vec2(CCRANDOM_0_1() * wSize.width, point.y + wSize.height)));
    }
}
///开始游戏
void StartGame::play(Ref* sender)
{
    CCLOG("play----");
    auto scene = GameScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionMoveInT::create(0.5, scene));
    
}


