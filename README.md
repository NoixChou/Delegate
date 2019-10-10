# EventSystem
オレオレイベントライブラリ

# つかいかた
- EventSystem.hppインクルード。
- events::Delegateがデリゲート型、events::ManagedEventListenerを継承したクラスにイベントハンドラを持てる。
- デリゲートは+=演算子でハンドラ追加、-=演算子でハンドラ削除ができる
- デリゲートに追加するときはevents::EventHandler(クラスのポインタ[this], ハンドラ関数[bool MemberMethod(Event& e)])を使って追加
- events::Delegate >> クラスのポインタ でそのクラスのハンドラがまとめて削除される
- events::ManagedEventListener::UseDelegate(Delegate型)でデリゲートを登録すれば、クラス開放時にデリゲートからそのクラスのハンドラが削除される

コードを書けば
```cpp
#include "EventSystem/Include/EventSystem.hpp"

events::Delegate g_Delegate;

class TestListener : public events::ManagedEventListener
{
public:
    TestListener()
    {
        g_Delegate += events::EventHandler(this, &TestListener::TestHandler);
        g_Delegate += events::EventHandler(this, &TestListener::TestHandler2);
        UseDelegate(g_Delegate);
    }
    
    bool TestHandler(events::Event& e)
    {
        std::cout << e.name << "でTestHandlerがよばれた！！！" << std::endl;
        return true; // falseを返すとキャンセルされる(これ以降のハンドラが呼ばれない)
    }
    
    bool TestHandler2(events::Event& e)
    {
        std::cout << e.name << "でTestHandler2がよばれた！！！" << std::endl;
        return true;
    }
};

void main()
{
    auto l_Listener = new TestListener();
    
    events::Event e;
    e.name = "Test Event";
    g_Delegate(e);
    
    delete l_Listener;
    
    g_Delegate(e); // 何も呼ばれない
}
```

こんなかんじ。
