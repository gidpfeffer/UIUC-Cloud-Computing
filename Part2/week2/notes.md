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

### Serial Equivalence

### Pessimistic Concurrency

### Optimimstic Concurrency Control

## Replication Control

### Replication

### Two Phase Commit
