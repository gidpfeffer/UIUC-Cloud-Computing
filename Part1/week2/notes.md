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

Gossips inside its own subnet with much higher probability than it would gossip outside of it's subnet.

## Membership

## Grids

