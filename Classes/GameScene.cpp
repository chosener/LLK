//
//  GameScene.cpp
//  LLK
//
//  Created by lerry on 6/16/15.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Resources.h"
#include <math.h>

USING_NS_CC;
using namespace CocosDenshion;

std::vector<Vec2> p1E;
std::vector<Vec2> p2E;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
        return false;
    }
    
    playBgMusic();
    
    initUI();
    
    initMap();
    
    return true;
}

void GameScene::initUI()
{
    // 纹理缓存，预加载的资源可以直接从缓存取出来
    auto textureCache = Director::getInstance()->getTextureCache();
    
//    auto spriteFrameCache = SpriteFrameCache::getInstance();
    
    // 加载背景
    auto background = Sprite::createWithTexture(textureCache->getTextureForKey(s_backgound));
    
    background->setPosition(wSize.width / 2, wSize.height / 2);
    
    addChild(background);
    
    
    // 白云1
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
    
    // 时间条背景框
    auto progressFrame = Sprite::createWithTexture(textureCache->getTextureForKey(s_time_slot));
    // 锚点，左下角
    progressFrame->setAnchorPoint(Vec2(0, 0));
    
    progressFrame->setPosition(120, wSize.height-50);
    
    addChild(progressFrame);
    
    // 时间条精灵
    auto pSprite = Sprite::createWithTexture(textureCache->getTextureForKey(s_time_bars));
    
    progress = ProgressTimer::create(pSprite);
    // 锚点，左下角
    progress->setAnchorPoint(Vec2(0, 0));
    // 类型，条形
    progress->setType(ProgressTimer::Type::BAR);
    
    progress->setPosition(120, wSize.height - 50);
    // 水平变化
    progress->setMidpoint(Vec2(0, 0));
    // 一次一个单位
    progress->setBarChangeRate(Vec2(1, 0));
    // 初始100
    progress->setPercentage(100);
    
    addChild(progress);
    
    // 时间数字
    numberTime = Label::createWithSystemFont("100", "Thonburi", 24);
    
    numberTime->setAnchorPoint(Vec2(0, 0));
    
    numberTime->setPosition(400, wSize.height - 50);
    
    numberTime->setColor(Color3B::BLACK);
    
    addChild(numberTime);
    
    auto tip = Sprite::createWithTexture(textureCache->getTextureForKey(s_game_leisure));
    
    auto menuItemSprite = MenuItemSprite::create(tip, tip, CC_CALLBACK_1(GameScene::autoClear, this));
    
    auto menu = Menu::create(menuItemSprite, nullptr);
    
    menu->setAnchorPoint(Vec2(1, 1));
    
    menu->setPosition(wSize.width - 100, wSize.height - 100);
    
    addChild(menu);
    
    //触摸监听
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //画线
    mDraw = DrawNode::create();
    
    addChild(mDraw, 1000);
    
    mPre = Vec2::ZERO;
    
    //计时器更新
    schedule(SEL_SCHEDULE(&GameScene::update), 1.0);
    
}

void GameScene::update(float dt)
{
    
    if (win()) {
        gameOver("You Win!");
        return;
    }
    
    // 当前进度
    int current = progress->getPercentage();
    // 为零就gameover
    if (current == 0) {
        // game over
        gameOver("Game Over");
        return;
    }
    else
    {
#if 0
        // 减一
        current--;
        progress->setPercentage(current);
        char time[] = {25};
        sprintf(time, "%d",current);
        numberTime->setString(time);
#endif
    }
}

bool GameScene::win()
{
    for (int x = 0; x < xCount; x++) {
        for (int y = 0; y < yCount; y++) {
            if (mMap[x][y] != 0) {
                return false;
            }
        }
    }
    return true;
}

void GameScene::autoClear(Ref *spender)
{

    
#if 0
    Sprite* spIcon = (Sprite*)spender;
    // 左右抖动的动画
    auto rote = RotateBy::create(0.05, 20);
    auto seq = Sequence::create(rote, rote->reverse(), rote->reverse(), rote->clone(), nullptr);

    spIcon->runAction(seq);
#endif
    
    if (die())
    {
        CCLOG("die-----");
        changeMap();
    }
    else
    {
        //没有死的时候
        drawLine();
    }
}

void GameScene::gameOver(std::string title)
{
    // 停止update函数调度。
    unschedule(SEL_SCHEDULE(&GameScene::update));
    
    
    // 创建一个颜色层
    auto layer = LayerColor::create(Color4B(100, 100, 100, 100));
    // 锚点默认是左下角
    layer->setPosition(0,0);
    addChild(layer, 1000);
    
    // 对话框背景
    auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(s_game_dialog));
    sprite->setPosition(wSize.width / 2, wSize.height / 2);
    layer->addChild(sprite);
    
    // Game Over 提示
    auto titleSprite = Label::createWithSystemFont(title, "Thonburi", 35);
    titleSprite->setPosition(layer->getContentSize().width / 2, sprite->getContentSize().height - 100);
    titleSprite->setColor(Color3B::RED);
    layer->addChild(titleSprite);
    
    // Font Item
    MenuItemFont::setFontName("fonts/Marker Felt.ttf");
    // Play Again 按钮
    auto item1= MenuItemFont::create("Play Again", CC_CALLBACK_0(GameScene::playAgain, this) );
    auto color_action = TintBy::create(0.5f, 0, -255, -255);
    auto color_back = color_action->reverse();
    auto seq = Sequence::create(color_action, color_back, nullptr);
    item1->runAction(RepeatForever::create(seq));
    
    auto menu = Menu::create(item1, nullptr);
    
    menu->setPosition(layer->getContentSize().width / 2, sprite->getContentSize().height / 2);
    
    layer->addChild(menu);
    
    // Game Over层添加触摸事件
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    // 吞掉触摸事件，即下面的层不会响应此次触摸
    listener->setSwallowTouches(true);
    // Lambda表达式实现触摸函数，空的，因为不需要做触摸实现
    listener->onTouchBegan = [](Touch*touch, Event* event){return true;};
    listener->onTouchMoved = [](Touch*touch, Event* event){};
    listener->onTouchEnded = [](Touch*touch, Event* event){};
    listener->onTouchCancelled = [](Touch*touch, Event* event){};
    // 加到GameOver层
    dispatcher->addEventListenerWithSceneGraphPriority(listener, layer);
    
}

void GameScene::playAgain()
{
    auto scene = GameScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionMoveInT::create(0.5, scene));

    
}

void GameScene::playBgMusic()
{
    SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_Back2new, false);
    
    scheduleOnce(SEL_SCHEDULE(&GameScene::playBgMusic1), 60);
}

void GameScene::playBgMusic1()
{
    SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_Back3new, false);
    scheduleOnce(SEL_SCHEDULE(&GameScene::playBgMusic2), 62);
}

void GameScene::playBgMusic2()
{
    SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_mainmenu, false);
    scheduleOnce(SEL_SCHEDULE(&GameScene::playBgMusic), 65);
}

void GameScene::initMap()
{
    // 这里x用来控制地图数组的每格的值，y用来控制两次反转，即每两个格标记同一个值
    int x = 1;
    int y = 0;
    // 数组下标从0开始，这里从1开始遍历，那么最外面一圈就是0不变
    for (int i = 1; i < xCount - 1; i++)
    {
        for (int j = 1; j < yCount - 1; j++)
        {
            // 地图数组赋值
            mMap[i][j] = x;
            // y控制反转，x控制每格值增加，增大到图片总数后从再1开始递增
            if (y == 1)
            {
                x++;
                y = 0;
                if ( x == iconCount)
                {
                    x = 1;
                }
            }
            else
            {
                y = 1;
            }
        }
    }
    
    change();
    
    drawMap();
    
}

///绘制整个地图
void GameScene::drawMap()
{
    // 绘制棋盘上精灵
    for (int x = 0; x < xCount; x++)
    {
        for (int y = 0; y < yCount; y++)
        {
            if (mMap[x][y] > 0)
            {
                char iconName[64] = {0};
                // 格式化图片名
                sprintf(iconName, "%d.png", mMap[x][y]);
                auto position = indextoScreen(x, y);
                // 所有图片已经加到帧缓存，这里直接用名字取一帧，创建一个精灵
                auto icon = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(iconName));
                //icon->setAnchorPoint(Vec2(0, 0));
                icon->setPosition(position);
                // 设置一个tag，方便后续识别点击的是那个图标
                int tag = (yCount - 2) * ( x - 1 ) + y;
                // 这是Z序和tag值，Z序设置大一些确保图标在其他精灵的前面
                addChild(icon, 100, tag);
            }
        }
    }

}


// 随机改变一下棋局位置
void GameScene::change()
{
    // 随机种子
    srand((unsigned int)time(NULL));
    
    // 临时变量，用来交换两个位置的数据
    int tempX, tempY, tempM;
    
    // 遍历地图数组，随机交换位置
    for (int x = 1; x < xCount - 1 ; x++)
    {
        for (int y = 1; y < yCount - 1; y++)
        {
            tempX = 1 + (int)(CCRANDOM_0_1() * (xCount - 2));
            tempY = 1 + (int)(CCRANDOM_0_1() * (yCount - 2));
            tempM = mMap[x][y];
            mMap[x][y] = mMap[tempX][tempY];
            mMap[tempX][tempY] = tempM;
        }
        
    }
    
    //如果是一盘死棋,当前没有可以连接
    if (die())
    {
        //重新布局
        changeMap();
    }
    
}

void GameScene::changeMap()
{
    // 随机种子
    srand((unsigned int)time(NULL));
    
    // 临时变量，用来交换两个位置的数据
    int tempX, tempY, tempM;
    
    // 遍历地图数组，随机交换位置
    for (int x = 1; x < xCount - 1 ; x++)
    {
        for (int y = 1; y < yCount - 1; y++)
        {
            tempX = 1 + (int)(CCRANDOM_0_1() * (xCount - 2));
            tempY = 1 + (int)(CCRANDOM_0_1() * (yCount - 2));
            tempM = mMap[x][y];
            mMap[x][y] = mMap[tempX][tempY];
            mMap[tempX][tempY] = tempM;
            
            
            // 交换精灵位置，交换tag值
            int tag1 = (yCount - 2) * ( x - 1 ) + y;
            int tag2 = (yCount - 2) * ( tempX - 1 ) + tempY;
            
            auto pos1 = indextoScreen(x, y);
            auto pos2 = indextoScreen(tempX, tempY);
            
            auto pic1 = getChildByTag(tag1);
            auto pic2 = getChildByTag(tag2);
            
            if (pic1)
            {
                pic1->setPosition(pos2);
                pic1->setTag(tag2);
            }else
            {
                // 如果为空，地图数组对应值要置0
                mMap[tempX][tempY] = 0;
            }
            
            if (pic2)
            {
                pic2->setPosition(pos1);
                pic2->setTag(tag1);
            }else
            {
                mMap[x][y] = 0;
            }
            
        }
    }

}

bool GameScene::die()
{
    for (int y = 1; y < yCount; y++) {
        for (int x = 1; x < xCount; x++) {
            if (mMap[x][y] != 0) {
                for (int j = y; j < yCount - 1; j++)
                {
                    if (j == y)
                    {
                        for (int i = x + 1; i < xCount - 1; i++)
                        {
                            if (mMap[i][j] == mMap[x][y] && link(Vec2(x, y), Vec2(i, j)))
                            {
                                return false;
                            }
                        }
                    }else
                    {
                        for (int i = 1; i < xCount - 1; i++)
                        {
                            if (mMap[i][j] == mMap[x][y] && link(Vec2(x, y), Vec2(i, j)))
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}
bool GameScene::dieTemp()
{
    for (int y = 1; y < yCount; y++) {
        for (int x = 1; x < xCount; x++) {
            if (mMap[x][y] != 0) {
                for (int j = y; j < yCount - 1; j++)
                {
                    if (j == y)
                    {
                        for (int i = x + 1; i < xCount - 1; i++)
                        {
                            if (mMap[i][j] == mMap[x][y] && linkTemp(Vec2(x, y), Vec2(i, j)))
                            {
                                return false;
                            }
                        }
                    }else
                    {
                        for (int i = 1; i < xCount - 1; i++)
                        {
                            if (mMap[i][j] == mMap[x][y] && linkTemp(Vec2(x, y), Vec2(i, j)))
                            {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto point = touch->getLocation();
    CCLOG("Location point x=%f, y=%f", point.x, point.y);
    return true;
}

void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void GameScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    float x = touch->getLocation().x;
    
    float y = touch->getLocation().y;
    
    // 屏幕坐标转换为地图坐标
    auto point = screentoIndex(x, y);
    
    CCLOG("touch poin index x:%d, y:%d" , (int)point.x, (int)point.y);
    
    // 判断连通与否
    if (mMap[(int)point.x][(int)point.y] > 0)
    {
        if (mSelected.size() == 1)
        {
            CCLOG("compare point x:%d, y:%d" , (int)point.x, (int)point.y);
            
            if (link(mSelected.front(), point))
            {
                CCLOG("path point count :%d", (int)mPath.size());
                mSelected.push_back(point);
                drawLine();
                
#if 1
                if (dieTemp())
                {
                    CCLOG("die-----");
                    changeMap();
                }
#endif
            }
            else
            {
                mPre = (Vec2)mSelected.front();
                mSelected.clear();
                mSelected.push_back(point);
            }
        }
        else
        {
            CCLOG("add a select point");
            mSelected.push_back(point);
        }
    }

    
    // 前一次点击的点
    if (!mPre.equals(Vec2::ZERO)) {
        int x = (int)mPre.x;
        int y = (int)mPre.y;
        int tag =  (yCount - 2) * ( x - 1 ) + y;
        auto slectedIcon = getChildByTag(tag);
        
        if(slectedIcon)
        {
            // 恢复原大小
            slectedIcon->setScale(1.0);
            // 恢复原Z序
            slectedIcon->setLocalZOrder(100);
        }

    }
    
    
    
    // 绘制选择图标，选中时变大
    for ( Vec2 position: mSelected ) {
        int x = (int)position.x;
        
        int y = (int)position.y;
        
        int tag = (yCount - 2) * ( x - 1 ) + y;
        
        auto slectedIcon = getChildByTag(tag);
        
        if(slectedIcon)
        {
            // 放大1.2倍
            slectedIcon->setScale(1.2);
            
            // Z序提前，放在所有精灵前面
            slectedIcon->setLocalZOrder(101);
        }

    }

    
}



// 数组坐标转换为屏幕坐标
Vec2 GameScene::indextoScreen(int x, int y)
{
    return Vec2(x * iconSize, y * iconSize);
}

Vec2 GameScene::screentoIndex(float x, float y)
{
    int ix = ceil((x - iconSize / 2) / iconSize);
    
    int iy = ceil((y - iconSize / 2) / iconSize);
    
    if (ix < xCount && iy < yCount) {
        return Vec2(ix ,iy);
    }else{
        return Vec2::ZERO;
    }
}

void GameScene::drawLine()
{

    // 画线
    if (mPath.size() >= 2) {
        Vec2 *vecs = new Vec2[mPath.size()];
        for (int i = 0 ; i < mPath.size(); i++) {
            vecs[i] = indextoScreen(mPath.at(i).x, mPath.at(i).y);
        }

        // 随机给线条一个颜色
        auto color = CCRANDOM_0_1();
        
        // 循环画线段，只有线段可以设置线条宽度
        for (int i = 0 ; i < mPath.size(); i++) {
            if ( i > 0) {
                mDraw->drawSegment(vecs[i-1], vecs[i], 5, Color4F(color, color, color, 1));
            }
        }
        
        // 连通的两个图标对应的地图数组置0
        Vec2 p1 = mPath.front();
        mMap[(int)p1.x][(int)p1.y] = 0;
        Vec2 p2 = mPath.back();
        mMap[(int)p2.x][(int)p2.y] = 0;
        
        // 删掉线段的顶点数组
        delete []vecs;
        
        // 清除连通的图标，同时清除路径点
        scheduleOnce(SEL_SCHEDULE(&GameScene::clearMatched), 0.2);
        
    }

}

void GameScene::clearMatched()
{
    mDraw->clear();
    
    Vec2 p1 = mPath.front();
    Vec2 p2 = mPath.back();
    
    auto x1 = (int)p1.x;
    auto y1 = (int)p1.y;
    auto tag1 = (yCount - 2) * ( x1 - 1 ) + y1;
    removeChildByTag(tag1);
    
    auto x2 = (int)p2.x;
    auto y2 = (int)p2.y;
    auto tag2 = (yCount - 2) * ( x2 - 1 ) + y2;
    removeChildByTag(tag2);
    
    mSelected.clear();
    mPath.clear();
}

bool GameScene::link(cocos2d::Vec2 v1, cocos2d::Vec2 v2)
{
    if (v1.equals(v2)) {
        return false;
    }
    // 路径点集，是一个vector
    mPath.clear();
    
    // 判断点击的两个点是不是图案一致
    if (mMap[(int)v1.x][(int)v1.y] == mMap[(int)v2.x][(int)v2.y])
    {
        // 直连
        if (linkD(v1, v2)) {
            mPath.push_back(v1);
            mPath.push_back(v2);
            return true;
        }
        
        // 一个拐角，对角可直连
        auto p = Vec2(v1.x, v2.y);
        if (mMap[(int)p.x][(int)p.y] == 0) {
            if (linkD(v1, p) && linkD(p, v2)) {
                mPath.push_back(v1);
                mPath.push_back(p);
                mPath.push_back(v2);
                return true;
            }
        }
        
        // 一个拐角，对角可直连
        p = Vec2(v2.x, v1.y);
        if (mMap[(int)p.x][(int)p.y] == 0) {
            if (linkD(v1, p) && linkD(p, v2)) {
                mPath.push_back(v1);
                mPath.push_back(p);
                mPath.push_back(v2);
                return true;
            }
        }
        
        // X扩展，判断是否有可直连的点
        expandX(v1, p1E);
        expandX(v2, p2E);
        
        for (auto pt1 : p1E) {
            for (auto pt2 : p2E) {
                if (pt1.x == pt2.x) {
                    if (linkD(pt1, pt2)) {
                        mPath.push_back(v1);
                        mPath.push_back(pt1);
                        mPath.push_back(pt2);
                        mPath.push_back(v2);
                        return true;
                    }
                }
            }
        }
        
        // Y扩展，判断是否有可直连的点
        expandY(v1, p1E);
        expandY(v2, p2E);
        for (auto pt1 : p1E) {
            for (auto pt2 : p2E) {
                if (pt1.y == pt2.y) {
                    if (linkD(pt1, pt2)) {
                        mPath.push_back(v1);
                        mPath.push_back(pt1);
                        mPath.push_back(pt2);
                        mPath.push_back(v2);
                        return true;
                    }
                }
            }
        }
        
        
        return false;
    }
    
    
    return false;
}

bool GameScene::linkTemp(cocos2d::Vec2 v1, cocos2d::Vec2 v2)
{
    if (v1.equals(v2)) {
        return false;
    }

    
    // 判断点击的两个点是不是图案一致
    if (mMap[(int)v1.x][(int)v1.y] == mMap[(int)v2.x][(int)v2.y])
    {
        // 直连
        if (linkD(v1, v2)) {
            return true;
        }
        
        // 一个拐角，对角可直连
        auto p = Vec2(v1.x, v2.y);
        if (mMap[(int)p.x][(int)p.y] == 0) {
            if (linkD(v1, p) && linkD(p, v2)) {

                return true;
            }
        }
        
        // 一个拐角，对角可直连
        p = Vec2(v2.x, v1.y);
        if (mMap[(int)p.x][(int)p.y] == 0) {
            if (linkD(v1, p) && linkD(p, v2)) {

                return true;
            }
        }
        
        // X扩展，判断是否有可直连的点
        expandX(v1, p1E);
        expandX(v2, p2E);
        
        for (auto pt1 : p1E) {
            for (auto pt2 : p2E) {
                if (pt1.x == pt2.x) {
                    if (linkD(pt1, pt2)) {
                        return true;
                    }
                }
            }
        }
        
        // Y扩展，判断是否有可直连的点
        expandY(v1, p1E);
        expandY(v2, p2E);
        for (auto pt1 : p1E) {
            for (auto pt2 : p2E) {
                if (pt1.y == pt2.y) {
                    if (linkD(pt1, pt2)) {
                        return true;
                    }
                }
            }
        }
        
        
        return false;
    }
    
    
    return false;
}


bool GameScene::linkD(Vec2 v1, Vec2 v2)
{
    // X坐标一致，逐个扫描Y坐标，中间如果都是通路，那么可以直连
    if (v1.x == v2.x)
    {
        int y1 = MIN(v1.y, v2.y);
        int y2 = MAX(v1.y, v2.y);
        
        bool flag = true;
        
        for (int y = y1 + 1; y < y2; y++)
        {
            if (mMap[(int)v1.x][y] != 0)
            {
                flag = false;
                break;
            }
        }
        
        if (flag)
        {
            return true;
        }
    }
    
    // Y坐标一致，逐个扫描X坐标，中间如果都是通路，那么可以直连
    if (v1.y == v2.y) {
        int x1 = MIN(v1.x, v2.x);
        int x2 = MAX(v1.x, v2.x);
        bool flag = true;
        for (int x = x1 + 1; x < x2; x++) {
            if (mMap[x][(int)v1.y] != 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return true;
        }
    }
    
    return false;
}

void GameScene::expandX(Vec2 v, std::vector<Vec2> &vector)
{
    // 注意第二个参数vector是传的引用
    vector.clear();
    
    // X轴扩展到边界，如果都是空的就将扩展的点放入容器
    for (int x = (int)v.x + 1; x < xCount; x++) {
        if (mMap[x][(int)v.y] != 0) {
            break;
        }
        vector.push_back(Vec2(x, (int)v.y));
    }
    
    // Y轴扩展到边界，如果都是空的就将扩展的点放入容器
    for (int x = (int)v.x - 1; x >= 0; x--) {
        if (mMap[x][(int)v.y] != 0) {
            break;
        }
        vector.push_back(Vec2(x, (int)v.y));
    }
}

void GameScene::expandY(Vec2 v, std::vector<Vec2> &vector)
{
    vector.clear();
    
    for (int y = (int)v.y + 1; y < yCount; y++) {
        if (mMap[(int)v.x][y] != 0) {
            break;
        }
        vector.push_back(Vec2((int)v.x, y));
    }
    
    for (int y = (int)v.y - 1; y >= 0 ; y--) {
        if (mMap[(int)v.x][y] != 0) {
            break;
        }
        vector.push_back(Vec2((int)v.x, y));
    }
}



