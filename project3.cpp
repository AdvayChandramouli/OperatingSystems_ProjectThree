/*
 * Advay Chandramouli
 * CS4348.501 - Prof. Greg Ozbirn
 * Project Three: OS Scheduling Algorithms
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Definition of Job class
class Job {
private:
    string job_name;
    int arrival_time;
    int remaining_time;
    int job_duration;
public:
    // Constructor
    Job() {};
    Job(string name, int time, int dur)
    {
        job_name = name;
        arrival_time = time;
        job_duration = dur;
        remaining_time = dur;
    }

    // Decrement time
    void decrementRemainingTime(int i)
    {
        remaining_time -= i;
    }

    // Returns name of job
    string getName() {
        return job_name;
    }
    // Returns remaining time
    int getRemainingTime()
    {
        return remaining_time;
    }

    // Returns arrival time
    int getArrivalTime() {
        return arrival_time;
    }

    // Returns Duration
    int getDuration() {
        return job_duration;
    }

    void printGraph(int numSpaces) {
        // Print out job name
        cout << job_name;

        // Creates proper spacing
        for (int i = 0; i <= numSpaces; i++) {
            std::cout << " ";
        }

        // Fills slots with 'X'
        for (int j = 0; j < job_duration; j++) {
            std::cout << "X";
        }
        cout << endl; // New line for next job
    }
};

// FCFS scheduling algorithm
void schedule_FCFS(vector<Job> jobList)
{
    int currentTime = 0;
    cout << "FCFS" << endl;
    for(auto i:jobList)
    {
        i.printGraph(currentTime);
        currentTime+=i.getDuration();
    }
    cout << " " << endl;
}

// Round Robin scheduling algorithm
void schedule_RoundRobin(vector<Job> jobList, int totalTime)
{
    int timeQuantum = 1;
    int currentTime = 0;
    cout << "RR" << endl;

    // 2D array to represent RR schedule matrix
    int RRmatrix[jobList.size()][totalTime];
    for(int i = 0; i < jobList.size(); i++)
    {
        for(int j = 0; j < totalTime; j++)
        {
            RRmatrix[i][j] = 0;
        }
    }
    Job x;
    int index = 0;

    // Perform Round Robin scheduling
    while(currentTime < totalTime)
    {
        for(int j = 0; j < jobList.size(); j++)
        {
            x = jobList.at(index);
            if((x.getArrivalTime() <= currentTime) && (x.getRemainingTime() > 0))
            {
                RRmatrix[index][currentTime] = 1;
                (jobList.at(index)).decrementRemainingTime(timeQuantum);
                currentTime+=timeQuantum;
                index = (index + 1)%(jobList.size());
                break;
            }
            else
            {
                index = (index + 1)%(jobList.size());
            }
        }
    }

    // Print Round Robin schedule matrix
    for(int i = 0; i < jobList.size(); i++)
    {
        cout << (jobList.at(i)).getName() << " ";
        for(int j = 0; j < totalTime; j++)
        {
            if(RRmatrix[i][j] == 1)
            {
                cout << "X";
            }
            else
            {
                cout << " ";
            }
        }
        cout << endl;
    }

    cout << " " << endl;
}

int main(int argc, char *argv[])
{
    vector<Job> jobs;
    string current_job;
    int total_jobtime = 0;
    // Check if less than 2 arguments provided
    if (argc < 2) {
        cout << "Invalid number of arguments!";
        exit(-1);
    }

    ifstream jobFile(argv[1]);
    if(!jobFile.is_open())
    {
        cout << "Error opening the file!" << endl;
        exit(1);
    }
    else{
        // Read job data from text file
        while(getline(jobFile, current_job))
        {
            istringstream ss(current_job);
            string name, arrival_timeStr, durationStr;

            if (getline(ss, name, '\t') && getline(ss, arrival_timeStr, '\t') && getline(ss, durationStr, '\t'))
            {
                // Convert arrival_timeStr and durationStr to integers
                int arrival_time = stoi(arrival_timeStr);
                int duration = stoi(durationStr);

                // Create a Job object using the extracted details and add it to the jobs vector
                jobs.push_back(Job(name, arrival_time, duration));
                total_jobtime += duration;
            }
            else
            {
                cerr << "Error parsing line: " << current_job << endl;
            }
        }
        jobFile.close();
        // Schedule jobs using FCFS and RR algorithms
        schedule_FCFS(jobs);
        schedule_RoundRobin(jobs, total_jobtime);
    }

    return 0;
}
