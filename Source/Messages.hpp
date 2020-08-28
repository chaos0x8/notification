#pragma once

struct Command {
  enum class Type : char { Stop = 's' };

  Type type;
};

struct Notification {
  uint32_t titleSize;
  uint32_t contentSize;
  uint32_t actionLabelSize;
  uint32_t actionSize;
};

struct Message {
  enum class Type : uint32_t { Command, Notification };

  Type type;

  union {
    Command command;
    Notification notification;
  };
};
