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

        // �|�C���^��ǉ�
        void operator+=(const EventHandler<Ret_, Args_...>& handler)
        {
            Handlers_.push_back(handler);
        }

        // �|�C���^�폜
        void operator-=(const EventHandler<Ret_, Args_...>& handler)
        {
            Handlers_.erase(std::remove(Handlers_.begin(), Handlers_.end(), handler));
        }

        // �w��N���X�̃|�C���^���폜
        void operator>>(const EventListener* class_)
        {
            Handlers_.erase(std::remove_if(Handlers_.begin(), Handlers_.end(), [&class_](const EventHandler<Ret_, Args_...> h) { return h.GetClass() == class_; }), Handlers_.end());
        }

        // �L�����Z��������ݒ�
        void SetCancelCondition(std::function<bool(Ret_, Args_...)> condition)
        {
            CancelCondition_ = condition;
        }

        // �Ăяo��
        std::vector<Ret_> operator()(Args_... args) const
        {
            std::vector<Ret_> ret_list;
            for (EventHandler<Ret_, Args_...> h : Handlers_)
            {
                auto ret = h.Call(args...);
                ret_list.push_back(ret);
                if (CancelCondition_ != nullptr)
                {
                    if(CancelCondition_(ret, args...))
                    {
                        return ret_list;
                    }
                }
            }

            return ret_list;
        }

        /* �L���X�g���Z�q */

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
