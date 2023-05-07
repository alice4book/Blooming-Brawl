#include "Tool.h"

bool Tool::isSpawned() {
	return tool_spawned;
}
void Tool::resetSpawn() {
	tool_spawned = false;
}
void Tool::setSpawn() {
	tool_spawned = true;
}
Tool::Tool(Entity* parent) : Component(parent) {};