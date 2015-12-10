

#ifndef __DIGIT_SPRITE_H__
#define __DIGIT_SPRITE_H__


#include "cocos2d.h"

USING_NS_CC;

class DigitSprite : public CCSprite
{
public:
	DigitSprite();
	static DigitSprite *create(int mode);
	virtual void onExit();
	void output(int value);
	void output(int value,int style);
	void output(float value);
	void output(const char *str);
	void outputWithFormat(const char *format,...);

	void setSizeX(float x);
	void setSizeY(float y);
	float getSizeX();
	float getSizeY();

    virtual GLubyte getOpacity(void);
    virtual void setOpacity(GLubyte opacity);

	virtual void setScale(float scale);
	virtual float getScale();

private:
	void ActionEnd();
	void outputEx(int value);
	void updateOutput(float dt);

private:
	int m_mode;
	int m_value;
	int m_curValue;
	int m_step;
	int m_width;
	int m_height;
	float m_scale;
	GLubyte m_opacity;
	CCSpriteBatchNode *m_spriteBatchNode;
};

#endif //__DIGIT_SPRITE_H__

