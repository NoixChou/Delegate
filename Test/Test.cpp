#include <iostream>
#include "CppUnitTest.h"

#include "../Include/EventSystem.hpp"
#include "TestEvent.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
    events::Delegate<bool, TestEvent*> g_TestHandle([](bool ret, TestEvent* e) { return !ret && e->isCancellable; });

    class SelfAddListener : public events::ManagedEventListener
    {
    public:
        SelfAddListener()
        {
            g_TestHandle += events::EventHandler<bool, TestEvent*>(this, &SelfAddListener::TestHandler);
            g_TestHandle += events::EventHandler<bool, TestEvent*>(this, &SelfAddListener::TestHandler2);
        }

        ~SelfAddListener()
        {
            g_TestHandle >> this;
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

    class OthersAddListener : public events::ManagedEventListener
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

    class StandardListener : public events::ManagedEventListener
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

            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_OthersAdd, &OthersAddListener::TestHandler);
            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_OthersAdd, &OthersAddListener::TestHandler2);

            TestEvent e("Hello");
            e.name = "Event_OthersAddTest";
            g_TestHandle(&e);

            g_TestHandle >> l_OthersAdd;

            delete l_OthersAdd;
        }

        TEST_METHOD(EmptyDelegateTest)
		{
            TestEvent e("Hello");
            e.name = "Event_EmptyDelegateTest";
            g_TestHandle(&e);
		}

        TEST_METHOD(EventCancellableTest)
		{
            auto l_Std = new StandardListener();

            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_Std, &StandardListener::TestHandler);
            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_Std, &StandardListener::TestHandler2);
            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_Std, &StandardListener::TestHandler3);

            TestEvent e("Hello");
            e.name = "Event_EventCancelTest_Cancellable";
            e.isCancellable = true;

            g_TestHandle(&e);

            g_TestHandle >> l_Std;

            delete l_Std;
		}

        TEST_METHOD(EventUnCancellableTest)
        {
            auto l_Std = new StandardListener();

            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_Std, &StandardListener::TestHandler);
            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_Std, &StandardListener::TestHandler2);
            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_Std, &StandardListener::TestHandler3);

            TestEvent e_uncancel("Hello");
            e_uncancel.name = "Event_EventCancelTest_UnCancellable";
            e_uncancel.isCancellable = false;

            g_TestHandle(&e_uncancel);

            g_TestHandle >> l_Std;

            delete l_Std;
        }

        TEST_METHOD(OtherMethodCallTest)
		{
            auto l_Std = std::make_shared<StandardListener>();

            g_TestHandle += events::EventHandler<bool, TestEvent*>(l_Std.get(), &StandardListener::CallOtherHandler);

            TestEvent e("Hello");
            e.name = "Event_OtherMethodCallTest";
            e.isCancellable = false;

            g_TestHandle(&e);

            g_TestHandle >> l_Std.get();
        }
	};
}
