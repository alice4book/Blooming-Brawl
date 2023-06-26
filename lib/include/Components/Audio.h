#include <windows.h>
#include <mmsystem.h>
#include <string>
#include "Component.h"
#include "soloud.h"

class Entity;
class Audio : public Component {
public:
    Audio(Entity* parent);

    ~Audio();

    void playMusic(const char* filePath, bool isLooped = false);
    void playBackgroundMusic(const char* filePath);

    void stopMusic();

private:
    SoLoud::Soloud* gSoloud; // SoLoud engine
};