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
        // �|�C���^��ǉ�
        void operator+=(const EventHandler& handler)
        {
            Handlers_.push_back(handler);
        }

        // �|�C���^�폜
        void operator-=(const EventHandler& handler)
        {
            Handlers_.erase(std::remove(Handlers_.begin(), Handlers_.end(), handler));
        }

        // �w��N���X�̃|�C���^���폜
        void operator>>(const EventListener* class_)
        {
            Handlers_.erase(std::remove_if(Handlers_.begin(), Handlers_.end(), [&class_](const EventHandler h) { return h.GetClass() == class_; }), Handlers_.end());
        }

        // �Ăяo��
        void operator()(Event& e) const
        {
            for (EventHandler h : Handlers_)
            {
                if (!h.Call(e) && e.isCancellable) break;
            }
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
