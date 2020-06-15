#ifndef _LIMITEDATTRIBUTE_
#define _LIMITEDATTRIBUTE_

#include "Attribute.h"

template <typename T>
class LimitedAttribute :public Attribute<T> {
	friend class AttributeChanger;

public:
	//create
	static std::shared_ptr<LimitedAttribute> createWithValue(T value) {
		auto temp = std::shared_ptr<LimitedAttribute>();

		if (temp && temp->initMember(value)) {
			return temp;
		}
		else {
			return std::shared_ptr<LimitedAttribute>(nullptr);
		}
	}

	static std::shared_ptr<LimitedAttribute> createWithValueMax(T value, T valueMax) {
		auto temp = std::shared_ptr<LimitedAttribute>();

		if (temp &&temp->initMember(value, valueMax)) {
			return temp;
		}
		else {
			return std::shared_ptr<LimitedAttribute>(nullptr);
		}
	}

	//you should set valueMax before setting value
	void setValue(T value) { _value = (value > _valueMax) ? _valueMax : value; }
	virtual void setValueMax(T valueMax) { _valueMax = valueMax; if (value > valueMax)value = valueMax; }

	//get
	virtual T getValueMax()const { return _valueMax; }

protected:
	virtual ~LimitedAttribute() = default;

	LimitedAttribute &operator=(T value)override {
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
	void operator*(T fold)override { *this * fold; }

	T _valueMax;//ÊôÐÔÉÏÏÞ

private:
	bool initMember(T value)override { _value = _valueMax = value; return true; }
	virtual bool initMember(T value, T valueMax) {
		_valueMax = valueMax;
		_value = (value > valueMax) ? valueMax : value;
		return true;
	}
};

#endif // _LIMITEDATTRIBUTE_
