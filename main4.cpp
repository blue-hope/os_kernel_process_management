//
//  main4.cpp
//  os2
//
//  Created by bluehope on 26/03/2019.
//  Copyright © 2019 bluehope. All rights reserved.
//

#include <iostream>
#include <sys/time.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <unistd.h>
#include <string.h>
using namespace std;

int main(int argc, const char * argv[]) {
    while(true){
        pid_t pid;
        int status;

        time_t curtime;
        struct tm *curtm;
        curtime = time(NULL);
        curtm = localtime(&curtime);

        string str;

        cout << "[" << curtm->tm_hour << ":" << curtm->tm_min << ":" << curtm->tm_sec << "]$ ";
        getline(cin, str);

        string exit = "exit";

        if(str.compare(exit) == 0){
            break;
        }

        pid = fork();

        if(pid < 0){
            perror("error in fork");
            return -1;
        }
        else if(pid == 0){
            size_t pos = 0;
            string token;
            string tmp = str;
            int step = 0;
            string command;
            string arg_0;
            string arg_1;
            string arg_2;

            while((pos = tmp.find(" ")) != string::npos){
                token = tmp.substr(0, pos);
                if(step == 0){
                    command = token;
                }
                else if(step == 1){
                    arg_0 = token;
                }
                else if(step == 2){
                    arg_1 = token;
                }
                else if(step == 3){
                    arg_2 = token;
                }
                tmp.erase(0, pos + 1);
                step++;
                if(step == 4) break;
            }
            
            string p1 = "./program1";
            string p2 = "./program2";
            string p3 = "./program3";

            if(command.compare(p1) == 0){
                //arg_0 is n number
                //arg_1 is m number
                char **argv = new char*[stoi(arg_1) + 5];
                for(int i = 0; i < stoi(arg_1) + 5; i++){
                    argv[i] = new char[100000];
                }
                
                int step2 = 0;
                while((pos = str.find(" ")) != string::npos){
                    token = str.substr(0, pos);
                    strcpy(argv[step2], token.c_str());
                    str.erase(0, pos + 1);
                    step2++;
                }

                strcpy(argv[stoi(arg_1) + 3], str.c_str());
                argv[stoi(arg_1) + 4] = NULL;

                if(execvp("./program1", argv) == -1){
                    perror("execvp errr");
                    return -1;
                }
            }
            else if(command.compare(p2) == 0){
                //arg_0 is process number
                //arg_1 is n number
                //arg_2 is m number
                char **argv = new char*[stoi(arg_2) + 6];
                for(int i = 0; i < stoi(arg_2) + 6; i++){
                    argv[i] = new char[100000];
                }
                
                int step2 = 0;
                while((pos = str.find(" ")) != string::npos){
                    token = str.substr(0, pos);
                    strcpy(argv[step2], token.c_str());
                    str.erase(0, pos + 1);
                    step2++;
                }

                strcpy(argv[stoi(arg_2) + 4], str.c_str());
                argv[stoi(arg_2) + 5] = NULL;

                if(execvp("./program2", argv) == -1){
                    perror("execvp errr");
                    return -1;
                }
            }
            else if(command.compare(p3) == 0){
                //arg_0 is thread number
                //arg_1 is n number
                //arg_2 is m number
                char **argv = new char*[stoi(arg_2) + 6];
                for(int i = 0; i < stoi(arg_2) + 6; i++){
                    argv[i] = new char[100000];
                }
                
                int step2 = 0;
                while((pos = str.find(" ")) != string::npos){
                    token = str.substr(0, pos);
                    strcpy(argv[step2], token.c_str());
                    str.erase(0, pos + 1);
                    step2++;
                }

                strcpy(argv[stoi(arg_2) + 4], str.c_str());
                argv[stoi(arg_2) + 5] = NULL;

                if(execvp("./program3", argv) == -1){
                    perror("execvp errr");
                    return -1;
                }
            }
        }
        else{
        }
        // waitpid(pid, &status, 0);
        wait(0);
    }
}
