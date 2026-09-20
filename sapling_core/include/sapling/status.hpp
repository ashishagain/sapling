#pragma once
#include <ostream>

namespace sapling {

// The four states a node can be in.
//   Idle    - not ticked yet, or reset after being halted / after finishing.
//   Running - the node needs more ticks to finish (e.g. the robot is still driving).
//   Success - finished, goal achieved.
//   Failure - finished, goal not achieved.
enum class Status { Idle, Running, Success, Failure };

inline const char* toString(Status s) {
  switch (s) {
    case Status::Idle: return "Idle";
    case Status::Running: return "Running";
    case Status::Success: return "Success";
    case Status::Failure: return "Failure";
  }
  return "Unknown";
}

inline bool isCompleted(Status s) { return s == Status::Success || s == Status::Failure; }

inline std::ostream& operator<<(std::ostream& os, Status s) { return os << toString(s); }

}  // namespace sapling
