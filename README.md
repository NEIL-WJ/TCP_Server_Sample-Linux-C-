# **TCP_Server_Sample-Linux-C-[^1]**
    Just a small TCP_Server sample for myself and other starters. 
---
## **Index**
* `Upcoming version`<br>
    * Get client IP
      * Struck on initialize ***structur in (struture \*)***
    * Add queue
    * UDP Server
* `Latest version`[^2]<br>
    * Function & Structure
      >* Socket
      >* Bind
      >* Listen
      >* Threads{
        >>* Loop{
          >>>* Client check
          >>>* Select{
            >>>>* Recv
            >>>>* Display
      >* Loop{
        >>* Accept
    * Behavior
      * Send&Recv client message
      * Main: blocking mode, block at *accept*
      * Threads: Recv and process client message while main is waiting to accept new clients
        * Blocking mode (with time limit): Hang-up at *select* for seconds and release if no message is received.
    * Comment:
      * Blocking mode can repidly decrease cpu usage. (Otherwise, it will keep requesting like a spinlock)
      * Threads is detached from main to simplify work, but may cause problem in actual work. (e.g., threads will be killed when main is finished, regardless of threads process.
      * Since recv message will be blocked by *select*, client_socket is set to non-blocking mode by *accept4*
      * Using array to transmit all client_socket into one threads to decrease the amount of threads. Create one sperate threads for each client_socket should also work.
      * Select is a good way to hang-up threads which is more accuracy and seems more safe.
      * Parameter in *Select* need to be initialize every time before use to refresh target socket (Recommanded by Microsoft document). 
        * After initialize, the default set is,
          * Hang-up time: Set to *NONE* (Block with no time limit)
          * Target socket: Won't trigger any socket.
        * Even *array->client_socket[num]* is used (so it should monitor target address), *select* won't trigger new socket if *array->client_socket[num]* is set before client_socket is accepted. So, always *set_zero* and set target socket before use.
<br><br>

### **--Previous Vision--**

* `v1.0 One_Client_Only`
    * Function & Structure
      >* Socket
      >* Bind
      >* Listen
      >* Loop{
        >>* Accept
        >>* Recv
        >>* Display
    * Behavior
      * Basic TCP_Server function
      * Blocking mode: block at *accept*
      * No threads, so can only accept&recv one client 
<br><br>
* `v2.0 Threads`
    * Function & Structure
      >* Socket
      >* Bind
      >* Listen
      >* Threads{
        >>* Loop{
          >>>* Client check
          >>>* Select{
            >>>>* Recv
            >>>>* Display
      >* Loop{
        >>* Accept
    * Behavior
      * Send&Recv client message
      * Main: blocking mode, block at *accept*
      * Threads: Recv and process client message while main is waiting to accept new clients
        * Blocking mode (with time limit): Hang-up at *select* for seconds and release if no message is received.
    * Comment:
      * Blocking mode can repidly decrease cpu usage. (Otherwise, it will keep requesting like a spinlock)
      * Threads is detached from main to simplify work, but may cause problem in actual work. (e.g., threads will be killed when main is finished, regardless of threads process.
      * Since recv message will be blocked by *select*, client_socket is set to non-blocking mode by *accept4*
      * Using array to transmit all client_socket into one threads to decrease the amount of threads. Create one sperate threads for each client_socket should also work.
      * Select is a good way to hang-up threads which is more accuracy and seems more safe.
      * Parameter in *Select* need to be initialize every time before use to refresh target socket (Recommanded by Microsoft document). 
        * After initialize, the default set is,
          * Hang-up time: Set to *NONE* (Block with no time limit)
          * Target socket: Won't trigger any socket.
        * Even *array->client_socket[num]* is used (so it should monitor target address), *select* won't trigger new socket if *array->client_socket[num]* is set before client_socket is accepted. So, always *set_zero* and set target socket before use.




[^1]:Running_In_Ubuntu_Eclipse
[^2]:v2.0_Threads