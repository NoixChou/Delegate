#ifndef TEST_EVENT_HPP
#define TEST_EVENT_HPP

#include "..//Include/Event.hpp"

class TestEvent : public events::Event
{
public:
    std::string message_ = "";
    TestEvent(const std::string& message)
    {
        message_ = message;
    }
};
#endif // TEST_EVENT_HPP
