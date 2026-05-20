# TCP Chat System in C

A simple multi-client terminal based chat application built using **C Socket Programming** and `select()` system call.

This project allows multiple users to connect to a central server and chat with each other in real time over a local network (same WiFi/LAN).

---

# Features

- Multi-client chat system
- Real-time messaging
- Private messaging support
- Join/Disconnect notifications
- Timestamps for messages
- Colored terminal output
- Dynamic server IP input
- Supports multiple devices on same WiFi
- Uses `select()` for handling multiple clients without threads

---

# Technologies Used

- C Programming
- TCP Sockets
- Linux System Calls
- `select()` for multiplexing
- Socket Programming APIs

---

# Project Structure

```text
chat_system/
│
├── server.c
├── client.c
├── README.md
```

---

# Prerequisites

Before running the project make sure you have:

- GCC Compiler
- Linux / Ubuntu / WSL
- Basic terminal knowledge
- Multiple devices connected to same WiFi (optional for LAN chat)

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

# How It Works

## Server

The server:
- listens for incoming client connections
- accepts multiple users
- broadcasts messages
- handles private messages
- detects disconnects

The server uses:

```c
select()
```

to monitor:
- new connections
- incoming client messages

without creating multiple threads.

---

## Client

The client:
- connects to server
- sends messages
- receives messages
- supports private messaging
- shows colored messages

---

# Compile the Project

## Compile Server

```bash
gcc server.c -o server
```

---

## Compile Client

```bash
gcc client.c -o client
```

---

# Running on Same System

## Step 1 — Start Server

```bash
./server
```

Example output:

```text
Server IP   : 127.0.0.1
Server Port : 8080
```

---

## Step 2 — Start Client

Open another terminal:

```bash
./client
```

Enter:

```text
Enter Server IP: 127.0.0.1
Enter Name: sonu
```

---

# Running on Same WiFi / LAN

## Step 1 — Start Server

Run:

```bash
./server
```

Example:

```text
Server IP   : 192.168.1.5
Server Port : 8080
```

---

## Step 2 — Start Client on Another Device

Run:

```bash
./client
```

Enter:

```text
Enter Server IP: 192.168.1.5
Enter Name: rahul
```

Now both devices can communicate.

---

# Important Notes

- Both devices must be connected to same WiFi/LAN.
- Server must stay running while clients connect.
- Port `8080` should not be blocked by firewall.

---

# Allow Firewall Port (Ubuntu)

```bash
sudo ufw allow 8080
```

---

# Functionalities

## Public Chat

Any normal message gets broadcasted to all connected users.

Example:

```text
hello everyone
```

Output:

```text
[16:30] sonu: hello everyone
```

---

## Private Messaging

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

Private messages are displayed in different color.

---

## Exit Chat

To leave the chat:

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

# Concepts Used

This project demonstrates:

- TCP Socket Programming
- Client-Server Architecture
- File Descriptors
- `select()` System Call
- Multi-client Handling
- Network Communication
- Real-time Chat Systems

---

# Future Improvements

Some possible future upgrades:

- File sharing
- Group chats
- Chat history
- Encryption
- Authentication system
- GUI version
- Internet-wide support using ngrok/VPS
- Voice chat
- WebSocket version

---

# Author

Developed as a socket programming and networking project using C.
