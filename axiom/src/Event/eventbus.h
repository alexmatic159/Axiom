#pragma once
#include "event.h"

#include <unordered_map>
#include <vector>
#include <functional>

namespace AXIOM {

    class EventBus 
    {
    private:
        static EventBus* s_Instance;

        EventBus() {}

    public:
        static EventBus* Get() {
            if (!s_Instance)
                s_Instance = new EventBus();
            return s_Instance;
        }

        using Callback = std::function<void(const Event&)>;

        // Sottoscrizione generica
        void Subscribe(EventType type, Callback callback) {
            subscribers[type].push_back(callback);
        }

        // Emissione evento
        void Publish(const Event& event) {
            if (auto it = subscribers.find(event.type()); it != subscribers.end()) {
                for (auto& callback : it->second) {
                    callback(event);
                }
            }
        }

        // Helper template per derivati di Event
        template<typename T>
        void PublishDerived(const T& event) {
            static_assert(std::is_base_of_v<Event, T>, "Must inherit from Event");
            Publish(static_cast<const Event&>(event));
        }

    private:
        std::unordered_map<EventType, std::vector<Callback>> subscribers;
    };

}
