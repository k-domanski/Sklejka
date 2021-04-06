#pragma once
#include "Event.h"

namespace Engine {
  class MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(float mouseX, float mouseY): m_MouseX(mouseX), m_MouseY(mouseY) {
    }

    static EventType GetStaticType() {
      return EventType::MouseMoved;
    }
    float GetX() const {
      return m_MouseX;
    }
    float GetY() const {
      return m_MouseY;
    }

    virtual int GetCategoryFlags() const override {
      return (EventCategory::Input | EventCategory::Mouse);
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "MouseMoved";
    }

    virtual std::string ToString() const override {
      std::stringstream ss;
      ss << "MouseMovedEvent: " << m_MouseX << " / " << m_MouseY;
      return ss.str();
    }

  private:
    float m_MouseX, m_MouseY;
  };

  class MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(float xOffset, float yOffset): m_XOffset(xOffset), m_YOffset(yOffset) {
    }

    static EventType GetStaticType() {
      return EventType::MouseScrolled;
    }
    float GetXOffset() const {
      return m_XOffset;
    }
    float GetYOffset() const {
      return m_YOffset;
    }

    virtual int GetCategoryFlags() const override {
      return (EventCategory::Input | EventCategory::Mouse);
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "MouseScrolled";
    }

    virtual std::string ToString() const override {
      std::stringstream ss;
      ss << "MouseScrolledEvent: " << m_XOffset << " / " << m_YOffset;
      return ss.str();
    }

  private:
    float m_XOffset, m_YOffset;
  };

  class MouseButtonEvent : public Event {
  public:
    int GetMouseButton() const {
      return m_Button;
    }

    virtual int GetCategoryFlags() const override {
      return (EventCategory::Input | EventCategory::Mouse | EventCategory::MouseButton);
    }

  protected:
    MouseButtonEvent(int button): m_Button(button) {
    }

    int m_Button;
  };

  class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(int button): MouseButtonEvent(button) {
    }

    static EventType GetStaticType() {
      return EventType::MouseButtonPressed;
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "MouseButtonPressed";
    }

    virtual std::string ToString() const override {
      std::stringstream ss;
      ss << "MousePressedEvent: " << m_Button;
      return ss.str();
    }
  };

  class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(int button): MouseButtonEvent(button) {
    }

    static EventType GetStaticType() {
      return EventType::MouseButtonReleased;
    }

    virtual EventType GetEventType() const override {
      return GetStaticType();
    }

    virtual const char* GetName() const override {
      return "MouseButtonReleased";
    }

    virtual std::string ToString() const override {
      std::stringstream ss;
      ss << "MouseReleasedEvent: " << m_Button;
      return ss.str();
    }
  };
}  // namespace Engine