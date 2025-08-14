/*
 * Author: Dev Rathod
 * Email: dev-rathod@hotmail.com
 * Date: 31st July, 2025
 * 
 * This file is the header file for server_api.cpp which handles
 * all the server side requests initiated by the user.
 * 
 * Two types/categories of managers are created in the file
 *  - repoManager --Handles all the repository Request 
 *  - authManager --Ensures valid fingerprint and authentication request
 * 
 * */
#include <iostream>
#include <string.h>
#include "repo_manager.h"
#include "auth_manager.h"

using namespace std;

 class ServerAPI{
    public:
        string requestHandler(const string& requestStr);

    private:
        RepoManager repoManager;
        AuthManager authManager;
 };