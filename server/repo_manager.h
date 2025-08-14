/*
 * Author: Dev Rathod
 * Email: dev-rathod@hotmail.com
 * Date: 31st July, 2025
 * 
 * This file handles all the command executions on the repository level
 * 
 * A brief summary of all the funcitons(including their connections) and their use cases are defined below
 *  -commit/commitRepo -- Saves the user's latest changes to the repository
 *  -init/initRepo   -- Sets up a folder as a new repository for version control
 *  -pull/pullRepo   --  Fetches and merges updates from the repository into the local branch
 * 
 * */

#include <iostream>
#include <nlohmann/json.hpp>
#include <string.h>
#include <exception>

using json = nlohmann::json;
using namespace std;

class RepoManager{
    public:
        bool initRepo(const string& username, const string& repoName);
       // string initRepo(const json& req);
        //string commitRepo(const json& req);
        bool commitRepo(const string& username, const string& repoName, const string& repoPath, const string& comments); 
        // string pullLatest(const json& req);
        bool pullLatest(const string& username, const string& repoName, const string& fileName, const string& transferType);
};