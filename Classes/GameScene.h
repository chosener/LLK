//
//  GameScene.h
//  LLK
//
//  Created by lerry on 6/16/15.
//
//

#ifndef __LLK__GameScene__
#define __LLK__GameScene__
#include "GameConfig.h"
#include "cocos2d.h"


class GameScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void initUI();
    
    void change();
    
    void initMap();
    
    void drawMap();
    
    void update(float dt);
    
    CREATE_FUNC(GameScene);
    
private:
    
    void playBgMusic();
    
    void playBgMusic1();
    
    void playBgMusic2();
    
    cocos2d::Vec2 indextoScreen(int x, int y);
    
    cocos2d::Vec2 screentoIndex(float x, float y);
    
    void drawLine();
    
    bool link(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
    
    bool linkTemp(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
    
    bool linkD(cocos2d::Vec2 v1, cocos2d::Vec2 v2);
    
    void expandX(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector);
    
    void expandY(cocos2d::Vec2 v, std::vector<cocos2d::Vec2> &vector);
    
    void clearMatched();
    
    void gameOver(std::string title);
    
    void playAgain();
    
    ///自动清除
    void autoClear(Ref *spender);
    
    bool die();
    
    bool dieTemp();
    
    bool win();
    
    void changeMap();
    
    int mMap[xCount][yCount] = {0};
    
    std::vector<cocos2d::Vec2> mSelected;
    
    std::vector<cocos2d::Vec2> mPath;
    
    cocos2d::DrawNode* mDraw;
    
    cocos2d::Vec2 mPre;
    
    cocos2d::ProgressTimer *progress;
    
    cocos2d::Label *numberTime;
    
};


#endif /* defined(__LLK__GameScene__) */
