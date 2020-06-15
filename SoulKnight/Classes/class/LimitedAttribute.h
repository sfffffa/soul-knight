#ifndef _LIMITEDATTRIBUTE_
#define _LIMITEDATTRIBUTE_

#include "Attribute.h"
#include "cocos2d.h"
#include <new>

template <typename T>
class LimitedAttribute :public Attribute<T> {
	friend class AttributeChanger;

public:
	//create value 和value 均设为实参value 值
	static LimitedAttribute *createWithValue(T value) {
		LimitedAttribute *temp = new(std::nothrow) LimitedAttribute();

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

	static LimitedAttribute *createWithValueMax(T value, T valueMax) {
		LimitedAttribute *temp = new(std::nothrow) LimitedAttribute();

		if (temp &&temp->initMember(value, valueMax)) {
			temp->autorelease();
			return temp;
		}
		else {
			delete temp;
			temp = nullptr;
			return nullptr;
		}
	}

	//you should set valueMax before setting value
	void setValue(T value) { _value = (value > _valueMax) ? _valueMax : value; }
	virtual void setValueMax(T valueMax) { _valueMax = valueMax; if (_value > valueMax)_value = valueMax; }

	//get
	virtual T getValueMax()const { return _valueMax; }

protected:
	virtual ~LimitedAttribute() = default;

	/*LimitedAttribute &operator=(T value)override {
		_value = (value > _valueMax) ? _valueMax : value;
		return *this;
	};
	LimitedAttribute &operator+=(T value)override {
		_value += value;
		if (_value > _valueMax)_value = _valueMax;
		return *this;
	}
	void operator+(T value)override { *this + value; }
	LimitedAttribute &operator*=(T fold) {
		_value *= fold;
		if (_value > _valueMax)_value = _valueMax;
		return *this;
	}
	void operator*(T fold)override { *this * fold; }*/

	T _valueMax;//属性上限

private:
	bool initMember(T value)override { _value = _valueMax = value; return true; }
	virtual bool initMember(T value, T valueMax) {
		_valueMax = valueMax;
		_value = (value > valueMax) ? valueMax : value;
		return true;
	}
};

#endif // _LIMITEDATTRIBUTE_
