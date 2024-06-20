# Networking-UPO Repository

The course provides information about **TCP** and **UDP** connection using C programs to demonstrate basic communication instructions and usages in a Server-Client environment.

## Server and Client

Further documentation about sockets, server-client interaction, TCP and UDP, visit the files linked below:

- [TCP](https://github.com/meltmeltix/Networking-UPO/blob/main/docs/TCP.md)
- [UDP](https://github.com/meltmeltix/Networking-UPO/blob/main/docs/UDP.md)

## Basic information and usage

- [Byte order](#byte-order)
- [Sockets and addresses](#sockets-and-addresses)
- [Functions](#functions)
  - [socket()](#socket)
  - [bind()](#bind)
  - [listen()](#listen)
  - [accept()](#accept)
  - [write() and close()](#write-and-close)

### Byte order

Ports and addresses are always specified in calls to the socket functions using the network byte order convention. Host byte order sorts bytes in the manner which is most natural to the host software and hardware. There are two common host byte order methods:

- **Network Order** uses *Little-endian*: byte ordering places the least significant byte first
- **Host Order** uses *Big-endian*: byte ordering places the most significant byte first

The following C functions allow the application program to switch numbers easily back and forth between the host byte order and network byte order without having to first know what method is used for the host byte order:

- `htonl()` or **h**ost **to** **n**etwork **l**ong: translates an unsigned long integer into network byte order.
- `htons()` or **h**ost **to** **n**etwork **s**hort: translates an unsigned short integer into network byte order.
- `ntohl()` or **n**etwork **to** **h**ost **l**ong: translates an unsigned long integer into host byte order.
- `ntohs()` or **n**etwork **to** **h**ost **s**hort: translates an unsigned short integer into host byte order.

### Sockets and addresses

When setting up a socket, it is required to choose the **address family**, the **type of service** and the **protocol** used by the socket.

Common **address faimilies** are:

- `AF_INET` and `AF_INET6` sockets provide a means of communicating between application programs that are on different systems using the TCP and UDP transport protocols provided by a TCP/IP product.x
- `AF_IUCV` sockets provide communication between processes on a single VM system, or on a group of systems that share IUCV connectivity.
- `AF_UNIX` sockets provide communication between processes on a single VM system, or on a group of systems that share a single Byte File System (BFS) server.

The `AF_INET` uses the following addresses:

```c
typedef unsigned long in_addr_t;
struct in_addr { in_addr_t s_addr; };
struct sockaddr_in {
        unsigned char  sin_len;           /* length of sockaddr struct */
        unsigned char  sin_family;      /* addressing family */
        unsigned short sin_port;          /* port number */
        struct in_addr sin_addr;          /* IP address */
        unsigned char  sin_zero[8];    /* unassigned */
};
```

### Functions

![Socket functions flow](https://raw.githubusercontent.com/meltmeltix/Networking-UPO/main/docs/img/function_call_flow.png)

#### socket()

```c
socket(AF_INET, SOCK_STREAM, 0)
```

Takes as parameters *the address family, the type of socket and the networking protocol* to use. By default, `0` is used, letting the system determine the appropriate protocol to use.

#### bind()

```c
struct sockaddr_in sa;
// ...
bind(s, (struct sockaddr *) &sa, sizeof sa)
```

Used to assign to the socket `s` the number of the port on which the server wants to provide it's server `(struct sockaddr *) &sa` and the IP address of the network connection.

The server puts the port number and IP address into a sockaddr_in structure, passing it and the socket file descriptor to the bind() function.

#### listen()

```c
listen(s, backlog_number);
```

The server now intends to listen for connections on this socket. `listen()` puts the socket into passive open mode and allocates a backlog queue of pending connections. In passive open mode, the socket is open for clients to contact.

#### accept()

```c
struct sockaddr_in sa;
int addrlen;
// ...
client_sock = accept(s, (struct sockaddr *) &sa, &addrlen);
```

Used by the server to accept a connection request from a client. When a connection is available, the socket created is ready for use to read data from the process that requested the connection. The call accepts the first connection on its queue of pending connections for the given socket socket.

#### write() and close()

```c
write(socket, MESSAGE, strlen(MESSAGE));
close(socket)
```

`write()` sends a buffer (MESSAGE) to the socket and `close()` closes the communication established with that socket.
