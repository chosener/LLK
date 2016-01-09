//
//  StartScene.h
//  LLK
//
//  Created by lerry on 6/12/15.
//
//

#ifndef __LLK__StartScene__
#define __LLK__StartScene__

#include "cocos2d.h"

class StartGame : public cocos2d::Layer
{
protected:
    cocos2d::Sprite* mBird;
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void initUi(float t);
    
    void update(float dt);
    
    void play(Ref* sender);
    
    CREATE_FUNC(StartGame);
};


#endif /* defined(__LLK__StartScene__) */
