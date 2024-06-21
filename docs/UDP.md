# UDP server-client connection

## Server / Listener

### Server Socket setup

#### Variables

```c
int simpleSocket = 0;
int simplePort = 0;
int returnStatus = 0;
struct sockaddr_in simpleServer;
```

- `simpleSocket` is the socket used to launch and use the server;
- `simplePort` contains the port to open the server to;
- `returnStatus` utility value used to check any operation;
- `simpleServer` **Sock**et **Add**erss **In**. This specifies a transport address and a port for the address family.

#### Defining the socket

```c
simpleSocket = socket(AF_INET, SOCK_DGRAM, 0);
```

To create a UDP socket, we will need to:

- Pass the address family the socket will use. `AF_INET` contains all the IPv4 family of addresses;
- Set the socket type to datagram passing `SOCK_DGRAM`;
- And then set the protocol to `0`, letting the system choose which protocol to use;

#### Setting up the server structure

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
  Using `htons` (*host to network short*), we **are binding the server socket to all available interfaces**.

>**Note that if the socket is set to bind to all available interfaces, the IP address will automatically be 0.0.0.0, meaning that the server is listening for request on any available address.**

Then, with `bind` we are binding the server structure to the socket. Returns `0` if it succeds.

> **So, what happened?**  
> Any kind of connection and operation is done through a socket. A socket needs a pretty specific structure to actually properly do something: that's why we had to setup a structure containing all the server information.  
> With bind, we pretty much told to the socket what it needs to do.

### Waiting for clients

Compared to a TCP socket, the socket will no longer start listening and accepting clients. Instead, the socket will simply wait for upcoming data as soon as a client connects to the socket.

Note that unlike in a TCP application, none of the instructions used to talk with another socket return a new `socket`. **UDP does not distinguish any sockets it is connected to**.

A UDP program to exchange data will use:

- `sendto(s, buf, len, flags, dest_addr, dest_addr_len)`  
  Sends `buf` data of `len` lenght to the `dest_address` of `dest_addr_len` lenght using the socket `s`. In other terms, the server socket sends the data from the buffer to the destination address.
- `recvfrom(s, buf, len, flags, src_addr, src_addr_len)`  
  Receives `buf` data of `len` lenght from the `src_addr` of `src_addr_len` lenght using the `s` socket. In other terms, the server socket receives the data and saves it into the budder from the source address.

```c
struct sockaddr_in clientName = {0};
char whoareyou[50];
int simpleChildSocket = 0;
int clientNameLength = sizeof(clientName);

memset(buffer, 0, sizeof(buffer));
returnStatus = recvfrom(
    simpleSocket, 
    buffer, 
    sizeof(buffer) - 1, 0,
    (struct sockaddr *)&clientName, 
    &clientNameLength
);

// ...

char response[] = "Hello, client!";
returnStatus = sendto(
    simpleSocket,
    response,
    sizeof(response),
    0,
    (struct sockaddr *)&clientName,
    clientNameLength
);
```

## Client / Talker

The client setup is pretty similar to the one used in a TCP application. The client simply attempts to connect to the server, and then exchange data accordingly.

Since the client is a UDP client, it needs to still send datagrams using the `sendto()` and `rcvfrom()` functions.

### Client Socket setup

```c
int simpleSocket = 0;
int simplePort = 0;
int returnStatus = 0;
char buffer[256] = "";
struct sockaddr_in simpleServer;
int serverLenght = sizeof(simpleServer);

simpleSocket = socket(AF_INET, SOCK_DGRAM, 0);

simplePort = atoi(argv[2]);
// bzero(&simpleServer, sizeof(simpleServer));
memset(&simpleServer, '\0', sizeof(simpleServer));
simpleServer.sin_family = AF_INET;
// inet_addr(argv[2], &simpleServer.sin_addr.s_addr);
simpleServer.sin_addr.s_addr = inet_addr(argv[1]);
simpleServer.sin_port = htons(simplePort);
```

The client socket follows the same exact setup as the server, making sure to use the datagram socket type.

Unlike the server, as client, we need to define to which IP does the socket need to talk to: so, **during the server structure setup**, instead of passing a whole family of addresses, we pass the IP address and Port of the server socket to talk to.

### Connecting to the server

```c
returnStatus = sendto(
    simpleSocket, 
    MESSAGE, 
    sizeof(MESSAGE), 
    0,
    (struct sockaddr *)&simpleServer, 
    serverLength
);

// ...

returnStatus = recvfrom(
    simpleSocket,
    MESSAGE,
    sizeof(MESSAGE),
    0, 
    (struct sockaddr *)&simpleServer, 
    &serverLenght
);
```
