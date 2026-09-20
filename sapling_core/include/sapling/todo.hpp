#pragma once
#include <stdexcept>
#include <string>

// Every function you still have to write throws this. When a test fails with
// "Not implemented yet: ..." you know exactly which function to go write next.
// Delete the SAPLING_TODO line once you've implemented the function.
#define SAPLING_TODO(what) \
  throw std::logic_error(std::string("Not implemented yet: ") + (what))
