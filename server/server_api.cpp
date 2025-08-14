/*
 * Author: Dev Rathod
 * Email: devmanojrathod@gmail.com
 * Date: 31st July, 2025
 * 
 * This file initiates and defines all the processes and commands available
 * on the server sides
 * 
 * A brief summary of all the commands and their use cases are defined below
 *  -commit -- Saves the user's latest changes to the repository
 *  -init   -- Sets up a folder as a new repository for version control
 *  -pull   --  Fetches and merges updates from the repository into the local branch
 * 
 * */
#include "server_api.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
using namespace std;

std::string ServerAPI::requestHandler(const std::string& requestStr) {
    json response;

    cout<<"Request:"<<requestStr<<endl;

    try {
        json req = json::parse(requestStr);

        std::string token = req.value("token", "");
        std::string command = req.value("command", "");
        cerr<<"Original Repo Name:"<<req.value("repo_name", "");
        std::string repoName = req.value("repo_name", "");

        if (!authManager.isValidToken(token)) {
            response["status"] = "error";
            response["message"] = "Invalid token";
            return response.dump();
        }

        std::string username = authManager.getUsernameFromToken(token);

        /*
         * While giving the command through the socket
         * The command format for:
         *  - init: '{"token": "my_secret_token", "command": "commit"}'
         *  - commit: '{"token": "my_secret_token", "command": "commit", "content": "my_content"}'
         */
        cout<<"Command:"<<command<<"\tCorrespondance:"<<(command == "init")<<endl;
        if (command == "init") {
            if (repoManager.initRepo(username, repoName)) {
                response["status"] = "success";
                response["message"] = "Repository initialized.";
            } else {
                response["status"] = "error";
                response["message"] = "Failed to initialize repository.";
            }
        } else if (command == "commit") {
            std::string content = req.value("content", "");
            std::string repoPath = req.value("repo_path", "");
            if (repoManager.commitRepo(username, repoName, repoPath, content)) {
                response["status"] = "success";
                response["message"] = "Commit successful.";
            } else {
                response["status"] = "error";
                response["message"] = "Commit failed.";
            }
        } else if(command == "pull"){
            std::string fileName = req.value("file_name", "");
            std::cerr<<"Got pull request"<<endl;
            if(repoManager.pullLatest(username, repoName, fileName, "file")){
                response["status"] = "success";
                response["message"] = "Pull successful.";
            } else{
                response["status"] = "error";
                response["message"] = "Pull Failed";
            }
        }else {
            response["status"] = "error";
            response["message"] = "Unknown command.";
        }
    } catch (const std::exception& e) {
        response["status"] = "error";
        response["message"] = std::string("Server error: ") + e.what();
    }

    return response.dump();
}