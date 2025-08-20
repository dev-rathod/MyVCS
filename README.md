# MyVCS  

A CLI-based version control system inspired by Git, enabling client-server communication over WebSockets within a local network, with server-side JSON storage for commits, branches, and repository state, and real-time updates sent to the client during pull operations.  

**Status:** Not Production Ready  
**Latest Version:** 1.0.0  
**Author:** Dev Rathod  

---

## 📂 Server Side Architecture  

```text
.
├── auth_manager.cpp
├── auth_manager.h
├── include/
├── json.hpp
├── main.cpp
├── readMe
├── repo_manager.cpp
├── repo_manager.h
├── repos/
│   └── testfolder/
│       └── workspace/
├── server_api.cpp
├── server_api.h
└── user_db.json
```
---

## 📂 Client Side Architecture  


```text
.
├── auth.cpp
├── auth.h
├── client_config.json
├── commands/
│ ├── commit.cpp
│ ├── commit.h
│ ├── pull.cpp
│ └── pull.h
├── main.cpp
├── myvcs
├── network_client.cpp
└── network_client.h
```
