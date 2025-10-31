# 💬 ChatApp in C — End-to-End Socket Programming

An end-to-end chat application built in C using TCP sockets and Winsock2. It enables real-time messaging between multiple clients and a central server, showcasing core networking principles and system-level programming.

## 📦 Requirements

To compile and run this project, you’ll need:

- Windows OS (tested on Windows 10/11)
- GCC compiler (e.g., via MinGW or TDM-GCC)
- Winsock2 library (`-lws2_32`)
- PowerShell or Command Prompt for execution

## 🚀 Features

- Real-time messaging between server and multiple clients
- TCP socket communication using Winsock2
- Lightweight and fast — built in pure C
- Clean separation of client and server logic

## 🛠️ How to Compile

Make sure GCC is installed, then run:

```bash
gcc server.c -o server.exe -lws2_32
gcc client.c -o client.exe -lws2_32
```
