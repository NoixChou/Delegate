#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <sstream>
#include <functional>

#define ALLOW_FORCE_CAST
#include "ForceCast.hpp"
#undef ALLOW_FORCE_CAST

namespace delegate
{
    template<typename> class DelegateHandler;
    template<typename Ret_, typename... Args_>
    class DelegateHandler<Ret_(Args_...)>
    {
    public:
        using Caller = std::function<Ret_(Args_...)>;
        struct HandlerData
        {
            Caller call_;
            void* class_ = nullptr;
            void* func_ = nullptr;
        };

    private:
         HandlerData Handler_;

    public:
        DelegateHandler() = default;

        template<typename T, typename U>
        DelegateHandler(T obj, U func)
        {
            SetHandler<T, U>(obj, func);
        }

        DelegateHandler(Caller func)
        {
            SetHandler(func);
        }

        template<typename T, typename U>
        void SetHandler(T obj, U func)
        {
            Handler_.call_ = [obj, func](Args_... args) ->
            Ret_ { return (obj->*func)(args...); };
            Handler_.class_ = obj;
            Handler_.func_ = util::ForceVoid(func);
        }

        void SetHandler(Caller func)
        {
            Handler_.call_ = func;
            Handler_.class_ = nullptr;
            Handler_.func_ = nullptr;
        }

        Ret_ operator()(Args_... args) const
        {
            return Handler_.call_(args...);
        }

        void* GetClass() const
        {
            return Handler_.class_;
        }

        bool operator==(const DelegateHandler& left) const
        {
            return (this->Handler_.class_ == left.Handler_.class_) && (this->Handler_.func_ == left.Handler_.func_);
        }

        operator std::string() const
        {
            std::stringstream l_Str;
            l_Str << "DelegateHandler[0x" << Handler_.func_ << "@ 0x" << Handler_.class_ << "]";
            return l_Str.str();
        }
    };

}
#endif // EVENT_HANDLER_HPP
