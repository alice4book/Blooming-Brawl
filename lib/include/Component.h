#include "ComponentType.h"

class Entity;
class Component
{
private:
    Entity* parent;
public:
	//Tells what kind of component
    ComponentType compType = eNone;

	explicit Component(Entity* parent);

	[[nodiscard]] ComponentType getComponentType() const;

	virtual bool isComponentType(ComponentType componentType);
};