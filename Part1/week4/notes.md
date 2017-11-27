# Week 4

## Key Value Stores

### NoSQL

Stands for not only sequel. Generally stored by column not by row. Supports gets and puts (potentially some additional other calls). Generally doesn't contain foreign keys. Scales horizontally where as relational databases scale vertically.

### Cassandra

A distributed key value storage system maintained by Apache. 

#### Determination of Key Storage Node

Uses a circular distributed hash table like the one seen for chord. There is no routing (finger) table in Cassandra like there was in Chord. This means that every server needs to know information about all the other servers in the ring. Partition strategies may differ from that of Chord though. Servers can be referenced like Chord, using hash functions (called RandomPartitioner). They can also be partioned using ByteOrdering which makes range queries easier (ex. ordering based on timestamp of tweets). These are SimpleStrategies. For NetworkTopologyStrategies, you can assign multiple keys per data center.

Hinted Handoff writes locally for a down server until it comes back up. After it comes back up it writes the updated data there.

#### Snitching

Snitches map ip adresses to racks and DCs. There are a variety of inferring techniques and can be used/configured as needed.

#### Writing

Writes to files when memory capacity is met. Writes base don sorted string. Also uses an index table to quickly find elements without having to parse through the whole data file. Uses a Bloom filter to determine whether or not an item is present in the database. Also merges SSTables after a certain amount of time and deletes the unmerged files. Called compaction.

### X-Cap Theorem

### The COnsistency Spectrum

### HBase

## Time and Ordering

### Cristian's Algorithm

### NTP

### Lamport Timestamps

### Vector Clocks
