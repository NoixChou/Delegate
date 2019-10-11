#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <sstream>
#include <functional>
#include "Event.hpp"
#include "EventListener.hpp"

#define ALLOW_FORCE_CAST
#include "ForceCast.hpp"
#undef ALLOW_FORCE_CAST

namespace events
{
    template<typename Ret_, typename... Args_>
    class EventHandler
    {
    public:
        //using Handler = bool(EventListener::*)(Event&);
        using HandlerSet = std::pair<void*, void*>;

        struct HandlerData
        {
            std::function<Ret_(Args_...)> func_;
            HandlerSet ptrs_;
        };

    private:
         HandlerData Handler_;

    public:
        EventHandler() = default;

        template<typename T, typename U>
        EventHandler(T c, U f)
        {
            SetHandler<T, U>(c, f);
        }

        template<typename T, typename U>
        void SetHandler(T c, U f)
        {
            Handler_.func_ = [c, f](Args_... args) ->
            Ret_ { return (c->*f)(args...); };
            Handler_.ptrs_ = std::make_pair(util::ForceVoid(f), c);
        }

        Ret_ Call(Args_... args) const
        {
            return Handler_.func_(args...);
            //return (Class_->*Function_)(e);
        }

        const void* GetClass() const
        {
            return Handler_.ptrs_.second;
        }

        bool operator==(const EventHandler& left) const
        {
            return this->Handler_.ptrs_ == left.Handler_.ptrs_;
        }

        operator std::string() const
        {
            std::stringstream str;
            str << "EventHandler[0x" << Handler_.ptrs_.second << "@" << Handler_.ptrs_.first << "]";
            return str.str();
        }
    };

}
#endif // EVENT_HANDLER_HPP
