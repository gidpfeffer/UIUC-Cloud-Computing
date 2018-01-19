# Week 4

## Distributed File Systems

Unix file system uses the notion of file descriptors. Each process needs to obtain a file descriptor before it is able to read or write to that file. It also needs to close the file desciptor when it is complete.

There are also read and write calls which are implemented using a read write pointer. Read reads the data into a buffer. Write writes the data from the buffer into the file. A command called lseek moves the read write pointer to a particular offset. Thsi may be an abolute offset, or a relative offset.

Linking can be used to have one file point to another. Hardlinking increments the reference count of a file, symbollic linking does not. You can call unlink to dereference the file pointers. This decrements the reference count. If the reference count goes to 0, the file is deleted.

In a distributed file system, you want the API's to mirror that of a local file system. The user should be able to perform operations on the file as if it were local. 

Security in DFS: First a user needs to be authenticated. Then they need to be authorized with respect to particular operations on the file system (whether or not they are allowed to read/write certain documents for example).

Access control lists are attached to documents and contain who can read/write them.

Capability lists are attached to users and contain the documents that they can read/write.

Flat file services store files independent of their heirarchy at server. 

Directory services are also at the server. This is used to comunicate with the client. 

Client service islocated at the client and talks to both the flat file service and the directory service.

Flat file service provides a read(id, absolute position, num_bytes). <- idempotent (can be repeated without side effects).

### NFS

The client makes a call to a virtual file system. The vitrual file system checks if the file exists locally. If it does, it calls APIs on the local file system. Otherwise, it makes a RPC to the remote file system.

NFS client system is integrated with the kernel. IT performs RPCs to the distributed file system whenever remote calls are needed. the NFS server system plays the role of flat file as well as the directory service from the example int he previous section. Allows mounting of directories and files. Mounting doesn't copy files. It just makes a local pointer to remote files.

The virtual file system module allows processes to access files via file descriptors. This allows for transparency. The v-node data srtucture stores the information about the files including where they can be access from.

Server caching is one of the big reasons NFS is so fast with reads.

### AFS

Uses two unusual design principles. Whole file caching and serving. Is fast as a result of the fact that many files are only accessed by one person and those files are generally small in size.

Provides a callback promise which ensures that is another user opens, edits, then closes a file, you will be told about the update.

## Distributed Shared Memory

A version of multiprocessing file system except now it is over a network. A cache is used to store a collection of pages that haver recently been accessed at a process. When there is a cache hit, the page is read from memory. When there is a cache miss, the page has to be accessed over the network. Usually this is accomplished by ulticasting to the other processes in the DSM network.

Each page has an owner with the most recent verion of the page. Each page is also either in read or write state. When a page is in R mode, many processes can also be in R mode. When a process is in W mode, no othe rprocess can be in W or R mode. Only the owner has a copy.

An invalidate approach can be used to guarentee that only one process is ever writing. This can lead to flip flopping where two processes that both want to write keep stealing ownership of the write permission. Each time, the file needs to be transferred over the network and a lot of overhead is incurred. Setting the page size is a very important concern since it affects the rate of necessary tranfer.

An alternative approach uses an update API. On a write, you send updates to other processes that also have the page. This is generally not as good of an approach as the anvalidate approach. 

## Sensor Networks
