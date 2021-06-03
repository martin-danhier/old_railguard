#include <vector>
#include <string>
#include <map>
#include <functional>

namespace railguard::core
{
    template <class T>
    using EventHandler = std::function<void(const T &)>;

    template <class T>
    class EventSender
    {
    private:
        std::map<std::string, EventHandler<T>> _handlers;

    public:
        // Implement them in the header because they are generic

        void SendEvent(const T &data)
        {
            // Call each event handler
            for (const auto& [_, handler]: _handlers)
            {
                handler(data);
            }
        }

        void RegisterListener(std::string name, EventHandler<T> handler)
        {
            _handlers[name] = handler;
        }

        void RemoveListener(std::string name)
        {
            _handlers.erase(name);
        }
    };
} // namespace railguard::core
