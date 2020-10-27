#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <list>
#include <sstream>
#include <sys/time.h>
#include <ctime>
#include <chrono>
using namespace std;

//Edge Task Struct
struct task {
   unsigned long node;
   unsigned long new_dist;

   bool operator<(const task& rhs) const
   {
      return new_dist > rhs.new_dist;
   }
};

typedef struct
{
    int cpu_num;
} thread_arg_t;

extern unsigned long ai[];
extern unsigned long ci[];
extern unsigned long rp[];
extern unsigned long DISTANCE[];
extern unsigned long ACTUAL_DISTANCE[];
extern unsigned long MAX_NODE;

pthread_t   thread_handle[1024];
thread_arg_t thread_arg[1024];


priority_queue<task> task_queue[64];
unsigned long TERMINATE[64];
unsigned long RECV[64];
unsigned long COUNT[64];
unsigned long WORK_DONE[64];


struct timeval stop, start;
int NUM_CORES = 1;
int gDone = 1;
void* controller(void* args);
void* controller(void* args) {
    
    int core_id = 0;    
    thread_arg_t* arg = (thread_arg_t*) args;
    core_id = arg->cpu_num;
    
    unsigned long curr_node  = 0;
    unsigned long curr_distance = 0;
    unsigned long  neighbor   = 0;
    unsigned long  dist = 0;
    task curr_task;
    bool flag = 0;
    int term = 0;
    long unsigned ctr = core_id;
    int distr = 0;
    std::clock_t c_start;
    std::clock_t c_end;
    int node;
    long unsigned int distance;
    unsigned long start, end;

    if (core_id == 0) {
        /* Distance is key here */
        /* DISTANCE of all the nodes is infinity right now */
        DISTANCE[0] = 0;
        /* Create a child task 0 with degree */
        task child_task = {0, 0};

        task_queue[core_id].push(child_task);
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    
    while (!task_queue[core_id].empty()) { 

        /* get current task */
        task curr_task = task_queue[core_id].top();
        task_queue[core_id].pop();
        curr_node = curr_task.node;
        curr_distance = curr_task.new_dist;
        
        
        /* for all the neighbors */
        int neighbors = rp[curr_node + 1] - rp[curr_node];
        for (int i = 0; i < neighbors; i++) {
                        
            neighbor = ci[rp[curr_node] + i];
            /* Edge distance is less than the key */ // && InBFS[neighbor] == 0
            if (DISTANCE[neighbor] > curr_distance + ai[rp[curr_node] + i]) {
                dist = curr_distance + ai[rp[curr_node] + i];
                /* Update key */
                DISTANCE[neighbor] = dist;
                /* Create Task */
                task child_task = {neighbor, dist};
                task_queue[core_id].push(child_task);
            }
        }
       
    }
    
    /* Calculate time */
    auto t2 = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << duration << endl;        
}


int main(int argc, char *argv[])
{  
    for(int j = 0; j < NUM_CORES; ++j) {
        thread_arg[j].cpu_num = j;
    }

    for(int j = 0; j < NUM_CORES; j++) {
        pthread_create(thread_handle + j, NULL, controller, (void*)&thread_arg[j]);
    }

      
    for(int j = 0; j < NUM_CORES; j++) { 
        pthread_join(thread_handle[j], NULL);
    }
    return 0;
}
