#include <memory>
#include <mutex>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "json.hpp"
#include <vector>

int RESOURCES, PROCESSES;

void computeSafety(const std::vector<int>& Available, std::vector<int>& Work, std::vector<bool>& Finish, std::vector<std::vector<int>>& Need, std::vector<std::vector<int>>& Allocation) {
    std::vector<int> result(PROCESSES);
    // result.resize(PROCESSES);

    for (int everyPass = 0; everyPass < PROCESSES; everyPass++) {
        //Check against other processes
        for (int process = 0; process < PROCESSES; process++) {
            if (Finish[process] == false) {
                // Check if load exceeds max
                bool flag = true;

                for (int resource = 0; resource < RESOURCES; resource++) {
                    if (Need[process][resource] > Available[resource]) {
                        flag = false;
                        break;
                    }
                }

                if (flag) {
                    // Safe
                    
                    result.insert(result.end(), process);

                    Finish[process] = true;

                    for (int i = 0; i < RESOURCES; i++) {
                        Work[i] += Allocation[process][i];
                    }
                }
            }
        }
    }

    bool Safe = true;
    for (int i = 0; i < PROCESSES; i++) if (Finish[i] == false) Safe = false;
    if (Safe) {
        std::cout << "Safe sequence computed.\n";
        for (std::vector<int>::iterator i = result.begin(); i < result.end(); i++) {
            // i++;
            // if (i == result.end()) {
            //     i--;
            //     std::cout << "P" << *i << "\nEnd of sequence.\n";
            // } else i--;
            std::cout << "P" << *i << " -> ";
        }
    } else {
        std::cout << "Unsafe sequence.\n";
        for (std::vector<int>::iterator i = result.begin(); i < result.end(); i++) {
            // i++;
            // if (i == result.end()) {
            //     i--;
            //     std::cout << "P" << *i << "\nEnd of sequence.\n";
            // } else i--;
            std::cout << "P" << *i << " -> ";
        }
        for (std::vector<bool>::iterator i = Finish.begin(); i < Finish.end(); i++) {
            // i++;
            // if (i == result.end()) {
            //     i--;
            //     std::cout << "P" << *i << "\nEnd of sequence.\n";
            // } else i--;
            // std::cout << "P" << *i << " -> ";
            std::cout << *i << ", ";
        }
    }
    // return result;

    // //Init Safety check
    // Work = Available;
    // //Check Allocation
    // for (int i = 0; i < PROCESSES; i++) {
    //     for (int resc = 0; i < RESOURCES; i++) {
    //         if (Allocation[i][resc] == 0) {
    //             Finish[i] = true;
    //         } else Finish[i] = false;
    //     }
    // }

    // int p;
    // for (int i = 0; i < PROCESSES; i++) {
    //     int need = Need[i][0] + Need[i][1] + Need[i][2];
    //     if (need <= Work[i] && Finish[i] == false) {
    //         Work[i] = Work[i] + ()
    //         Finish[i] = true;
    //     }
    // }
}

int main(int argc, char* argv[]) {
    std::ifstream File;
    
    File.open(argv[1]);

    nlohmann::json data = nlohmann::json::parse(File);
    // File.close();

    int processes = data["Processes"].size();
    PROCESSES = processes;
    int resources = data["Resources"].size();
    RESOURCES = resources;

    std::vector<int> Available(resources);
    for (int i = 0; i < resources; i++) Available[i] = data["Resources"][i]["Available"];

    std::vector<std::vector<int>> Max(processes, std::vector<int>(resources));
    std::vector<std::vector<int>> Allocation(processes, std::vector<int>(resources));
    std::vector<std::vector<int>> Need(processes, std::vector<int>(resources));

    std::vector<int> Work(resources);
    std::vector<bool> Finish(processes, false);

    for (int PID = 0; PID < processes; PID++) {
        for (int RID = 0; RID < resources; RID++) {
            Max[PID][RID]        = data["Processes"][PID]["Max"][RID];
            Allocation[PID][RID] = data["Processes"][PID]["Allocation"][RID];

            Need[PID][RID] = Max[PID][RID] - Allocation[PID][RID];
        }
    }
    computeSafety(Available, Work, Finish, Need, Allocation);
}