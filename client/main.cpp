#include "commands/commit.h"
#include "commands/pull.h"
#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <filesystem>
#include <chrono>
#include <unistd.h>
#include <thread>   // for std::this_thread::sleep_for
#include <nlohmann/json.hpp>

#define PORT 5020

// Structure to store server addr
const char* vcs_addr = "127.0.0.1";

// Command to run once while executing the program
volatile bool initialized = false;

using namespace std;
using namespace std::filesystem;
using json = nlohmann::json;

/*
 * TODO:
 *      - Log in the comments whiel commenting and pull in the server
 *      - Write a code to push in the main branch 
 * 
 */


int main(int argc, char* argv[]) {
    // Default socket varaibles
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Opening the json file
    ifstream file("client_config.json");

    if(!file.is_open()){
        cerr<<"Could not open the password fingerprint \n";
        return 1;
    }

    // Creating a json to read the file contents
    json fingerprint;
    file>>fingerprint;
    

    // Extracting username and password from the json file 
    string username = fingerprint["username"];
    string pssd = fingerprint["public_key"];

    while(true){
        // Closing the sock for avoiding timeout
        // close(sock);

        // Run once to show the user the options they have
        if (initialized == false) {
            initialized = true;
            std::cout << "Usage:\n"
                    << "  - init <repoName>\n"
                    << "  - commit <repoName> <filePath> <comments>\n"
                    << "  - commit -d <repoName> <filePath> <comments>\n"
                    << "  - pull <repoName> <filename> <storingPath>\n"
                    << "  - pull -d <repoName> <folderpath>\n";
        }   
        
        if (!cin) {
            cerr << "cin is in a bad state (EOF or error)\n";
        }

        // Boolean variable to find weather or not the address is a variable or directory
        bool folderDirectory = false;

        // Getting the command response from the user
        string userInput;
        getline(cin, userInput);

        // Breaking all the words in the sentence into specific commands
        stringstream ss(userInput);
        vector<string> commands{
            istream_iterator<string>{ss},
            istream_iterator<string>{}
        };

        // Use IPv4
        serv_addr.sin_family = AF_INET;
        // Convert port to network byte order            
        serv_addr.sin_port = htons(PORT);

        // Create socket (IPv4, TCP, default protocol)
        sock = socket(AF_INET, SOCK_STREAM, 0); 
        if (sock < 0) {
            perror("Socket creation error");  // Show error if socket fails
            return -1;
        }

        // Convert IP string to binary and store in serv_addr
        if (inet_pton(AF_INET, vcs_addr, &serv_addr.sin_addr) <= 0) {
            perror("Invalid address");
            return -1;
        }

        // Variable to store all the file names in the directory
        vector <path> individual_files;

        // cout<<"Sockets Created\n";
        cout <<"For the if statement"<<endl;
        if(!commands.empty()){
            // Creating a json object to be transferred to the server
            json message_json;
            // Variable for converting json into a string object
            string message;

            // Adding username and fingerprint
            message_json["username"] = username;
            message_json["token"] = pssd;
            message_json["repo_name"] = commands[1];

            if (commands[0] == "init" && commands.size() == 2){
                // Send appropiate message to the server to enable communication for init
                message_json["command"] = "init";
                message_json["content"] = commands[3];
            }
            else if(commands[0] == "commit" && (commands.size() == 4 || commands.size() == 5)){
                /*// Send appropiate message to the server to enable communication for commits
                message_json["command"] = "commit";
                message_json["repo_path"] = commands[2];
                message_json["comments"] = commands[3];*/
                
                // Finding out if the user wants to upload a file or a folder
                if((commands.size() == 5) && (commands[1] == "-d"))
                {
                    // Folder section
                    if(is_directory(commands[3]))
                    {
                        message_json["command"] = "commit";
                        message_json["repo_path"] = commands[3];
                        message_json["repo_name"] = commands[2];
                        message_json["comments"] = commands[4];
                        folderDirectory = true;

                        for (const directory_entry& entry : directory_iterator(commands[3])) {
                            if (is_regular_file(entry.status())) {
                                individual_files.push_back(entry.path());
                            }
                        }
                    }
                    else{
                        cerr<<"Error: Please enter a valid directory"<<endl;
                    }
                } 
                else if((commands.size() == 4))
                {
                    // File section
                    if(is_regular_file(commands[2]))
                    {
                        // Send appropiate message to the server to enable communication for commits
                        message_json["command"] = "commit";
                        message_json["repo_path"] = commands[2];
                        message_json["comments"] = commands[3];
                    }
                }

                // commit con;
                // con.commitCommand(commands[1], commands[2]);
                // cout<<"message drafted\n";
            }
            else if(commands[0] == "pull" && commands.size() == 4){
                // Send appropiate message to the server to enable communication for pull
                
                /*
                if(commands.size() == 4 && commands[1] == "-d")
                {
                    // If the repo_path is a directory 
                    if(is_directory(commands[3]))
                    {
                        // Send appropiate message to the server to enable communications for commits
                        message_json["command"] = "commit";
                        message_json["type"] = "directory";
                        message_json["repo_path"] = commands[3];
                        message_json["repo_name"] = commands[2];
                    }
                }*/
                if(commands.size() == 4){
                    // If the repo_path is a file
                    cerr<<"Pull function"<<endl;
                    // if(is_regular_file(commands[3]))
                    // {
                        cerr<<"Processing command"<<endl;
                        message_json["command"] = "pull";
                        message_json["type"] = "file";
                        message_json["file_name"] = commands[2];
                        message_json["repo_name"] = commands[1];
                    // }
                }
            }
            else{
                initialized = false;
                cout << "Invalid command.\n";
            }

            // Converting the json object into a string
            message = message_json.dump();

            // Rerouting the request to particular request file
            // Send the Json Message to the server
            // Attempt to connect to the server (Raspberry Pi)
            if ((connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)){
                cerr<<"Connection failed client side"<<endl;
                perror("Connection Failed");
                return -1;
            }
            else if(initialized){
                if (commands[0] == "init" && commands.size() == 2){
                    // Send appropiate message to the server to enable communication for init
                    cerr<<"Sending Message!\n";
                    send(sock, message.c_str(), message.length(),0);
                    cout<<"Json string sent to the server:"<<message<<endl;
                }
                else if(commands[0] == "commit" && (commands.size() == 4 || commands.size() == 5)){
                    commit con;
                    // Identifing if we are sending a single file or folder
                    if(folderDirectory){
                        for (const auto& file : individual_files) {
                            message_json["repo_path"] = file.string();
                            message_json["filename"] = file.filename().string(); // Optional: add filename
                            message_json["type"] = "commit";

                            // Re-serialize the updated JSON
                            string message = message_json.dump();

                            this_thread::sleep_for(chrono::milliseconds(500));

                            cerr << "Sending Message!\n";
                            send(sock, message.c_str(), message.length(), 0);
                            cout << "Json string sent to the server: " << message << endl;

                            cout << "Uploading file: " << file.filename() << endl;
                            // con.commitCommand(file.filename(), file);
                            con.commitCommand(commands[2], file);
                            cout << "\t-- Sent file\n";
                        }
                    }else{
                        cerr<<"Sending Message!\n";
                        send(sock, message.c_str(), message.length(),0);
                        cout<<"Json string sent to the server:"<<message<<endl;

                        con.commitCommand(commands[1], commands[2]);
                        cout<<"message drafted\n";
                    }                    
                }
                else if(commands[0] == "pull" && (commands.size() == 3 || commands.size() == 4)){
                    // Creating a object for commands/pull.cpp
                    PullReq pull;

                    // Identifing if a file path is a directory or a regular file
                    // if(message_json["type"] == "file"){
                        // If the pull statement is intended for a single file
                        cerr<<"Sending Message!\n";
                        send(sock, message.c_str(), message.length(), 0);
                        cerr<<"Json string sent to the server:"<<message<<endl;

                        // TODO
                        pull.pullLatestCommand(commands[1], commands[3], commands[2]);
                        cout<<"message drafted\n";
                    // } else if(message_json["type"] == "directory"){
                    //     // If the pull statement is intended for a directory

                    // }
                }
                else{
                    initialized = false;
                    cout << "Invalid command.\n";
                }
            }
        }
    
        // Close the sock connection
        close(sock);
    }
   
    return 0;
}
