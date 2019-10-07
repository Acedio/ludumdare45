#ifndef EVENT_H
#define EVENT_H

enum class EventType {
  NONE,
  WIN,
  DIE,
};

struct Event {
  EventType type = EventType::NONE;
};

#endif
