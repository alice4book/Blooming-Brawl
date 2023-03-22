#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <glad/glad.h>
class Component
{
public:
	//Tells what kind of component
	std::string compType;

	Component() {
		this->compType = "";
	}

	virtual std::string getComponentType() {
		return this->compType;
	}

	virtual bool isComponentType(std::string compType) {
		if (this->compType == compType) {
			return true; 
		}
		return false;
	}
};