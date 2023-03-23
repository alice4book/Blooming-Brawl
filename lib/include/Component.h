enum ComponentType{
    eNone,
    eModel,
    eCollisionStatic,
    eCollisionDynamic
};

class Component
{
public:
	//Tells what kind of component
    ComponentType compType;

	Component();

	virtual ComponentType getComponentType();

	virtual bool isComponentType(ComponentType compType);
};