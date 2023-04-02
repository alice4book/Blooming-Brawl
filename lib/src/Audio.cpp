#include "Audio.h"

Audio::Audio(Entity* parent) : Component(parent) {

}

void Audio::playAudio(std::string alias) {
	mciSendString(("play " + alias).c_str(), NULL, 0, NULL);
}

void Audio::stopAudio(std::string alias) {
	mciSendString(("stop " + alias).c_str(), NULL, 0, NULL);
}

void Audio::pauseAudio(std::string alias) {
	mciSendString(("pause " + alias).c_str(), NULL, 0, NULL);
}

void Audio::resumeAudio(std::string alias) {
	mciSendString(("resume " + alias).c_str(), NULL, 0, NULL);
}

void Audio::playLoop(std::string alias) {
	mciSendString(("play " + alias +" repeat").c_str(), NULL, 0, NULL);
}

void Audio::openAudio(std::string path, std::string alias) {
	mciSendString(("open " + path + " type mpegvideo alias "+ alias).c_str(), NULL, 0, NULL);
}

void Audio::closeAudio(std::string alias) {
	mciSendString(("close " + alias).c_str(), NULL, 0, NULL);
}