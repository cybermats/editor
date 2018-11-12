
#pragma once


enum class PositionState {
  KNOWN,
  UNKNOWN,
  END
};

struct Position {
  unsigned long line = 0;
  unsigned long column = 0;
  PositionState state = PositionState::KNOWN;

  bool operator<(const Position& pos) const {
    if (line < pos.line)
      return true;
    if (line > pos.line)
      return false;
    return column < pos.column;
  }
};
