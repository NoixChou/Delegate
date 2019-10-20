#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <vector>
#include <algorithm>
#include "EventHandler.hpp"
#include <unordered_map>

namespace delegate
{
    template<class> class Delegate;
    template<typename Ret_, typename... Args_>
    class Delegate<Ret_(Args_...)>
    {
    public:
        //using Handlers = std::vector<EventHandler<Ret_, Args_...>>;
        using Handler = EventHandler<Ret_(Args_...)>;
        using Handlers = std::list<Handler>;
        using Iterator = typename Handlers::iterator;
    private:
        Handlers Handlers_;

    public:
        Delegate() {}

        // ポインタを追加
        void operator+=(const Handler& handler)
        {
            Handlers_.emplace_back(handler);
        }

        // ポインタ削除
        void operator-=(const Handler& handler)
        {
            Handlers_.erase(std::remove(Handlers_.begin(), Handlers_.end(), handler));
        }

        void operator-=(void* class_)
        {
            Handlers_.erase(std::remove_if(Handlers_.begin(), Handlers_.end(), [&class_](const EventHandler<Ret_(Args_...)> h)
                {
                    return h.GetClass() == class_;
                }), Handlers_.end());
        }

        // 呼び出し
        std::vector<Ret_> operator()(Args_... args) const
        {
            std::vector<Ret_> l_RetList;
            l_RetList.resize(Handlers_.size());

            uint_fast64_t i = 0;
            for (const auto& l_Handler : Handlers_)
            {
                l_RetList[i] = l_Handler(args...);
                ++i;
            }

            return l_RetList;
        }

        /* キャスト演算子 */
        operator std::string() const
        {
            std::string l_Str = "Delegate { ";
            for (const auto& l_Handler : Handlers_)
            {
                l_Str += l_Handler;
                l_Str += " ";
            }

            l_Str += "}";
            return l_Str;
        }
    };
}
#endif
