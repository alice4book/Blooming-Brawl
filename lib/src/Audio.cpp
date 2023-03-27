#include "Audio.h"

Audio::Audio(Entity* parent) : Component(parent) {

}

void Audio::playAudio(std::string path) {
	mciSendString(("play " + path).c_str(), NULL, 0, 0);
}
