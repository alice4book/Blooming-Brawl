#include "TimeManager.h"
#include "Component.h"
#include "GLFW/glfw3.h"

#define DELTA_TIME_120FPS (7.5f/900.f)
#define DELTA_TIME_60FPS (15.f/900.f)
#define DELTA_TIME_2FPS 0.5f

// Define the static Singleton pointer
TimeManager* TimeManager::inst_ = nullptr;

TimeManager* TimeManager::getInstance() {
    if (inst_ == nullptr) {
        inst_ = new TimeManager();
    }
    return(inst_);
}

void TimeManager::updateTime(){
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // skip first frame because of lag on load
    if(deltaTime >= 0.1f) {
        deltaTime = 0.f;
        return;
    }

    notify(listObserverUnlimited);

    time120FPS += deltaTime;
    deltaTime120FPS += deltaTime;
    time60FPS += deltaTime;
    deltaTime60FPS += deltaTime;
    time2FPS += deltaTime;
    deltaTime2FPS += deltaTime;

    if(time120FPS >= DELTA_TIME_120FPS){
        notify(listObserver120FPS);
        time120FPS = 0.f;
        deltaTime120FPS = 0.f;
    }

    if(time60FPS >= DELTA_TIME_60FPS){
        notify(listObserver60FPS);
        time60FPS = 0.f;
        deltaTime60FPS = 0.f;
    }

    if(time2FPS >= DELTA_TIME_2FPS){
        notify(listObserver2FPS);;
        time2FPS = 0.f;
        deltaTime2FPS = 0.f;
    }
}

void TimeManager::notify(std::list<Component*> listObserver) {
    auto component = listObserver.begin();

    if (dynamic_cast<Component*>(*component) == nullptr)
        return;

    while (component != listObserver.end()) {
        (*component)->update();
        ++component;
    }
}

void TimeManager::attachUnlimitedFPS(Component* observer) {
    listObserverUnlimited.push_back(observer);
}

void TimeManager::attach120FPS(Component* observer) {
    listObserver120FPS.push_back(observer);
}

void TimeManager::attach60FPS(Component* observer) {
    listObserver60FPS.push_back(observer);
}

void TimeManager::attach2FPS(Component* observer) {
    listObserver2FPS.push_back(observer);
}

void TimeManager::detach(Component *observer) {
    listObserverUnlimited.remove(observer);
    listObserver120FPS.remove(observer);
    listObserver60FPS.remove(observer);
    listObserver2FPS.remove(observer);
}

float TimeManager::getDeltaTimeUnlimitedFPS() const {
    return deltaTime;
}

float TimeManager::getDeltaTime120FPS() const {
    return deltaTime;
}

float TimeManager::getDeltaTime60FPS() const {
    return deltaTime;
}

float TimeManager::getDeltaTime2FPS() const {
    return deltaTime;
}
