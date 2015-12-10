
#include "LinkMap.h"
#include "MapData.h"

USING_NS_CC;

enum
{
	LinkMapOffsetWidth = 0,
	LinkMapOffsetHeight,
	LinkMapOffsetTotal,
	LinkMapOffsetTotal2,
	LinkMapOffsetTime,
	LinkMapOffsetTime2,
	LinkMapOffsetReserved,
	LinkMapOffsetReserved2,
	LinkMapOffsetData
};

bool LinkMap::init(int style)
{
	if(style <= 0 || style > MAPDATA_MAX)
		return false;

	m_style = style - 1;
	//m_map = (unsigned char *)mapTable[m_style];
	m_map = (unsigned char *)cardMapTable[m_style];
	//m_map = (unsigned char *)cardMapData56;

	CCAssert((m_map[LinkMapOffsetTotal] + m_map[LinkMapOffsetTotal2]) % 2 == 0, "Total Error");
	CCAssert((m_map[LinkMapOffsetTime] + m_map[LinkMapOffsetTime2]) != 0, "Total Error");

	int size = (int)m_map[LinkMapOffsetWidth] * m_map[LinkMapOffsetHeight];
	CCAssert(size > 0, "Size Error");

	m_count = 0;
	const unsigned char *p = &m_map[LinkMapOffsetData];
	while(size--)
	{
		if(*p == LinkMapTypeNormal || *p == LinkMapTypeFreeze)
			m_count++;
		p++;
	}

	CCAssert(m_count != 0, "Count Error");
/*	CCAssert(m_count % 2 == 0, "Count Error");*/
	CCAssert(m_count >= 8, "Count Error");

	return true;
}

LinkMap *LinkMap::create(int style)
{
	LinkMap *cardMap = new LinkMap();
	if(cardMap != NULL && cardMap->init(style))
	{
		cardMap->autorelease();
		return cardMap;
	}
	else
	{
		CC_SAFE_DELETE(cardMap);
		return NULL;
	}
}

int LinkMap::getMapTableMax()
{
	return MAPDATA_MAX;
}

int LinkMap::getStyle()
{
	return m_style;
}

int LinkMap::getTime()
{
	return (int)m_map[LinkMapOffsetTime] + m_map[LinkMapOffsetTime2];
}

int LinkMap::getWidth()
{
	return m_map[LinkMapOffsetWidth];
}

int LinkMap::getHeight()
{
	return m_map[LinkMapOffsetHeight];
}

int LinkMap::getSize()
{
	return (int)m_map[LinkMapOffsetWidth] * m_map[LinkMapOffsetHeight];
}

int LinkMap::getTotal()
{
	return m_map[LinkMapOffsetTotal] + m_map[LinkMapOffsetTotal2];
}

int LinkMap::getCardCount()
{
	return m_count;
}

int LinkMap::getCardType(int index)
{
	CCAssert(index >= 0 && index < m_map[LinkMapOffsetWidth] * m_map[LinkMapOffsetHeight], "Invalid Map Index");
	return (int)m_map[LinkMapOffsetData + index];
}