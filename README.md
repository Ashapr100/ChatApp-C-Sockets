# 💬 ChatApp in C — End-to-End Socket Programming

An end-to-end chat application built in C using TCP sockets and Winsock2. It enables real-time messaging between multiple clients and a central server, showcasing core networking principles and system-level programming.

## 📦 Requirements
- Windows OS (tested on Windows 10/11)
- GCC compiler (MinGW or TDM-GCC)
- Winsock2 Library (-lws2_32)
- PowerShell or Command Prompt

## 🚀 How to Compile
```bash
gcc server.c -o server.exe -lws2_32
gcc client.c -o client.exe -lws2_32
```

## 💻 How to Run
1. Run server.exe in one terminal
2. Run client.exe in separate terminals
3. Start chatting!

## 📸 Demo

## 📁 Repository Structure
```
ChatApp-C-Sockets/
├── client.c
├── client.exe
├── server.c
├── server.exe
├── screenshot.png
├── .gitignore
└── README.md
```

## 🧠 Features
- TCP-based communication using sockets
- Multi-client support
- Real-time message relay via server
- Simple and clean terminal interface

## 👨‍💻 Author
**Akash Garimella** — Aspiring backend and networking developer

