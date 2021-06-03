#include <vector>
#include <string>
#include <map>

namespace railguard::core
{
    template <class T>
    using EventHandler = void* (const T&);

    template <class T>
    class EventSender
    {
    private:
        std::map<std::string, EventHandler<T>> _handlers;

    public:
        explicit EventSender(size_t defaultCapacity);
        void SendEvent(const T &data);
        void RegisterListener(std::string name, EventHandler<T> handler);
        void RemoveListener(std::string name);
    };
} // namespace railguard::core
