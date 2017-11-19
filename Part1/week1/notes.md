# Week 1 Notes

## Intro To Clouds

### Definitions

**Cloud** - a Lot of storage resources with compute cycles located nearby.

**Single Site Cloud (Datacenter)** -
  * Compute nodes(grouped into racks)
  * Switches, connecting the racks
  * A network topology
  * Storage nodes connected to servers
  * Software services
 
**Geographically Distributed** -
  * Multiple single sites
  * generally sites have similar software stacks

### Cloud Computing Industry Leaders

  * Amazon AWS
  * Microsoft Azure
  * Google Engine
  
### Timeline
  
   1. First data centers (ENIAC, ORDVAC, ILLIAC)
   2. Data processing (Honeywell, IBM)
   3. Grid Network (Gryphn)
   4. Peer to peer (bit torrent)
   5. Cloud
  
  ### New Features In Cloud
  
  * Massive Scale
  * On demand access
  * Data intensive nature
  * New cloud computing paradigms (MapReduce, Hadoop, NoSQL, Cassandra, MongoDB)
  
  ## Distributed Systems
  
  ### Definitions
  
  **Operating Systems** - Provides a user interface to hardware, allocates storage, and manages computational resources.
  
  **Distributed Systems** - A collection of entities, each of which is autonomous, programmable, asynchronous and failure prone, and which communicate through an unreliable communication medium.
  
  ## Map Reduce
  
  ### File Systems
  
 File systems are needed to communicate the results from input to map, map to reduce, and from reduce back to the user. Generally done on the same machine to increase speed. 
  
  Google uses Google File System (GFS). Hadoop uses Hadoop Distributed File System (HDFS).
  
  Hadoop Resource Manager assigns tasks to servers (both map and reduce).
  
  Mappers write locally and remotely pass the information to the reducers.
  
  Mappers generally try to access data already on that server, if not then rack, and so on.
    
