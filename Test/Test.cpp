#include <iostream>
#include "CppUnitTest.h"

#include "../EventSystem/Include/EventSystem.hpp"
#include "TestEvent.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
    events::Delegate g_TestHandle;

    class SelfAddListener : public events::ManagedEventListener
    {
    public:
        SelfAddListener()
        {
            g_TestHandle += events::EventHandler(this, &SelfAddListener::TestHandler);
            g_TestHandle += events::EventHandler(this, &SelfAddListener::TestHandler2);
            UseDelegate(g_TestHandle);
        }

        bool TestHandler(TestEvent& e)
        {
            Logger::WriteMessage(e.message_.c_str());
            Logger::WriteMessage(("Called SelfAddListener::TestHandler(TestEvent& e): " + e.name).c_str());
            return true;
        }
        bool TestHandler2(TestEvent& e)
        {
            Logger::WriteMessage(e.message_.c_str());
            Logger::WriteMessage(("Called SelfAddListener::TestHandler(TestEvent& e): " + e.name).c_str());
            return true;
        }
    };

    class OthersAddListener : public events::ManagedEventListener
    {
    public:
        OthersAddListener()
        {
            UseDelegate(g_TestHandle);
        }

        bool TestHandler(TestEvent& e)
        {
            Logger::WriteMessage(e.message_.c_str());
            Logger::WriteMessage(("Called OthersAddListener::TestHandler(TestEvent& e): " + e.name).c_str());

            return true;
        }
        bool TestHandler2(TestEvent& e)
        {
            Logger::WriteMessage(e.message_.c_str());
            Logger::WriteMessage(("Called OthersAddListener::TestHandler2(TestEvent& e): " + e.name).c_str());
            return true;
        }
    };

    class StandardListener : public events::ManagedEventListener
    {
    public:
        StandardListener()
        {
            UseDelegate(g_TestHandle);
        }

        bool TestHandler(TestEvent& e)
        {
            Logger::WriteMessage(e.message_.c_str());
            Logger::WriteMessage(("Called StandardListener::TestHandler(TestEvent& e): " + e.name).c_str());

            return true;
        }
        bool TestHandler2(TestEvent & e)
        {
            Logger::WriteMessage(e.message_.c_str());
            Logger::WriteMessage(("Called StandardListener::TestHandler2(TestEvent& e): " + e.name).c_str());
            Logger::WriteMessage("Cancelled");
            return false;
        }
        bool TestHandler3(TestEvent& e)
        {
            Logger::WriteMessage(e.message_.c_str());
            Logger::WriteMessage(("Called StandardListener::TestHandler3(TestEvent& e): " + e.name).c_str());
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
            g_TestHandle(e);

            delete l_SelfAdd;
		}

        TEST_METHOD(OthersAddTest)
        {
            auto l_OthersAdd = new OthersAddListener();

            g_TestHandle += events::EventHandler(l_OthersAdd, &OthersAddListener::TestHandler);
            g_TestHandle += events::EventHandler(l_OthersAdd, &OthersAddListener::TestHandler2);

            TestEvent e("Hello");
            e.name = "Event_OthersAddTest";
            g_TestHandle(e);

            delete l_OthersAdd;
        }

        TEST_METHOD(EmptyDelegateTest)
		{
            TestEvent e("Hello");
            e.name = "Event_EmptyDelegateTest";
            g_TestHandle(e);
		}

        TEST_METHOD(EventCancellableTest)
		{
            auto l_Std = new StandardListener();

            g_TestHandle += events::EventHandler(l_Std, &StandardListener::TestHandler);
            g_TestHandle += events::EventHandler(l_Std, &StandardListener::TestHandler2);
            g_TestHandle += events::EventHandler(l_Std, &StandardListener::TestHandler3);

            TestEvent e("Hello");
            e.name = "Event_EventCancelTest_Cancellable";
            e.isCancellable = true;

            g_TestHandle(e);

            delete l_Std;
		}

        TEST_METHOD(EventUnCancellableTest)
        {
            auto l_Std = new StandardListener();

            g_TestHandle += events::EventHandler(l_Std, &StandardListener::TestHandler);
            g_TestHandle += events::EventHandler(l_Std, &StandardListener::TestHandler2);
            g_TestHandle += events::EventHandler(l_Std, &StandardListener::TestHandler3);

            TestEvent e_uncancel("Hello");
            e_uncancel.name = "Event_EventCancelTest_UnCancellable";
            e_uncancel.isCancellable = false;

            g_TestHandle(e_uncancel);

            delete l_Std;
        }
	};
}
