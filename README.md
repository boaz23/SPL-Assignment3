Repository for assignment 3 in _SPL_ course.

This assignment was about creating an client application which communicated with other clients through a common server.
A client could create a room and join rooms. A client could also send messages within a room,
the message was only received by clients which had joined the room already.
The client was also responsible for almost the entire domain logic.  
Each client was written in C++ had 2 threads: one for user input and another for the client-server connection.

The server was just acting as a relay, it did not perform any domain logic besides that,
but it had to keep data structures for the rooms and the clients which were connected to them.
Naturally, the server was responsible for decoding, encoding and parsing messages.  
The server was written in Java.

Another challenge of the server was multithreading
The server used an architecture which the course staff called _Reactor_.
It used a fixed amount of threads, so each thread served multiple clients.
This architecture was already implemented for us by the staff, but we had to change it a bit.
We also had to be cautious when dealing with the above data structures (because of the multi-threading).

[Full assignment description](https://www.cs.bgu.ac.il/~spl201/index.php?page=Assignments.Assignment_3)
