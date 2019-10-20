#include <iostream>
#include "CppUnitTest.h"

#include "../Include/EventSystem.hpp"
#include "TestEvent.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
    delegate::Delegate<bool(TestEvent*)> g_TestHandle;

    class SelfAddListener
    {
    public:
        SelfAddListener()
        {
            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(this, &SelfAddListener::TestHandler);
            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(this, &SelfAddListener::TestHandler2);
        }

        ~SelfAddListener()
        {
            g_TestHandle -= this;
        }

        bool TestHandler(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called SelfAddListener::TestHandler(TestEvent& e): " + e->name).c_str());
            return true;
        }
        bool TestHandler2(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called SelfAddListener::TestHandler(TestEvent& e): " + e->name).c_str());
            return true;
        }
    };

    class OthersAddListener
    {
    private:
        std::string TestString_ = "";

    public:
        OthersAddListener(const std::string& moduleName)
        {
            TestString_ = moduleName;
            //UseDelegate(g_TestHandle);
        }

        ~OthersAddListener()
        {
        }

        bool TestHandler(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called OthersAddListener::TestHandler(TestEvent& e): " + e->name).c_str());
            Logger::WriteMessage(TestString_.c_str());

            return true;
        }
        bool TestHandler2(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called OthersAddListener::TestHandler2(TestEvent& e): " + e->name).c_str());
            Logger::WriteMessage(TestString_.c_str());
            return true;
        }
    };

    class StandardListener
    {
    private:
        std::shared_ptr<OthersAddListener> others = nullptr;

    public:

        StandardListener()
        {
            others = std::make_shared<OthersAddListener>("StandardListener");
            //UseDelegate(g_TestHandle);
        }

        ~StandardListener()
        {
        }

        bool TestHandler(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called StandardListener::TestHandler(TestEvent& e): " + e->name).c_str());

            return true;
        }
        bool TestHandler2(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called StandardListener::TestHandler2(TestEvent& e): " + e->name).c_str());
            Logger::WriteMessage("Cancelled");
            return false;
        }
        bool TestHandler3(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called StandardListener::TestHandler3(TestEvent& e): " + e->name).c_str());
            return true;
        }

        bool CallOtherHandler(TestEvent* e)
        {
            Logger::WriteMessage(e->message_.c_str());
            Logger::WriteMessage(("Called StandardListener::CallOtherHandler(TestEvent& e): " + e->name).c_str());

            Logger::WriteMessage("  Call others...");

            TestEvent evt_("Called from other class");
            evt_.name = "TEST_EVENT";
            others->TestHandler(&evt_);
            return true;
        }

        bool NoLogHandler(TestEvent* e)
        {
            return true;
        }
    };

	TEST_CLASS(Test)
	{
	public:
		TEST_METHOD(SelfAddTest)
		{
            auto l_SelfAdd = new SelfAddListener();

            TestEvent e("Hello");
            e.name = "Event_SelfAddTest";
            g_TestHandle(&e);

            delete l_SelfAdd;
		}

        TEST_METHOD(OthersAddTest)
        {
            auto l_OthersAdd = new OthersAddListener("OthersAddTest");

            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_OthersAdd, &OthersAddListener::TestHandler);
            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_OthersAdd, &OthersAddListener::TestHandler2);

            TestEvent e("Hello");
            e.name = "Event_OthersAddTest";
            g_TestHandle(&e);

            g_TestHandle -= l_OthersAdd;

            delete l_OthersAdd;
        }

        TEST_METHOD(EmptyDelegateTest)
		{
            TestEvent e("Hello");
            e.name = "Event_EmptyDelegateTest";
            g_TestHandle(&e);
		}

        TEST_METHOD(OtherMethodCallTest)
		{
            auto l_Std = new StandardListener();

            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::CallOtherHandler);

            TestEvent e("Hello");
            e.name = "Event_OtherMethodCallTest";

            g_TestHandle(&e);

            g_TestHandle -= l_Std;

            delete l_Std;
        }

        TEST_METHOD(ManyHandlerCallTest)
		{
            auto l_Std = new StandardListener();

            for (int i = 0; i < 10000; ++i)
            {
                g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::NoLogHandler);
            }

            TestEvent e("Hello");
            e.name = "Event_ManyHandlerCallTest";

            g_TestHandle(&e);

            g_TestHandle -= l_Std;

            delete l_Std;
		}

        TEST_METHOD(HandlerRemoveTest)
		{
            auto l_Std = new StandardListener();

            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::TestHandler);
            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::TestHandler2);
            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::TestHandler3);

            TestEvent e("Hello");
            e.name = "Event_HandlerRemove";

            g_TestHandle(&e);

            g_TestHandle -= delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::TestHandler2);

            g_TestHandle(&e);

            g_TestHandle -= l_Std;

            delete l_Std;
		}

        TEST_METHOD(StringCastTest)
		{
            auto l_Std = new StandardListener();

            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::TestHandler);
            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::TestHandler2);
            g_TestHandle += delegate::EventHandler<bool(TestEvent*)>(l_Std, &StandardListener::TestHandler3);

            Logger::WriteMessage(("g_TestHandle: " + std::string(g_TestHandle)).c_str());

            TestEvent e("Hello");
            e.name = "Event_HandlerRemove";

            g_TestHandle(&e);

            delete l_Std;
		}
	};
}
