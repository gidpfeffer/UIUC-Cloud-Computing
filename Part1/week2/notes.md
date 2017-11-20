# Week 2

## Gossip

### Multicast Problem 

As opposed to broadcast, where you want to send information to everyone in the network, multicast only sends information to a group in the network.

You want it to be reliable, in spite of failures and delays that may happend. Additionally, you want the protocol to be scalable, and grow slowly as new nodes are added to the group. 

The centralized approach puts the responsibility on a single node to transmit the information to all other nodes. Though very simple, there are obvious downsides in terms of reliability and speed.

Tree based protocols have been developed to respond to the problems faced by the centralized approach. You need to set up and maintain the tree. If one of the nodes goes down, this could lead to propagation failures. Acknowledges and negative acknoledges can be used to repair the tree.

### The Protocol

Periodically, the sender picks **b** random targets and sends them copies of the "gossip" message. The constant **b** is generally around 2. Other nodes to the same, sending out **b** messages to random nodes. Technically, it gossip sends out more messages then previous protocol. This is a push gossip protocol. Pull protocols request instead of transmitting. And lastly, push-pull is a hybrid.

### Analysis

#### Push

The gossip converges in a factor of the logarithmic number of total nodes (n). The differential equations can be found in the slides. If half of the nodes fail, it can be shown that the time required to propogate all messages is still bounded by log (clog(n) -> 2clog(n)).

#### Pull

The first half of propogation still takes O(log(n)) though the second half takes O(log(log(n))). It infects at a very rapid rate once half of thepopulation is infected (is intuitive why).

### Topology Aware Gossiping

Gossips inside its own subnet with much higher probability than it would gossip outside of it's subnet. Reduces load on router to O(1) if you distribute the probability correctly. 

## Membership

Trying to solve crash stop or fail stop failures (process fails and does not recover).

### Group Membership Service

Membership lists are maintained of currently working processes. 

**Strongly Consistent** paradigm maintains that all processes maintain a complete list. 

**Almost Complete** lists (gossip-style or SWIM). Responsible for detecting failures and disseminating information (such as process joining and leaving).

### Gossip Sytle Failure Detection

Gossip style communicates updated heartbeat tables. The reciever merges the table with itself. Only the more recent entries than what it currently has is merged into the recievers table. It table entry is updated if and only if the heartbeat counter is higher for the data coming in. If a signal for a certain entry has not been recieved for a certain amount of time, it is marked as failing.

### SWIM

pj first pings pj. If pj responds, the stage is over. If not, pi pings k other nodes telling them to ping pj. Spatially and temporally, attempts are made to guarentee failure. As you increase k, the probability of mistake falls. Each member maintains a target list which it selects iits ping taget from at random. Follows geometric distribution in terms of number of calls until it checks for a specific node.

You can bound to O(n) by randomly permuting the list and traversing it in order. Piggyback information about recent failures on top of ping messages. Known as epidemic style dissemination. Most rocesses know about filures after log(n) timesteps.

INstead of using absolutes, can improve the algorithm by assigning a node to suspected before setting it to failing. The node or other nodes may tell you it is still alive, in which case you set it back. You use incranation numbers to maintain consistency while going back and forth between working and suspecting.

## Grids




