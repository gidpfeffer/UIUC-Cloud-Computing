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

### Optimimstic Concurrency Control

## Replication Control

### Replication

### Two Phase Commit
