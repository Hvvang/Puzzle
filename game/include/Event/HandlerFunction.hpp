#pragma once

class BaseHandlerFunction;
class Event;

template<class T, class EventType>
class HandlerFunction : public BaseHandlerFunction {
public:
    typedef void (T::*MemberFunction)(EventType*);

    HandlerFunction(T * instance, MemberFunction memberFunction) : instance{instance}, memberFunction{memberFunction} {};

    void call(Event *event) override {
        // Cast event to the correct type and call member function
        (instance->*memberFunction)(static_cast<EventType*>(event));
    }
private:
    // Pointer to class instance
    T * instance;

    // Pointer to member function
    MemberFunction memberFunction;
};