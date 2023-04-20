#ifndef OPENGLGP_TIMEMANAGER_H
#define OPENGLGP_TIMEMANAGER_H

#include "Component.h"
#include <list>

class TimeManager {
public:
    void updateTime();


    [[nodiscard]] float getDeltaTimeUnlimitedFPS() const;
    [[nodiscard]] float getDeltaTime120FPS() const;
    [[nodiscard]] float getDeltaTime60FPS() const;
    [[nodiscard]] float getDeltaTime2FPS() const;

    virtual ~TimeManager() = default;
    void attachUnlimitedFPS(Component* observer);
    void attach120FPS(Component* observer);
    void attach60FPS(Component* observer);
    void attach2FPS(Component* observer);
    void detach(Component* observer);

    // This is how clients can access the single instance
    static TimeManager* getInstance();

private:
    float deltaTime = 0.f;
    float lastFrame = 0.f;
    float time120FPS = 0.f;
    float deltaTime120FPS = 0.f;
    float time60FPS = 0.f;
    float deltaTime60FPS = 0.f;
    float time2FPS = 0.f;
    float deltaTime2FPS = 0.f;

    static void notify(std::list<Component*> listObserver);

    std::list<Component*> listObserverUnlimited;
    std::list<Component*> listObserver120FPS;
    std::list<Component*> listObserver60FPS;
    std::list<Component*> listObserver2FPS;

    static TimeManager* inst_;   // The one, single instance
    TimeManager() = default; // private constructor
    TimeManager(const TimeManager&);
    TimeManager& operator=(const TimeManager&);
};


#endif //OPENGLGP_TIMEMANAGER_H
