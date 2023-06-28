#ifndef OPENGLGP_TIMEMANAGER_H
#define OPENGLGP_TIMEMANAGER_H

#include "Component.h"
#include <vector>
#include <memory>

class TimeManager {
public:
    void updateTime();


    [[nodiscard]] float getDeltaTimeUnlimitedFPS() const;
    [[nodiscard]] float getDeltaTime120FPS() const;
    [[nodiscard]] float getDeltaTime60FPS() const;
    [[nodiscard]] float getDeltaTime2FPS() const;
    [[nodiscard]] float getDeltaTime1FPS() const;

    virtual ~TimeManager() = default;
    void attachUnlimitedFPS(Component* observer);
    void attachUnlimitedFPS(std::shared_ptr<Component> observer);
    void attach120FPS(Component* observer);
    void attach60FPS(Component* observer);
    void attach2FPS(Component* observer);
    void attach1FPS(Component* observer);
    void detach(Component* observer);

    // This is how clients can access the single instance
    static TimeManager* getInstance();

private:
    float deltaTime = 0.f;
    float lastFrame = 0.f;
    float time120FPS = 0.f, deltaTime120FPS = 0.f;
    float time60FPS = 0.f, deltaTime60FPS = 0.f;
    float time2FPS = 0.f, deltaTime2FPS = 0.f;
    float time1FPS = 0.f, deltaTime1FPS = 0.f;

    static void notify(std::vector<Component*> listObserver);

    std::vector<Component*> listObserverUnlimited;
    std::vector<Component*> listObserver120FPS;
    std::vector<Component*> listObserver60FPS;
    std::vector<Component*> listObserver2FPS;
    std::vector<Component*> listObserver1FPS;
    std::vector<std::shared_ptr<Component>> sharedPointers;

    static TimeManager* inst_;   // The one, single instance
    TimeManager() = default; // private constructor
    TimeManager(const TimeManager&);
    TimeManager& operator=(const TimeManager&);
};


#endif //OPENGLGP_TIMEMANAGER_H
