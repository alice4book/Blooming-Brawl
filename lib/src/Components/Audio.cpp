#include "Audio.h"
#include <iostream>
#include <soloud_wav.h>

Audio::Audio(Entity* parent): Component(parent) {
    // Initialize SoLoud
    gSoloud = new SoLoud::Soloud;
    gSoloud->init();
}

Audio::~Audio(){
    // Deinitialize SoLoud
    gSoloud->deinit();
}

void Audio::playMusic(const char* filePath, bool isLooped = false) {
    // Load the music file
    SoLoud::Wav* music = new SoLoud::Wav;
    if (music->load(filePath) != 0) {
        // Failed to load the music file
        std::cout << "Failed to load the music fil" << std::endl;
        return;
    }
    music->setLooping(isLooped);
    // Play the music
    int handle = gSoloud->play(*music);         // Play the sound
}

void Audio::stopMusic() {
    // Stop all playing sounds
    gSoloud->stopAll();
}