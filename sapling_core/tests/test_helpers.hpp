#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "sapling/sapling.hpp"

namespace sapling::testing {

// A fake leaf that returns a scripted sequence of statuses, one per tick,
// repeating the last one forever once the script runs out. It also counts how
// often it was ticked and halted, so tests can check what the parents did.
class ScriptedAction : public Node {
 public:
  ScriptedAction(std::string name, std::vector<Status> script)
      : Node(std::move(name)), script_(std::move(script)) {}

  int ticks = 0;
  int halts = 0;

 protected:
  Status onTick() override {
    Status s = script_.at(std::min<std::size_t>(ticks, script_.size() - 1));
    ++ticks;
    return s;
  }
  void onHalt() override { ++halts; }

 private:
  std::vector<Status> script_;
};

inline std::shared_ptr<ScriptedAction> scripted(std::string name, std::vector<Status> script) {
  return std::make_shared<ScriptedAction>(std::move(name), std::move(script));
}

constexpr Status S = Status::Success;
constexpr Status F = Status::Failure;
constexpr Status R = Status::Running;

}  // namespace sapling::testing
