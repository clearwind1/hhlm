#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "cocos2d.h"

USING_NS_CC;

class BackGround : public CCLayer
{
public:
	BackGround();
	~BackGround();
	static BackGround *shared();

    virtual bool init();  

    CREATE_FUNC(BackGround);

	void setRedColor(bool value);
	bool getRedColor();

private:
	void updateFireworks(float dt);
	void setRedColorCallBack();

private:
	bool m_isRedColor;
	CCLayerColor *m_layerColor;
};

#endif // __BACKGROUND_H__
