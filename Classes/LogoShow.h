//
//  LogoShow.h
//  ds2048
//
//  Created by yang on 15/4/28.
//
//

#ifndef __ds2048__LogoShow__
#define __ds2048__LogoShow__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class LogoShow : public CCLayer
{
public:
    LogoShow();
    ~LogoShow();
    
    CREATE_FUNC(LogoShow);
    
    virtual bool init();
    
private:
    void showLogo();
    void endLogo(CCObject* obj);
    
};

#endif /* defined(__ds2048__LogoShow__) */
