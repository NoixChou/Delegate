#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

class Event
{
public:
    std::string name;
    bool isCancellable = true;
};
#endif // EVENT_HPP
