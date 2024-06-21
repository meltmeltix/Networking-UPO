# TCP server-client connection

- [Server](#server)
  - [Creating the server socket](#creating-the-server-socket)
  - [Setting up the server structure](#setting-up-the-server-structure)
  - [Listening for upcoming connections](#listening-for-upcoming-connections)
  - [Talking to the client](#talking-to-the-client)
- [Client](#client)
  - [Creating the socket](#creating-the-socket)
  - [Talking to the server](#talking-to-the-server)

## Server

### Creating the server socket

```c
int simpleSocket = 0;
int simplePort = 0;
int returnStatus = 0;
struct sockaddr_in simpleServer;
```

- `simpleSocket` holds the socket which will be created later;
- `simplePort` contains the port to open the server to;
- `returnStatus` utility value used to check any operation;
- `simpleServer` **Sock**et **Add**erss **In**. This specifies a transport address and a port for the address family.

```c
simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
```

Sock stream

To establish a TCP connection we must pass the `IPPROTO_TCP` as protocol. The function will return `-1` if something went wrong during the creation of the socket.

### Setting up the server structure

```c
memset(&simpleServer, '\0', sizeof(simpleServer));
simpleServer.sin_family = AF_INET;
simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
simpleServer.sin_port = htons(simplePort);

returnStatus = bind(
    simpleSocket, 
    (struct sockaddr *)&simpleServer,
    sizeof(simpleServer)
);
```

Before running anything it is necessary to setup the server correctly:

- `memset(&simpleServer, '\0', sizeof(simpleServer));`  
  To use `simpleServer`, since the variable wasn't initially initialized, we'll need to properly assign memory space so that we can fill it's properties;
- `simpleServer.sin_family = AF_INET;`  
  Setting up the Address family, same as the one used for the socket;
- `simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);`  
  `sin_addr` stands for *Socket Internet Address*. This is actually a struct, which inherits the `in_addr` struct: this struct contains only one field, `s_addr`, which will contains a transport IPv4 address.  
  Using `htonl` (*host to network long*), we **are binding the server socket to all available interfaces**.

>**Note that if the socket is set to bind to all available interfaces, the IP address will automatically be 0.0.0.0, meaning that the server is listening for request on any available address.**

Then, with `bind` we are binding the server structure to the socket. Returns `0` if it succeds.

> **So, what happened?**  
> Any kind of connection and operation is done through a socket. A socket needs a pretty specific structure to actually properly do something: that's why we had to setup a structure containing all the server information.  
> With bind, we pretty much told to the socket what it needs to do.

### Listening for upcoming connections

```c
returnStatus = listen(simpleSocket, 5);
```

As said in the main documentation, the server now intends to **listen for connections on this socket**. listen() puts the socket into *passive open mode* and allocates a **backlog queue** of pending connections.

Note that the *backlog queue* is the number passed as second parameter: the socket will queue (in this case) 5 connection requests before refusing any further request.

### Talking to the client

```c
struct sockaddr_in clientName = {0};
int simpleChildSocket = 0;
int clientNameLength = sizeof(clientName);

/* wait here */

simpleChildSocket = accept(
    simpleSocket, 
    (struct sockaddr *)&clientName,
    &clientNameLength
);
```

If the request queue is currently empty, the server simply awaits to establish a connection with the client.

Otherwise, it attempts to connect with the first request in the queue. Whenever accepting connections, `accept` will return a new, unique socket to talk to.

```c
returnStatus = read(simpleChildSocket, buffer, sizeof(buffer) - 1);
// ... verify read data

write(simpleChildSocket, buffer, strlen(buffer));
close(simpleChildSocket);
```

If everything went well, the server can start talking to the client socket and exchange data.

## Client

### Creating the socket

The process of crearting a socket is very streamlined across any socket function.

A server is a simple socket that depending on how it's created, will open up to the defined addresses. A client on the other hand is also a socket, but it connects to the server's specific (allowed) IP address and port.

```c
int simpleSocket = 0;
int simplePort = 0;
int returnStatus = 0;
struct sockaddr_in simpleServer;

// ...
simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// ...

// bzero(&simpleServer, sizeof(simpleServer));
memset(&simpleServer, '\0', sizeof(simpleServer));
simpleServer.sin_family = AF_INET;
// inet_addr(argv[2], &simpleServer.sin_addr.s_addr);
simpleServer.sin_addr.s_addr = inet_addr(argv[1]);
simpleServer.sin_port = htons(simplePort);
```

### Talking to the server

```c
printf("Input any string: ");
fgets(buffer, sizeof(buffer), stdin);
write(simpleSocket, buffer, sizeof(buffer));

returnStatus = read(simpleSocket, buffer, sizeof(buffer));
close(simpleSocket);
```

If everything went well, the client can start talking to the server socket and exchange data.
