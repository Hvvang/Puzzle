#pragma once

#include <list>
#include <map>
#include <typeindex>

#include <Event/BaseHandlerFunction.hpp>
#include <Event/HandlerFunction.hpp>

typedef std::list<BaseHandlerFunction*> HandlerList;
class EventSystem {
public:
    template<typename EventType>
    void emit(EventType *event) {
        HandlerList *handlers = subscribers[typeid(EventType)];

        if (!handlers)
            return;

        for (auto & handler : *handlers) {
            if (handler)
                handler->exec(event);
        }
    }

    template<class T, class EventType>
    void connect(T *instance, void (T::*memberFunction)(EventType *)) {
        HandlerList *handlers = subscribers[typeid(EventType)];

        //First time initialization
        if (!handlers) {
            handlers = new HandlerList();
            subscribers[typeid(EventType)] = handlers;
        }

        handlers->push_back(new HandlerFunction<T, EventType>(instance, memberFunction));
    }
private:
    std::map<std::type_index, HandlerList*> subscribers;
};


