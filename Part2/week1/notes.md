# Week 1

## Leader Election

### Problem

A leader may be needed to control replicas across multiple servers for example. Problems may arise when a leader fails, servers disagree about who the leader is, etc.

When leaders are elected, every server that is part of the group needs to be informed about this decision.

Any process may be able to detect that the leader has failed and therefor any server should be able to call for a leader election. Multiple processes can call for a new election, but only one leader should be elected at a time.

New leaders are generally picked base don an attribute (ip adress, available memory, etc). Elections should ensure safety and liveness.

Safety: The new leader elected is not faulty.
Liveness: The new leader is selected for every run (the election process terminates).
