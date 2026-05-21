# TCP Chat System in C

A multi-client terminal based chat application developed using **C Socket Programming** and **TCP connections**.

The project allows multiple users connected on the same WiFi/LAN network to communicate with each other in real time using a centralized server.

The application demonstrates:
- Socket Programming
- TCP Communication
- Linux System Calls
- Client-Server Architecture
- I/O Multiplexing using `select()`
- Multi-client handling without multithreading

---

# Project Features

- Multi-client chat system
- Real-time messaging
- Public chat
- Private messaging support
- Join/Disconnect notifications
- Timestamp based messages
- Colored terminal output
- Concurrent client handling using `select()`
- Dynamic client communication
- Linux terminal based application
- Makefile support for easy compilation

---

# Technologies Used

- C Programming
- TCP/IP Networking
- Linux Socket APIs
- POSIX System Calls
- `select()` System Call
- Makefile

---

# Project Structure

```text
chat_system/
│
├── server.c
├── client.c
├── Makefile
├── README.md
```

---

# Prerequisites

Before running the project make sure you have:

- GCC Compiler
- Linux / Ubuntu / WSL
- Basic terminal knowledge
- Multiple devices connected to same WiFi (for LAN chat)

---

# Install GCC (Ubuntu)

```bash
sudo apt update
sudo apt install gcc
```

---

# Verify GCC Installation

```bash
gcc --version
```

---

# Understanding Project Architecture

The project follows a **Client-Server Architecture**.

## Server Responsibilities

The server:
- creates a TCP socket
- binds socket to a port
- listens for incoming connections
- accepts multiple clients
- broadcasts messages
- handles private messages
- detects client disconnections

The server uses:

```c
select()
```

to monitor multiple sockets simultaneously without using threads.

---

## Client Responsibilities

The client:
- connects to server
- sends messages
- receives messages
- supports private messaging
- displays colored outputs
- handles real-time communication

---

# Socket Flow

The project internally follows this socket flow:

```text
socket()
   ↓
bind()
   ↓
listen()
   ↓
accept()
   ↓
send()/read()
   ↓
close()
```

---

# Important Concepts Used

## TCP Protocol

TCP is used because it provides:
- reliable communication
- ordered packet delivery
- retransmission of lost packets
- connection-oriented communication

---

## select() System Call

The server uses:

```c
select()
```

to handle multiple clients simultaneously.

It monitors:
- new client connections
- incoming client messages

without multithreading.

---

## File Descriptors

Sockets in Linux are represented as:
- file descriptors

This allows:
- `select()`
- `read()`
- `write()`

to work on sockets like normal files.

---

# Compile the Project Using Makefile

## Build Server and Client

```bash
make
```

This automatically compiles:
- server.c
- client.c

---

# Makefile Commands

## Compile Project

```bash
make
```

---

## Run Server

```bash
make run-server
```

---

## Run Client

```bash
make run-client
```

---

## Remove Executables

```bash
make clean
```

This removes:
- server
- client

executables.

---

# Running the Project

# Step 1 — Start Server

Run:

```bash
make run-server
```

Example output:

```text
=====================================
 Chat Server Started Successfully
=====================================

Server IP   : 192.168.1.5
Server Port : 8080

Waiting for clients...
```

---

# Step 2 — Start Client

Open another terminal:

```bash
make run-client
```

---

# Step 3 — Enter Server IP

The client dynamically asks for the server IP.

Example:

```text
Enter Server IP: 192.168.1.5
```

---

# Step 4 — Enter Username

Example:

```text
Enter Name: sonu
```

---

# Running on Same WiFi / LAN

To use this project on multiple devices:

- connect all devices to same WiFi
- run server on one device
- run client on other devices
- clients must enter the server machine IP

---

# Finding Server IP

Run on server machine:

```bash
hostname -I
```

Example:

```text
192.168.1.5
```

This is the IP address clients should enter while connecting.

---

# Why Dynamic IP Input Is Used

Instead of hardcoding the server IP inside `client.c`,
the client accepts IP dynamically during runtime.

Advantages:
- no recompilation required
- same executable works everywhere
- easier LAN testing
- supports future internet/VPS/ngrok connectivity
- more realistic client-server architecture

---

# Connection Flow

```text
Client Starts
      ↓
User Enters Server IP
      ↓
Client Creates Socket
      ↓
Client Connects To Server
      ↓
Username Sent To Server
      ↓
Real-Time Communication Starts
```

---

# Firewall Configuration (Ubuntu)

Allow port `8080`:

```bash
sudo ufw allow 8080
```

Verify firewall:

```bash
sudo ufw status
```

---

# Functionalities

# Public Messaging

Any normal text message gets broadcasted to all connected users.

Example:

```text
hello everyone
```

Output:

```text
[16:30] sonu: hello everyone
```

---

# Private Messaging

Private messages can be sent using:

```text
/msg username message
```

Example:

```text
/msg rahul hello bhai
```

Output:

```text
[16:32] (PM) sonu -> rahul: hello bhai
```

Private messages are visible only to:
- sender
- receiver

---

# Exit Chat

To leave chat:

```text
/exit
```

---

# Example Chat

```text
[16:45] [INFO] sonu joined the chat

[16:46] rahul: hello

[16:46] YOU: hi

[16:47] (PM) rahul -> YOU: how are you?

[16:48] [INFO] rahul disconnected
```

---

# Memory Layout Concepts Used

The project indirectly uses concepts like:

- Stack Memory
- Heap Memory
- File Descriptors
- Buffers
- Socket Buffers
- Dynamic Communication Handling

---

# Error Handling Done

The project handles:
- client disconnections
- failed connections
- empty socket reads
- duplicate message display issue
- exact username matching
- private message parsing

---

# Challenges Solved During Development

- Handling multiple clients simultaneously
- Preventing duplicate terminal messages
- Exact username matching for private chat
- Concurrent socket monitoring
- Real-time terminal updates
- Managing file descriptors correctly

---

# Future Improvements

Possible future upgrades:

- File transfer support
- Chat history
- Authentication system
- GUI version
- Encryption
- Internet-wide support using ngrok/VPS
- Group chat system
- Voice chat
- epoll() based scalable server
- Multithreaded implementation

---

# Concepts Demonstrated

This project demonstrates understanding of:

- TCP Socket Programming
- Linux System Calls
- Client-Server Communication
- Networking Fundamentals
- I/O Multiplexing
- C Programming
- Memory Handling
- Real-time Systems
- Multi-client Handling

---

# Learning Outcomes

After building this project, concepts learned include:

- how TCP communication works
- how sockets work internally
- how Linux handles file descriptors
- how select() multiplexing works
- how client-server systems communicate
- how concurrent networking systems are designed

---

# Interview Topics Covered

This project helps strengthen understanding of:
- TCP vs UDP
- File Descriptors
- select()
- Client-Server Architecture
- Linux System Calls
- Socket Programming
- Concurrent Client Handling
- Networking Fundamentals
- Real-Time Communication

---

# Author

Developed as a Linux Socket Programming and Networking project using C.
