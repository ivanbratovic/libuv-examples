# libuv-examples
## *Example app using libuv.*

## Functionality

### 1. TCP Echo server

The app uses a uv tcp server to open a TCP socket and listen for a client. It accepts a single TCP client and replies
to all messages with the same message.

It prints the number of bytes recieved and time elapsed on client disconnect.

### 2. Timer

The app uses uv timers to track the time passed between two clients.

### 3. Local echo (polling)

The app also recieves input from standard input. Echoes the same input back to standard output.
It detects input on stdin by using uv polls.

## Running

### Requirements:

 + libuv
 
### Compilation and execution

```
 $ gcc echo_server_tcp.c -luv -o echo_server_tcp 
 $ ./echo_server_tcp
```
