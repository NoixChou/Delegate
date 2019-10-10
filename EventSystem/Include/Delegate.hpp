#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <vector>
#include <algorithm>
#include "EventHandler.hpp"

namespace events
{
    class Delegate
    {
    private:
        std::vector<EventHandler> Handlers_;

    public:
        // ポインタを追加
        void operator+=(const EventHandler& handler)
        {
            Handlers_.push_back(handler);
        }

        // ポインタ削除
        void operator-=(const EventHandler& handler)
        {
            Handlers_.erase(std::remove(Handlers_.begin(), Handlers_.end(), handler));
        }

        // 指定クラスのポインタを削除
        void operator>>(const EventListener* class_)
        {
            Handlers_.erase(std::remove_if(Handlers_.begin(), Handlers_.end(), [&class_](const EventHandler h) { return h.GetClass() == class_; }), Handlers_.end());
        }

        // 呼び出し
        void operator()(Event& e) const
        {
            for (EventHandler h : Handlers_)
            {
                if (!h.Call(e) && e.isCancellable) break;
            }
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
