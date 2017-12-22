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

### Consistent Cuts

A cut is a time frontier at each process and and each channel. 

Consistent cuts is a cut that obeys causality. This means that if f is in the cut and and e happens before f, the e is also in the cut. The Chandy Lamport algorithm always creastes consistent cuts.

### Safety and Liveness

Safety: A guarentee that something bad will never happen. For example a peace treaty has the disired property that war will not be waged between them.

Liveness: A guarantee that something good will happen eventually. No bounds are specified though. For example, the termination of a process may be a liveness property.

It is very hard to guarantee both properties. 

All stable properties can be detected using the Chandy Lamport algorithm. A property is stable if its value doesn't change after being defined.

## Multicast

Multicast is a message that needs to be sent out to a selected group of processes. A broadcast is a message sent to all processes. A unicast is a message sent from one sender to one reciever. 

### Ordering

You want all multicasts to be recieved in causal ordering between nodes.

FIFO: makes sure that messages from certain nodes are recieved in order, but can reviece messages form different nodes in varying order.

Causal: In this ordering, multicasts with messages that are causally related must be recieved in the same causality obeying order at all the recievers. Concurrent messages can be recieved in varying orders. Causal implies that FIFO is satisfied. FIFO does not imply causality.

Total Ordering (Atmoic Broadcast): Guarentees that all recievers recieve the messages in the same order (doesn't worry about the order in which they were sent).

## Paxos
