#ifndef EVENT_SENDER_H
#define EVENT_SENDER_H

#include <functional>
#include <map>


namespace railguard::core
{
    template<class T>
    using EventHandler = std::function<void(const T &)>;

    /**
     * @brief Allows handlers to listen to events.
     *
     * An EventSender can be used to represent an event that can happen.
     *
     * Handlers can register to an event using the RegisterListener() method. Each handler is associated with a name,
     * that will represent it. Using this name, a handler can stop listening by calling the RemoveListener() method.
     *
     * Handlers are functions that return void and take a paremeter of type T, which is defined by the EventSender using the template parameter.
     *
     * If the event occurs (e.g keypress), the event can be propagated to every handlers by calling the SendEvent() function.
     *
     * @tparam T Type of the value that is passed to handlers
     */
    template <class T>
    class EventSender
    {
    private:
        std::map<std::string, EventHandler<T>> _handlers;

    public:
        // Implement them in the header because they are generic

        /**
         * @brief Send an event to every listener/
         *
         * @param data Data that can be passed to the handlers.
         */
        void SendEvent(const T &data)
        {
            // Call each event handler
            for (const auto& [_, handler]: _handlers)
            {
                handler(data);
            }
        }

        /**
         * @brief Registers an event handler that will then be called if the event is sent.
         *
         * @param name Name of the handler. Used to retreive it later, to stop listening.
         * @param handler Callback that will be called when the event is sent. Must return void and take a value `data` of type T as parameter.
         */
        void RegisterListener(std::string name, EventHandler<T> handler)
        {
            _handlers[name] = handler;
        }

        /**
         * @brief Removes a handler that was previously registered. It will stop receiving events.
         *
         * @param name Name of the handler to remove.
         */
        void RemoveListener(std::string name)
        {
            _handlers.erase(name);
        }
    };
} // namespace railguard::core

#endif // EVENT_SENDER_H