# Week 3

## Lesson 1

### Stream Processing in Storm

Stream processing is used to allow for high throughput with low latency. Takes in high amounts of data and processes it as it arrives (not in batches).

Spouts create the data (web crawler or database reader for example). Bolts process that data using filters, joins, etc. Grouping strategies are used to decide how the bolts should be paralellized.

## Lesson 2

### Distributed Graph Processing

An iteration: A node queries its neighbors for values and updates it own values.

Iterations are repeated until convergence of values or for a fixed number of iterations.

At each iteration, a server performs a gather-apply-scatter paradigm. Scatter is comprised of sending your updated values to your neighbors.

## Lesson 3

### Structure of Networks

Path length: the shortest path between any two vertices.

Clustering Coefficient: If A is connected to C and B is connected to C, what is the probability that A is connected to B?

Small world networks have high clustering coefficients and short path lengths.

Power law graphs decrease their probability of a certain degree logarithmically.

## Lesson 4

### Single-processir Scheduling

Most basic is FIFO ordering on tasks. Average completion time is equal to the sum of the completion times divided by number of tasks.

The next scheduler is shortest task first. It again maintans a data strucutre to order the processes. Thsi time, it prioritizes that shorter tasks go first.

Round robin algorithm breaks the processes into components. It then runs portions of the processes until completion.

### Hadoop Scheduling

Capacity Scheduler: Contains multiple queues, each of which contains multiple jobs. Each queue is guarenteed some portion of the computing resources. Soft limits can be used to specify minimum percentages of resources. Hard limits specify max's.

Hadoop Fair Scheduler: All jobs get equal share of resources. if there is one ask, it gets everything, two they both get half, and so on. May pre empt jobs by killing process and restarting them later. it always picks the most recently started tasks to kill to minimize the amount of waste.

### Dominant Resource Fair Scheduling

DRF is fair (shares resources evenly), strategy proof (tenants cannot benefit from lying), and is envy free (tenants cannot envy another tenants allocations). Used in Mesos, an OS designed for cloud applications.

Dominant resource is eaither RAM or CPU usage. Ehichever uses a higher proportion of the total resources on the node. The total percentage of dominant resources that every task gets is equivalent. Generalizes to other resource requirements. Disk, network, etc.
