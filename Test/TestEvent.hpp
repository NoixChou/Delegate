﻿#ifndef TEST_EVENT_HPP
#define TEST_EVENT_HPP

#include "Event.hpp"

class TestEvent : public Event
{
public:
    std::string message_ = "";
    TestEvent(const std::string& message)
    {
        message_ = message;
    }
};
#endif // TEST_EVENT_HPP
