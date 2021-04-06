#pragma once
#include "pch.h"

namespace Engine {
  BETTER_ENUM(__EventType, int, None = 0, WindowClose, WindowResize, KeyPressed, KeyReleased,
              MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled);
  typedef __EventType EventType;

  BETTER_ENUM(__EventCategory, int, None = 0, Application = 1 << 0, Input = 1 << 1,
              Keyboard = 1 << 2, Mouse = 1 << 3, MouseButton = 1 << 4);
  typedef __EventCategory EventCategory;

  class Event {
    friend class EventDispatcher;

  public:
    virtual ~Event()                       = default;
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const    = 0;
    virtual int GetCategoryFlags() const   = 0;
    virtual std::string ToString() const {
      return GetName();
    }

    bool IsInCategory(EventCategory category) {
      return GetCategoryFlags() & category;
    }

  protected:
    bool m_Handled = false;
  };

  class EventDispatcher {
    template< typename T >
    using EventFn = std::function< bool(T&) >;

  public:
    EventDispatcher(Event& event): m_Event(event) {
    }

    template< typename T >
    bool Dispatch(EventFn< T > fun) {
      if (m_Event.GetEventType == T::GetStaticType()) {
        m_Event.m_Handled = fun(*(T*)&m_Event);
        return true;
      }
      return false;
    }

  private:
    Event& m_Event;
  };

  inline std::ostream& operator<<(std::ostream& os, const Event& event) {
    return os << event.ToString();
  }
}  // namespace Engine