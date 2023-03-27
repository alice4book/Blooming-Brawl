#include <windows.h>
#include <mmsystem.h>
#include <string>
#include "Component.h"

class Entity;
class Audio : public Component{
public:
	Audio(Entity* parent);
	void playAudio(std::string path);
	void stopAudio();
};

