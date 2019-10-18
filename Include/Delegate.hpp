#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <vector>
#include <algorithm>
#include "EventHandler.hpp"
#include <unordered_map>

namespace events
{
    template<typename Ret_, typename... Args_>
    class Delegate
    {
    private:
        std::vector<EventHandler<Ret_, Args_...>> Handlers_;
        std::function<bool(Ret_, Args_...)> CancelCondition_ = nullptr;

    public:
        Delegate() {}
        Delegate(std::function<bool(Ret_, Args_...)> condition)
        {
            SetCancelCondition(condition);
        }

        // ポインタを追加
        void operator+=(const EventHandler<Ret_, Args_...>& handler)
        {
            Handlers_.push_back(handler);
        }

        // ポインタ削除
        void operator-=(const EventHandler<Ret_, Args_...>& handler)
        {
            Handlers_.erase(std::remove(Handlers_.begin(), Handlers_.end(), handler));
        }

        // 指定クラスのポインタを削除
        void operator>>(const EventListener* class_)
        {
            Handlers_.erase(std::remove_if(Handlers_.begin(), Handlers_.end(), [&class_](const EventHandler<Ret_, Args_...> h) { return h.GetClass() == class_; }), Handlers_.end());
        }

        // キャンセル条件を設定
        void SetCancelCondition(std::function<bool(Ret_, Args_...)> condition)
        {
            CancelCondition_ = condition;
        }

        // 呼び出し
        std::vector<Ret_> operator()(Args_... args) const
        {
            std::vector<Ret_> ret_list;
            ret_list.resize(Handlers_.size());

            uint_fast64_t i = 0;
            for (EventHandler<Ret_, Args_...> h : Handlers_)
            {
                Ret_ l_Ret = h(args...);
                ret_list[i] = std::move(l_Ret);
                if (CancelCondition_ != nullptr)
                {
                    if(CancelCondition_(l_Ret, args...))
                    {
                        return ret_list;
                    }
                }
                ++i;
            }

            return ret_list;
        }

        /* キャスト演算子 */

        operator std::string() const
        {
            std::string str = "Delegate{ ";
            for (auto h : Handlers_)
            {
                str += h;
                str += " ";
            }

            str += "}";
            return str;
        }
    };
}
#endif
