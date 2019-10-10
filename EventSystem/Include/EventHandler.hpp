#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <sstream>
#include "Event.hpp"
#include "EventListener.hpp"

#define ALLOW_FORCE_CAST
#include "ForceCast.hpp"
#undef ALLOW_FORCE_CAST

namespace events
{

    class EventHandler
    {
    public:
        using Handler = bool(EventListener::*)(Event&);

    private:
        EventListener* Class_ = nullptr;
        Handler Function_ = nullptr;

    public:
        EventHandler() = default;

        template<typename T>
        EventHandler(EventListener* c, T f)
        {
            SetHandler(c, f);
        }

        template<typename T>
        void SetHandler(EventListener* c, T f)
        {
            Class_ = c;
            Function_ = util::ForceCast<T, Handler>(f);
        }

        bool Call(Event& e) const
        {
            return (Class_->*Function_)(e);
        }

        const EventListener* GetClass() const
        {
            return Class_;
        }

        bool operator==(const EventHandler& left) const
        {
            return this->Function_ == left.Function_;
        }

        operator std::string() const
        {
            std::stringstream str;
            str << "EventHandler[0x" << Class_ << "@" << Function_ << "]";
            return str.str();
        }
    };

}
#endif // EVENT_HANDLER_HPP
