#ifndef EVENT_H
#define EVENT_H

enum class EventType {
  NONE,
  WIN,
};

struct Event {
  EventType type = EventType::NONE;
};

#endif
