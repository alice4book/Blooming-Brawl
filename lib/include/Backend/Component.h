#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Component
{
protected:
    Entity* parent;
public:
	explicit Component(Entity* parent) : parent(parent) { };

	virtual void update(){ };

	Entity* getParent();

    ~Component();
};

#endif