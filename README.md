# Delegate
オレオレ~~イベント~~デリゲートライブラリ

# つかいかた
- Delegate.hppインクルード。
- delegate::Delegateがデリゲート型、delegate::ManagedEventListenerを継承したクラスにイベントハンドラを持てる。
- デリゲートは+=演算子でハンドラ追加、-=演算子でハンドラ削除ができる
- デリゲートに追加するときはdelegate::DelegateHandler(クラスのポインタ[this], ハンドラ関数[bool MemberMethod(Event& e)])を使って追加
- delegate::Delegate -= クラスのポインタ でそのクラスのハンドラがまとめて削除される
- delegate::ManagedEventListener::UseDelegate(Delegate型)でデリゲートを登録すれば、クラス開放時にデリゲートからそのクラスのハンドラが削除される

コードを書けば
```cpp
#include <iostream>
#include <string>
#include "EventSystem/Include/Delegate.hpp"

delegate::Delegate<bool(const std::string&)> g_Delegate;

class TestListener
public:
    TestListener()
    {
        g_Delegate += delegate::DelegateHandler<bool(const std::string&)>(this, &TestListener::TestHandler);
        g_Delegate += delegate::DelegateHandler<bool(const std::string&)>(this, &TestListener::TestHandler2);
    }
    
    bool TestHandler(const std::string& e)
    {
        std::cout << "TestHandlerがよばれた！！！ メッセージ: " << e << std::endl;
        return true;
    }
    
    bool TestHandler2(const std::string& e)
    {
        std::cout << "TestHandler2がよばれた！！！ メッセージ: " << e << std::endl;
        return true;
    }
};

void main()
{
    auto l_Listener = new TestListener();

    g_Delegate("Test Message");
    
	g_Delegate -= l_Listener; // l_Listenerのメンバ関数ハンドラを全て削除

    delete l_Listener;
    
    g_Delegate("Test Message 2"); // 何も呼ばれない
}
```

こんなかんじ。
