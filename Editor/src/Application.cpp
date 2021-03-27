#include "Test.h"
#include "irrKlang.h"
#include "App/Log.h"

int main() {
  Engine::Log::Init();
  int a = 3;

  APP_TRACE("Test macro TRACE var = {0}.", a);
  APP_DEBUG("Test macro DEBUG var = {0}.", a);
  APP_INFO("Test macro INFO var = {0}.", a);
  APP_WARN("Test macro WARN var = {0}.", a);
  APP_ERROR("Test macro ERROR var = {0}.", a);
  APP_FATAL("Test macro FATAL var = {0}.", a);
  Sklejka::TestWindow();
}