# Design notes

Fill this in as you go. Writing down *why* is what turns a tutorial into a portfolio project,
and it's what interviewers ask about.

## Tick semantics
<!-- How does a tick propagate? What does Running mean for the parent? -->

## Halting
<!-- Why is halt() needed at all? Why only call onHalt() on Running nodes?
     What goes wrong in the turtle demo without MoveTo::onHalt()? -->

## Memory vs. reactive control nodes
<!-- When do you use Sequence vs ReactiveSequence? Give robot examples. -->

## The blackboard
<!-- Why std::any? What are the downsides of a global string-keyed store?
     How do BehaviorTree.CPP ports address this? -->

## Separating behaviour from ROS
<!-- Why are the turtle leaves ROS-free? How does that help testing? -->

## The patrol tree
<!-- Answer the three questions from sapling_turtle/include/sapling_turtle/behaviors.hpp -->

## Comparison with BehaviorTree.CPP
<!-- What does it have that sapling doesn't? (XML, ports, Groot, coroutines, ...) -->
