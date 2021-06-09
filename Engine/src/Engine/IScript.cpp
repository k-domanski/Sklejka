#include "pch.h"
#include "IScript.h"

auto Engine::GetNextScriptTypeID() -> int {
  static int id = 1;
  return id++;
}
