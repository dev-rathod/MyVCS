/*
 * Author: Dev Rathod
 * Email: devmanojrathod@gmail.com
 * Date: 31st July, 2025
 * 
 * This file handles all the command executions on the repository level
 * 
 * A brief summary of all the funcitons(including their connections) and their use cases are defined below
 *  -commit/commitRepo -- Saves the user's latest changes to the repository
 *  -init/initRepo   -- Sets up a folder as a new repository for version control
 *  -pull/pullRepo   --  Fetches and merges updates from the repository into the local branch
 * 
 * vcs = version control system!
 * */

#include "repo_manager.h"
#include <vector>
#include <fstream>
// C++17+ -- ibrary allows you to manipulate files, directories, and path
#include <filesystem>
// Library that allows you to format streams (like padding, setting width, formatting dates/times).
#include <iomanip>
// Library that provides access to clocs and time points
#include <chrono>
// Library which lets you use strings as input/output streams (like a file or console)
#include <sstream>
#include <arpa/inet.h>     // 🧠 For inet_pton()
#include <sys/socket.h>    // For socket-related functions
#include <netinet/in.h>    // For sockaddr_in
#include <unistd.h>        // For close()
#include <regex>
#include <ctime>
#include <thread> // For std::this_thread
#include <chrono> // For std::chrono::milliseconds

using namespace std;
// For filesystem library
using namespace std::filesystem;

/*
 * Port Structure:
 *  - 6969 -- commit function
 *  - 9696 -- pull function 
 */
/*
 * function name: initRepo
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * Initiates the Repository which wants to be accessed by the end user
 * 
 * Inputs:
 *  - username -- constant string value inputted from the json file
 *  - repoName -- constant string value(product) which needs to be accessed by the user
 * 
 * Output: <bool>
 *  - true -- fuction correctly excecuted
 *  - false -- error during excecution
 */
bool RepoManager::initRepo(const string& username, const string& repoName){
    // Main Branch Repo Path
    path repoPath = "/Users/server/repos/" + repoName;
    // Specific User Repo Path
    path userPath = repoPath / ".myvcs/users" / username / "commits";
    cerr<<"In init Repo for:"<<repoName<<endl;
    cerr<<"Branches Created:"<<endl;
    try {
        create_directories(repoPath);
        // Creating userPath/Entering the Specific User Path
        cerr<<"\t"<<repoPath<<endl;
        create_directories(userPath);
        cerr<<"\t"<<repoPath<<userPath;
        // Creating/Navagating to the End commits folder
        create_directories(repoPath / ".myvcs/main/commits");
        // Creating/Navagating to a temp workspace for the user
        create_directories(repoPath / "workspace");
        // Creating empty log files
        ofstream(repoPath / ".myvcs/users" / username / "log.txt").close();
        ofstream(repoPath / ".myvcs/users" / username / "file_db.json").close();
        ofstream(repoPath / ".myvcs/main/log.txt").close();

        // Create default config.json (just placeholder for now)
        ofstream configFile(repoPath / ".myvcs/config.json");
        configFile << "{ \"branches\": [\"main\"], \"users\": [\"" << username << "\"] }";
        configFile.close();

        return true;
    } catch(const exception& e){
        cerr << "Error in repo_manager::initRepo: " << e.what() << endl;
        return false;
    }
}


/*
 * function name: currentTimestamp
 * Creation Date: 5th August, 2025
 * Author: Dev Rathod
 * 
 * This funciton collects and returns the current time for logging
 *  
 * Output: <string>
 */
std::string currentTimestamp() {
    std::ostringstream oss;
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    oss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

/*
 * function name: jsonLatestFileVersion
 * Creation Date: 5th August, 2025
 * Author: Dev Rathod
 * 
 * Function that checks and returns the lastest file version in the json selectives database
 *  
 * Inputs:
 *  - filename -- a variable used to store the filename (variable to be searched)
 *  - filepath -- used to store the path of the database
 *  - updateVersionNumber -- a boolean flag that shows the function if there is a need to update the version in database
 * 
 * Output: <int>
 *  - 0 -- filename is not present
 *  - +ve -- filename exists
 */
int jsonLatestFileVersion(const string& filename, const string& filepath, const bool updateVersionNumber)
{
    // Create a JSON object to hold the file's data
    json file_db;

    // Initialize the version number to 1 by default
    int latest_file_version = 1;

    // ============================
    // CASE 1: File doesn't exist or is empty
    // ============================
    if (!exists(filepath) || filesystem::file_size(filepath) == 0) {
        // Set metadata only once on first creation
        file_db["__meta__"] = {
            {"created_at", currentTimestamp()},
            {"created_by", "vcs_server"},
            {"description", "Version tracking database"}
        };

        // Set version to 1 if updateVersionNumber is true
        if (updateVersionNumber) {
            file_db[filename]["version"] = 1;
        }

        // Create and write the new JSON file
        ofstream createFile(filepath);
        if (!createFile.is_open()) {
            cerr << "Failed to create JSON file!" << endl;
            return -1;
        }

        createFile << file_db.dump(4); // Pretty-print with 4-space indentation
        createFile.close();

        // Return version 1 on fresh creation
        return 1;
    }

    // ============================
    // CASE 2: File exists — Read and update it
    // ============================

    // Open the JSON file for reading
    ifstream filebase(filepath);
    if (!filebase.is_open()) {
        throw runtime_error("Error accessing JSON file: " + filepath);
    }

    // Attempt to parse the file content into the JSON object
    try {
        filebase >> file_db;
    } catch (const json::parse_error& e) {
        cerr << "JSON parsing error: " << e.what() << endl;
        filebase.close();
        return -1;
    }

    // Close the input stream after reading
    filebase.close();

    // ============================
    // Update version logic
    // ============================

    if (file_db.contains(filename)) {
        // If the filename already exists in the JSON, get and increment the version
        latest_file_version = file_db[filename]["version"];
        latest_file_version++;

        if (updateVersionNumber) {
            // Update the version number only if update is requested
            file_db[filename]["version"] = latest_file_version;
        }
    } else {
        // File doesn't exist in the database, initialize version to 1
        latest_file_version = 1;

        if (updateVersionNumber) {
            file_db[filename]["version"] = 1;
        }
    }

    // ============================
    // Write updated data back to disk (only if updating)
    // ============================

    if (updateVersionNumber) {
        ofstream outfile(filepath);
        if (!outfile.is_open()) {
            cerr << "Failed to open file for writing: " << filepath << endl;
            return -1;
        }

        outfile << file_db.dump(4); // Write updated JSON back to file
        outfile.close();            // Close the file
    }

    // Return the latest version number (either new or read from file)
    return latest_file_version;
}

/*
 * function name: commitRepo
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * Commits the new version entered by the end user to the repository while logging the user into the log file
 * 
 * Used in:
 *  - commit funciton
 *  - pull function
 * 
 * Inputs:
 *  - username -- constant string value inputted from the json file
 *  - repoName -- constant string value(product) which needs to be accessed by the user
 *  - content  -- constant string value which getings entered into log file after pushing a new version to the main branch
 * 
 * Output: <bool>
 *  - true -- fuction correctly excecuted
 *  - false -- error during excecution
 */
bool RepoManager::commitRepo(const string& username, const string& repoName, const string& repoPath, const string& comments) {
    // Create/Nagavigate to the given directory
    cerr<<"In commitRepo function"<<endl;
    try{
        // Setting up the socket for commpunitcation 
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("[server] socket() failed");
            return false;
        }

        cerr<<"Connected to the socket"<<endl;

        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("[server] setsockopt() failed");
            return false;
        }

        cerr<<"Setting socket opt"<<endl;

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(6969);

        if (::bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("[server] bind() failed");
            return false;
        }

        cerr<<"Binding the socket"<<endl;

        if (listen(server_fd, 1) < 0) {
            perror("[server] listen() failed");
            return false;
        }

        cout << "[server] Listening on port_push 6969...\n";

        
        //Accepting client connection
        int client_fd = accept(server_fd, nullptr, nullptr);
        cout << "Accept Status: " << client_fd << endl;
        cout<<"Is accept working"<<endl;
        if (client_fd < 0) {
            cerr << "[server] Failed to accept client.\n";
            return false;
        }

        cout<<"Reading the lines in the file"<<endl;
        // Reading the file line by line 
        auto readLine = [&](string& line) {
            char ch;
            line.clear();
            while (read(client_fd, &ch, 1) == 1 && ch != '\n') {
                line += ch;
            }
        };

        string receivedUser, receivedRepo, receivedFilename;
        //First Line
        readLine(receivedUser);
        //Second Line   
        readLine(receivedRepo);
        //Third Line
        readLine(receivedFilename);

        // Extracting the name of the file from the repo path
        size_t slashPosition = repoPath.find_last_of('/');
        string fileName = (slashPosition != std::string::npos) ? repoPath.substr(slashPosition + 1) : repoPath;

        path commitDir = "/Users/server/repos/" + repoName + "/.myvcs/users/" + username + "/commits";
        cerr<<"CommitDir:"<<endl;
        create_directories(commitDir);

        // Authenticating if the information from the user matches, we recieved the ideal file || receivedFilename != expectedFilename
        if (receivedUser != username || receivedRepo != repoName ) {
            cerr << "[server] Header mismatch! Rejecting upload.\n";
            close(client_fd);
            close(server_fd);
            return false;
        }

        cerr<<"Username Recieved:"<<receivedUser<<"\tUsername:"<<username<<endl;
        cerr<<"Repo Recieved:"<<receivedRepo<<"\tRepo:"<<repoName<<endl;

        cout<<"Finding the current max version of the file available"<<endl;

        // Calculating which version according to the file name given was uploaded before this one
        int maxVersion = 0;
        // Checking if the filename exists in the database folder

        /*for (const auto& entry : directory_iterator(commitDir)) {
            string name = entry.path().filename().string();
            if (name[0] == 'V') {
                int v = stoi(name.substr(1, name.find('_') - 1));
                maxVersion = max(maxVersion, v);
            }
        }*/        

        // Setting up a json file path
        string jsonPath = "/Users/server/repos/" + repoName + "/.myvcs/users/" + username + "/file_db.json";
        // Incrementing the new version by one to keep effective version control
        int newVersion = jsonLatestFileVersion(fileName, jsonPath , false);
        string versionedName = "V" + to_string(newVersion) + "_" + fileName;
        path filePath = commitDir / versionedName;
        cerr<<"Commit Directory:"<<filePath<<endl;

        cerr<<"Expected Filename:"<<fileName<<endl;
        cerr<<"Copying the elements of the original file into a dublicated file"<<endl;
        // Copying the elements of the original file to a dublicated file
        // ofstream outFile(filePath, ios::binary);
        ofstream outFile(filePath, ios::binary);
        if (!outFile.is_open()) {
            cerr << "Failed to open file: " << filePath << endl;
            perror("ofstream");
            return 1;
        }

        // outFile << "Hello world!\n";
        // if (!outFile) {
        //     cerr << "Failed to write to file: " << filePath << endl;
        //     return 1;
        // }
        // cout << "File created successfully: " << filePath << endl;

        char buffer[4096];
        ssize_t bytesRead;
        while ((bytesRead = read(client_fd, buffer, sizeof(buffer))) > 0) {
            outFile.write(buffer, bytesRead);
        }

        // Closing all the files and sockets available
        outFile.close();
        close(client_fd);
        close(server_fd);

        cerr << "[server] File committed as: " << versionedName << "\n";

        // Creating a log file with the version name and date of upload
        ofstream logFile("server/repos/" + repoName + "/.myvcs/users/" + username + "/log.txt", ios::app);
        auto now = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(now);
        logFile << versionedName << " committed at "
                << put_time(localtime(&t), "%Y-%m-%d %H:%M:%S") << "\n";

        if(jsonLatestFileVersion(fileName, jsonPath, true) == newVersion)
        {
            cerr<<"Database Version Updated"<<endl<<"Version Number:"<<newVersion;
        } else{
            cerr<<"Error in json verion creation"<<endl;
        }

        cerr<<"Commited Successfully"<<endl;
        return true;

    } catch (const exception& e) {
        cerr << "[server] Commit failed: " << e.what() << "\n";
        return false;
    }
}

/*
 * function name: sendFile
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * This function sends the file to the end client during the pull commands
 * 
 * Commands used for:
 *  - pull command
 *  
 * Inputs:
 *  - clientSocket -- The socket in which the client is being connected
 *  - filePath -- the filepath of the file to be sent to the client
 * 
 * Output: <bool>
 */
bool sendFile(int clientSocket, const string& filePath, const string& fileName, const string& username){
    // Sending file details and username to client for verification
    string pullVerification = fileName + "\n" + username + "\n";
    cerr<<"Sending the string to the client:"<<pullVerification.c_str()<<endl;
    send(clientSocket, pullVerification.c_str(), pullVerification.size(), 0);

    // Making a file object
    cerr<<"Filepath for pulling:"<<filePath<<endl;
    ifstream file(filePath, ios::binary);
    if(!file.is_open())
    {
        cerr<<"Error could not open the file: "<< filePath << endl;
        return false;
    }

    // Creating a buffer to store each file in the file for sending it to the client
    char buffer[4096];

    this_thread::sleep_for(chrono::milliseconds(500));

    cerr<<"Succesfully sent the file to the client"<<endl;
    file.close();

    return true;
}

/*
 * function name: pullRepo
 * Creation Date: 31st July, 2025
 * Author: Dev Rathod
 * 
 * Pulls the latest version of the file from the repository to the end user
 *  
 * Inputs:
 *  - username -- constant string value inputted from the json file
 *  - repoName -- constant string value(product) which needs to be accessed by the user
 *  - repoPath -- The repo -- chere the repo is present 
 *  - transferType -- does the user wants to transfer a file or
 * 
 * Output: <string>
 */
bool RepoManager::pullLatest(const string& username, const string& repoName, const string& fileName, const string& transferType) {
    // Finding the path to the file
    path pullDir = "/Users/server/repos/" + repoName + "/.myvcs/users/" + username + "/commits";

    cerr<<"[repo_manager] In the function" << endl;

    try {
        /*
        // Extracting the name of the file from the repo path
        size_t slashPosition = repoPath.find_last_of('/');
        string fileName = (slashPosition != std::string::npos) ? repoPath.substr(slashPosition + 1) : repoPath;
        */
        // Finding the latest version number form the json database
        path latestFile;
        int highestVersion = jsonLatestFileVersion(fileName, repoName, false);
        //regex versionRegex(R"(V(\d+)_.*)");
        pullDir = "/Users/server/repos/" + repoName + "/.myvcs/users/" + username + "/commits/V" + to_string(highestVersion) + "_" + fileName;
        cerr<<"FileName:"<<fileName<<endl;

        cerr<<"Checking if the directory exists" << endl;

        // Checking if the directory exists on the server database
        // if (!exists(pullDir)) {
        //     return "ERROR: Commit directory does not exist.";
        // }

        cerr<<"Connecting socket"<<endl;

        // Connecting to the socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("[client] Socket creation failed");
            return false;
        }

        cerr << "connecting to port 9696" << endl;
        // Connecting to port 9696
         sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(9696);
        // inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        cerr << "Binding to the socket" << endl;
        if (::bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("[server] bind() failed");
            return false;
        }

        cerr<<"Binding the socket"<<endl;
 
        if (listen(sock, 1) < 0) {
            perror("[server] listen() failed");
            return false;
        }
        
        cerr<<"Listening on port 9696..."<< endl;

        //Accepting client connection
        int client_fd = accept(sock, nullptr, nullptr);

        if(client_fd < 0)
        {
            cerr<<"Client connection error"<<endl;
            return false;
        }

        /*
        if(!sendFile(client_fd, pullDir, fileName, username)){
            // close(sock);
            close(sock);
            cerr<<"Error while sending the file to the client"<<endl;
            return false;
        }
        */
        string pullVerification = fileName + "\n" + username + "\n";
        cerr<<"Sending the string to the client:"<<pullVerification.c_str()<<endl;
        send(client_fd, pullVerification.c_str(), pullVerification.size(), 0);

        // Making a file object
        cerr<<"Filepath for pulling:"<<pullDir<<endl;
        ifstream file(pullDir, ios::binary);
        if(!file.is_open())
        {
            cerr<<"Error could not open the file: "<< pullDir << endl;
            return false;
        }

        char buffer[4096];  
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            streamsize bytes = file.gcount();
            if (bytes > 0) {
                ssize_t sent = send(client_fd, buffer, bytes, 0);
                cerr<<buffer<<endl;
                if (sent < 0) {
                    perror("send");
                    break;
                }
            }
        }

        file.close();
        close(sock); 

        return true;
    } catch (const exception& e) {
        return bool("ERROR: Exception during pull: ") + e.what();
    }
}
