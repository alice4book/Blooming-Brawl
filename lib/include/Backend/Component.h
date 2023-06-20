#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;
class Component
{
protected:
    Entity* parent;
	bool enabled;
public:
	explicit Component(Entity* parent) : parent(parent), enabled(true) { };

	virtual void update(){ };

	Entity* getParent();

	virtual void enable(bool value);

    ~Component();
};

#endif