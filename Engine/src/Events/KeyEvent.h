#pragma once
#include "Event.h"

namespace Engine {
  class KeyEvent : public Event {
  public:
    int GetKeyCode() const {
      return m_KeyCode;
    }

    virtual int GetCategoryFlags() const override {
      return (EventCategory::Input | EventCategory::Keyboard);
    }

  protected:
    KeyEvent(int keycode): m_KeyCode(keycode) {
    }

    int m_KeyCode;
  };

  class KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(int keycode, int repeatCount): KeyEvent(keycode), m_RepeatCount(repeatCount) {
    }

    int GetRepeatCount() const {
      return m_RepeatCount;
    }

    static EventType GetStaticType() {
      return EventType::KeyPressed;
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "KeyPressed";
    }

    virtual std::string ToString() const override {
      std::stringstream ss;
      ss << "KeyPressedEvent: " << m_KeyCode << " [" << m_RepeatCount << " repeats]";
      return ss.str();
    }

  private:
    int m_RepeatCount;
  };

  class KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(int keycode): KeyEvent(keycode) {
    }

    static EventType GetStaticType() {
      return EventType::KeyReleased;
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "KeyReleased";
    }

    virtual std::string ToString() const override {
      std::stringstream ss;
      ss << "KeyReleasedEvent: " << m_KeyCode;
      return ss.str();
    }
  };
}  // namespace Engine