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

	TEST_CLASS(Test)
	{
	public:
        SelfAddListener* l_SelfAdd = nullptr;
        OthersAddListener* l_OthersAdd = nullptr;
		TEST_METHOD(SelfAddTest)
		{
            l_SelfAdd = new SelfAddListener();

            TestEvent e("Event_SelfAddTest");
            e.name = "TEST EVENT";
            g_TestHandle(e);

            delete l_SelfAdd;
		}

        TEST_METHOD(OthersAddTest)
        {
            l_OthersAdd = new OthersAddListener();

            g_TestHandle += events::EventHandler(l_OthersAdd, &OthersAddListener::TestHandler);
            g_TestHandle += events::EventHandler(l_OthersAdd, &OthersAddListener::TestHandler2);

            TestEvent e("Event_OthersAddTest");
            e.name = "TEST EVENT";
            g_TestHandle(e);

            delete l_OthersAdd;
        }

        TEST_METHOD(EmptyDelegateTest)
		{
            TestEvent e("Event_EmptyDelegateTest");
            e.name = "TEST EVENT";
            g_TestHandle(e);
		}
	};
}
