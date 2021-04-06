#pragma once
#include "Event.h"

namespace Engine {
  class WindowResizeEvent : public Event {
  public:
    WindowResizeEvent(int width, int height): m_Width(width), m_Height(height) {
    }

    static EventType GetStaticType() {
      return EventType::WindowResize;
    }
    int GetWidth() const {
      return m_Width;
    }
    int GetHeight() const {
      return m_Height;
    }

    virtual int GetCategoryFlags() const override {
      return EventCategory::Application;
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "WindowResize";
    }

    virtual std::string ToString() const override {
      std::stringstream ss;
      ss << "WindowResizeEvent: " << m_Width << " / " << m_Height;
      return ss.str();
    }

  private:
    int m_Width, m_Height;
  };

  class WindowCloseEvent : public Event {
  public:
    WindowCloseEvent() = default;

    static EventType GetStaticType() {
      return EventType::WindowClose;
    }

    virtual int GetCategoryFlags() const override {
      return EventCategory::Application;
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "WindowClose";
    }
  };
}  // namespace Engine