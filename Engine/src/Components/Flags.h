#pragma once
#include <pch.h>
#include <Utility/Helpers.h>
#include <Utility/Utility.h>

namespace Engine {
  BETTER_ENUM(__TransformFlags, uint32_t, Dirty = Helpers::Bit32(0),
              Updated = Helpers::Bit32(1));
  typedef __TransformFlags TransformFlags;
  typedef Utility::BitFlags< TransformFlags > TransformBitFlags;
}