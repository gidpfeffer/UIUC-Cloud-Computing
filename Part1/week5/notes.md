# Classical Algorithms

## Snapshots

Snapshots may alow for checking

* Checkpointing to restart failing systems
* Garbage collecting objects at servers without any pointers to them
* Termination of computing

Snapshots are resresented by a series of causal events, not timestamps.

### Model

There are N processes in a system. Between every process, there are two channels. Communication channels is first in first out. Assume that there are not faiures at processes. 

Assumes that snapshot calculation does not interfere with normal application operations. 

### Initiator Process

First, the initiator records its own state. Then, the initiator process creates special messages called "Marker" messages. These messages are sent to all other processes in the system using an outgoing channel. Then the ititator starts recieving incoming messages. It records the state from these messages.

When a process recieves a marker, and this is the first marker that the node is recieving, it stores a record of its state. It then marks the state of the channel it recieved the marker from as empty. Then it sends out markers to everyone it is connected to. Next time it recieves another marker from the same channel, it sends all the updated responses. The algorithm ends when all channels have had a marker pass through them.

Algorithm is causally correct.

## Multicast

## Paxos
