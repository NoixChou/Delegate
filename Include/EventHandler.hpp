#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <sstream>
#include <functional>
#include "Event.hpp"
#include "EventListener.hpp"

#define ALLOW_FORCE_CAST
#include "ForceCast.hpp"
#undef ALLOW_FORCE_CAST

namespace delegate
{
    template<class> class EventHandler;
    template<typename Ret_, typename... Args_>
    class EventHandler<Ret_(Args_...)>
    {
    public:
        struct HandlerData
        {
            std::function<Ret_(Args_...)> call_;
            void* class_ = nullptr;
            void* func_ = nullptr;
        };

    private:
         HandlerData Handler_;

    public:
        EventHandler() = default;

        template<typename T, typename U>
        EventHandler(T obj, U func)
        {
            SetHandler<T, U>(obj, func);
        }

        template<typename T, typename U>
        void SetHandler(T obj, U func)
        {
            Handler_.call_ = [obj, func](Args_... args) ->
            Ret_ { return (obj->*func)(args...); };
            Handler_.class_ = obj;
            Handler_.func_ = util::ForceVoid(func);
        }

        Ret_ operator()(Args_... args) const
        {
            return Handler_.call_(args...);
        }

        void* GetClass() const
        {
            return Handler_.class_;
        }

        bool operator==(const EventHandler& left) const
        {
            return (this->Handler_.class_ == left.Handler_.class_) && (this->Handler_.func_ == left.Handler_.func_);
        }

        operator std::string() const
        {
            std::stringstream l_Str;
            l_Str << "EventHandler[0x" << Handler_.func_ << "@ 0x" << Handler_.class_ << "]";
            return l_Str.str();
        }
    };

}
#endif // EVENT_HANDLER_HPP
