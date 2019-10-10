#ifndef MANAGED_EVENT_LISTENER_HPP
#define MANAGED_EVENT_LISTENER_HPP

#include "Delegate.hpp"

namespace events
{
    class ManagedEventListener : public EventListener
    {
    private:
        std::vector<Delegate*> Handlers_;

    public:
        ManagedEventListener() {}

        ~ManagedEventListener()
        {
            for (auto d : Handlers_)
            {
                (*d) >> this;
            }
        }

        void UseDelegate(Delegate& d)
        {
            Handlers_.push_back(&d);
        }
    };
}
#endif // MANAGED_EVENT_LISTENER_HPP
