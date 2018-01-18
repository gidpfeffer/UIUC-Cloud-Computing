# Week 2

## Concurrency Control

### RPC's

Remote Procedure Calls (RPCs) allow processes to call functions in other processes. Local procedure calls (LPCs) have both a caller and a callee function. In an RPC, the cller and callee functions reside in different processes.

You need to access objects by global references since you can't use pointers between processes. For example, object = Port + ip + object number.

Failures make different cases hard to distinguish. The typical semantics used are

* At most once
* At least once
* Maybe, best effort

Idempotent operations can be repeated and end result will be the same. Code for middleware components are generated automatically. You only have to write the caller and callee functions.

### Transactions

A transaction is a series of opertations performed between the user and the server using RPC's. Transactions either complete and commit, or fail and have no effect on the server.

Atomictity: all or nothing principle.

Isolation: transactions cannot get access to intermediate state or other transaction states.

**A**tomicity: see above
**C**onsistency: if the server starts in a consistent state, it ends in one
**I**solation: see above
**D**urability: after a transaction is complete, its effects stay.

### Serial Equivalence

An interleaving O is serially equivalent if another interleaving O' gives the same end result.

Two operations are said to be conflicting operations if their order matters. For example, read(x) then write(x) vs write(x) then read(x). Read(x) then read(x) are not conflicting. 

Two interleavings are serailly equivalent if and only if all conflicting operations appear in the same relative ordering between them.

One such solution is to check for serial equivalence with other overlapping processes before commiting. If it is met, you commit. Otherwise, abort.

### Pessimistic Concurrency

ONe example would be locking and unlocking. A downside is that processes which can be run concurrently (for example reads), are locked by mutual exclusion.

Read-write locks address the above concern by allowing concurrent reads until a write is sent through.

How do you ensure that serial equivalence is obtained? Two-phase locking allows for this. 

Phase 1, Growing Phase: locks are only obtained.
Phase 2, Shrinking Phase: locks are only released.

Potential problem: deadlocks. One possible solution is to use timeouts. This can be expensive if timeouts occur often. Another approach is deadlock detection. You could use the Chandy Lamport algorithm (global snapshot), and detect wait-for loops.

Combatting deadlocks: 

* Allow reads only
* Allow timeouts
* Access all locks, if any fail, then abort

### Optimimstic Concurrency Control

It is preferrable to use optimistic control when conflicts are expected to be rare. Increases operations per second and decreases latency.

One simple implementation is to check for serial equivalence when the commit is sent. If serial equivalence is not obtained, rollback the updates. This can cause cascading rollbacks if other updates were dependent on the rolled back updates.

Second Approach - Timestamp Ordering: assign each transaction an id which will be used to determine its position in serialization ordering. Ensure that for a transaction both are true:

1. T writes to O if and only if transations that had read or written O had lower id that T.
2. T's read to object O is allowed only if O was last written by a transaction with a lower id than T.

If rule is violated, abort.

Third Approach - Multi-Version Concurrency Control: Both a tentative and a commited version are maintained. Each tentative version has a timestamp. ON a read or write, find the "right" timestamp based on timestamps.

## Replication Control

### Replication

Replication control deals with having multiple servers, where objects are stored accross multiple machines.

Replication is important for fault tolerance. It also allows for load balancing. Reads and writes can be sent to different servers where replicas live based on traffic to specific servers.

**Replication Transparency** says that the client should not be aware of the multiple copies.

**Replication Consistency** says that multiple clients should see a single consistent copy of data, despite replication.

Passive replication uses an active or master copy. Active treats all replicas identically. Both approaches use the concept of "Replicated State Machine".

One copy serializability refers to serial equivalence and for replicated servers.

### Two Phase Commit

A consensus algorithm is run to make sure that either all servers commit or none of them do. This leads to a series of failure related problems. 

The two phase commit algorithm. The coordinator first sends a prepare message from a cordinator. The servers then save data to a log. If everything is ok, the servers responds with a YES vote. Otherwise, it sends a NO vote. If the coordinate recieves all yes responses, it sends a commit message, otherwise an abort message. Timeouts are also used to abort.
