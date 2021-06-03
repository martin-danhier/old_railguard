#include "core/EventSender.h"

namespace railguard::core
{
    template <class T>
    EventSender<T>::EventSender(size_t defaultCapacity) {
        _handlers.reserve(defaultCapacity);
    }

    template <class T>
    void EventSender<T>::SendEvent(const T &data) {
        // Call each event handler
        for (auto handler : _handlers)
        {
            handler(data);
        }
        
    }

    template <class T>
    void EventSender<T>::RegisterListener(std::string name, EventHandler<T> handler) {
        _handlers[name] = handler;
    }

    template <class T>
    void EventSender<T>::RemoveListener(std::string name) {
        _handlers.erase(name);
    }


} // namespace core
