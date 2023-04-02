#include <windows.h>
#include <mmsystem.h>
#include <string>
#include "Component.h"

class Entity;
class Audio : public Component{
public:
	Audio(Entity* parent);
	void openAudio(std::string path, std::string alias);
	void closeAudio(std::string alias);
	void playAudio(std::string alias);
	void stopAudio(std::string alias);
	void pauseAudio(std::string alias);
	void resumeAudio(std::string alias);
	void playLoop(std::string alias);
};

