#ifndef _ATTRIBUTE_CHANGER_
#define _ATTRIBUTE_CHANGER_

#include "Attribute.h"

template<typename T>
class AttributeChanger {
public:
	enum OpSym { ADD, SUBTRACT, MULTIPLY, DIVIDE, ASSIGN };

	AttrbuteChanger(std::shared_ptr<Attribute<T>> targetAttribute) :_targetAttribute(targetAttribute) {}
	virtual ~AttributeChanger() = default;

	virtual void operator()(OpSym opSym, T value) {
		switch (opSym)
		{
		case ADD:
			_targetAttribute->operator +(value); break;
		case SUBTRACT:
			_targetAttribute->operator +(-value); break;
		case MULTIPLY:
			_targetAttribute->operator *(value); break;
		case DIVIDE:
			_targetAttribute->operator *(1 / value); break;
		case ASSIGN:
			_targetAttribute->operator =(value); break;
		default:
			break;
		}
	}

protected:
	std::shared_ptr<Attribute<T>> _targetAttribute;
}

#endif // _ATTRIBUTE_CHANGER_
