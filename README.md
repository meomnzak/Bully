# Bully Algorithm

Simulate the [Bully election algorithm](https://en.wikipedia.org/wiki/Bully_algorithm) using multiprocessing and shared memory communication technology.

## Getting Started

### Usage

* The exe file can be found in Task\cmake-build-debug\Task.exe
* The first command you can type is “start process” without quotes in order to start a new process.
* The second command is “shut id” without quotes where id is the id of the process you want to shut.
* The final command is “end” without quotes in order to shut all running processes and end the program.

## The logic

* I added a facilitator process which is running first and doesn’t participate in election it only helps processes. When the user type “start process” the facilitator makes a new process and create a shared memory between the facilitator and the new process. The information of the new processes are kept in a map in the facilitator process.

* The format of the message in the shared memory is:
  * isCoordinator|Time|makeComputation|arrSz|array
* The isCoordinator field is a boolean to know the coordinator. The time is a field that every process increments by one every second. The facilitator has a globalTime field which is only updated by the coordinator and every other process check its time against the one given by coordinator when the coordinator is down it will not update the time and this will start election.
* The winner of the election is decided in the facilitator since it has the id of all the processes and it will change the isCoordinator field in the new coordinator and make it true.
* The last three fields in the message are related to the bonus feature. The makeComputation is a boolean which is true when we want to make a distributed computation. The arrSz field is the size of the array. The array field has the numbers of the array comma separated.

## Logs example
 ```
start process
Process with id: 1 has started
Election has started
The available processes are : 1
The winner of the election is 1
The time of process with id 1 is 1
start process
Process with id: 2 has started
Election has started
The available processes are : 1 2
The winner of the election is 2
The time of process with id 1 is 14
The time of process with id 2 is 14
start process
Process with id: 3 has started
Election has started
The available processes are : 1 2 3
The winner of the election is 3
The time of process with id 1 is 22
The time of process with id 2 is 22
The time of process with id 3 is 23
shut 3
Process with id: 3 has been terminated
The time of process with id 1 is 27
The time of process with id 2 is 27
The coordinator is down
Election has started
The available processes are : 1 2
The winner of the election is 2
end
Process with id: 1 has been terminated
Process with id: 2 has been terminated

 ```
 
 ## Technical decisions and Trade-offs
 * I used C/C++ as they are considered the fastest.
 * I used shared memory for communication between different processes because it is the fastest mechanism and doesn’t need system calls for every data exchange.

## Possible future improvements
* I added a process which I called facilitator that make some of the work of the coordinator so I want to get rid of it in the future.
## References

* [Creating Named Shared Memory - Win32 apps | Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory?redirectedfrom=MSDN)
