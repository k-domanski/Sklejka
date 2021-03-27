#include "Test.h"
#include "irrKlang.h"
#include "Application/Log.h"

int main()
{
    Engine::Log::Init();
    int a = 3;
    CORE_TRACE("Test macro var = {0}.", a);
    CORE_DEBUG("Test macro var = {0}.", a);
    CORE_INFO("Test macro var = {0}.", a);
    CORE_WARN("Test macro var = {0}.", a);
    CORE_ERROR("Test macro var = {0}.", a);
    CORE_FATAL("Test macro var = {0}.", a);

    APP_TRACE("Test macro var = {0}.", a);
    APP_DEBUG("Test macro var = {0}.", a);
    APP_INFO("Test macro var = {0}.", a);
    APP_WARN("Test macro var = {0}.", a);
    APP_ERROR("Test macro var = {0}.", a);
    APP_FATAL("Test macro var = {0}.", a);
	Sklejka::TestWindow();
}