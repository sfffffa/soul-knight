#ifndef _ELEMENTAL_BULLET_
#define _ELEMENTAL_BULLET_

#include "Bullet.h"
#include <new>

class ElementalBullet :public Bullet {
public:
	enum Element { ICE, FIRE, POISON };

	static ElementalBullet *create(
		int damage = 0, float speed = 0.0f, bool crit = false, Element element = ICE);

	static ElementalBullet *createWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage = 0, float speed = 0.0f, bool crit = false, Element element = ICE);

	static ElementalBullet *createWithSpriteFrameName(const std::string &spriteFrameName,
		int damage = 0, float speed = 0.0f, bool crit = false, Element element = ICE);

	virtual void setElement(Element element) { _element->setValue(element); }

	virtual Element getElement()const { return _element->getValue(); }

	virtual Attribute<Element> *getElementInstance()const { return _element; }

	ElementalBullet *clone()const override;

protected:
	//destructor
	virtual ~ElementalBullet() = default;

	bool init(
		int damage, float speed, bool crit, Element element);

	bool initWithSpriteFrame(SpriteFrame *spriteFrame,
		int damage, float speed, bool crit, Element element);

	bool initWithSpriteFrameName(const std::string &spriteFrameName,
		int damage, float speed, bool crit, Element element);

	Attribute<Element> *_element;//ÔªËØ

private:

	bool initMember(Element element);
};

#endif // _ELEMENTAL_BULLET_
