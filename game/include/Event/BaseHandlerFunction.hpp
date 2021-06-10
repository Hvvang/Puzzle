#pragma once

class Event;

class BaseHandlerFunction {
public:
    virtual ~BaseHandlerFunction() {};
    void exec(Event *event) {
        call(event);
    }

private:
    virtual void call(Event *) = 0;
};