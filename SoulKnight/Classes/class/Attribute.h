#ifndef _ATTRIBUTE_
#define _ATTRIBUTE_

#include <memory>
#include <new>
#include "cocos2d.h"

template<typename T>
class Attribute :public cocos2d::Ref {
	friend class AttributeChanger;

public:
	//create
	static Attribute *createWithValue(T value) {
		Attribute *temp = new(std::nothrow) Attribute();

		if (temp && temp->initMember(value)) {
			temp->autorelease();
			return temp;
		}
		else {
			delete temp;
			temp = nullptr;
			return nullptr;
		}
	}

	//set
	virtual void setValue(T value) { _value = value; }

	//get
	virtual T getValue()const { return _value; }

protected:
	//destructor
	virtual ~Attribute() = default;

	/*virtual Attribute &operator=(T value) { _value = value; return *this; }
	virtual Attribute &operator+=(T value) { _value += value; return *this; }
	virtual void operator+(T value) { *this + value; }
	virtual Attribute &operator*=(T fold) { _value *= fold; return *this; }
	virtual void operator*(T fold) { *this * fold; }*/

	T _value;//µ±Ç°Öµ

private:

	virtual bool initMember(T value) { _value = value; return true; }
};

#endif // _ATTRIBUTE_
