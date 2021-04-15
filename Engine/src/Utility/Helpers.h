#pragma once
#include <pch.h>
#include <cstdint>

namespace Engine::Helpers {
  /* String helpers */
  auto FindToken(const std::string& str, const std::string& token, std::size_t offset = 0)
      -> std::size_t;
  auto FindTokens(const std::string& str, const std::string& token) -> std::vector< std::size_t >;
  auto GetNextWord(const std::string& str, std::size_t pos, const char separator = ' ')
      -> std::string;
  auto GetLineFromPosition(const std::string& str, std::size_t pos) -> std::size_t;
  auto GetLineBeginning(const std::string& str, std::size_t line) -> std::size_t;
  auto GetLineEnd(const std::string& str, std::size_t line) -> std::size_t;
  auto GetParentFolderPath(const std::string& str) -> std::string;
  auto RemoveAllComments(std::string str, const std::string& commentToken) -> std::string;

  /* Bit helpers */
  template< std::size_t bit_count >
  struct _bit_container {};
  template<>
  struct _bit_container< 8 > {
    typedef uint8_t container_t;
  };
  template<>
  struct _bit_container< 16 > {
    typedef uint16_t container_t;
  };
  template<>
  struct _bit_container< 32 > {
    typedef uint32_t container_t;
  };
  template<>
  struct _bit_container< 64 > {
    typedef uint64_t container_t;
  };
  template< std::size_t bit_count >
  constexpr auto Bit(uint32_t shift) -> typename _bit_container< bit_count >::container_t {
    return _bit_container< bit_count >::container_t(1) << shift;
  }
  constexpr auto Bit8(uint32_t shift) -> uint8_t {
    return uint8_t(1) << shift;
  }
  constexpr auto Bit16(uint32_t shift) -> uint16_t {
    return uint16_t(1) << shift;
  }
  constexpr auto Bit32(uint32_t shift) -> uint32_t {
    return uint32_t(1) << shift;
  }
  constexpr auto Bit64(uint32_t shift) -> uint64_t {
    return uint64_t(1) << shift;
  }
}  // namespace Engine::Helpers