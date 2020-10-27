#include <iostream>
#include <queue>
#include <string>
#include <fstream>
#include <list>
#include <unordered_map>
#include <sstream>
#include <sys/time.h>
#include <stdlib.h>  
#include <chrono>
#include <ctime>

using namespace std;


#define MSG_QUEUE_SIZE 10240
#define NUM_CORES_TOTAL 64

pthread_barrier_t barrier; 
pthread_barrierattr_t attr;

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

/* TMC */
pthread_t   thread_handle[NUM_CORES_TOTAL];
thread_arg_t thread_arg[NUM_CORES_TOTAL];

struct MSG {
    unsigned long index;
    unsigned long priority;
};
MSG MSG_QUEUE[NUM_CORES_TOTAL][MSG_QUEUE_SIZE];
int MSG_LOC[NUM_CORES_TOTAL];
pthread_mutex_t core_locks[NUM_CORES_TOTAL];

/* Queue to hold tasks */ 
priority_queue<task> task_queue[NUM_CORES_TOTAL];

unsigned long TERMINATE[NUM_CORES_TOTAL];

int DIST_FACTOR = 8;
int MASTER_CORE = 0;
int NUM_CORES = 64;
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
    
    bool flag = 0;
    int term = 0;
    long unsigned ctr = core_id;
    int distr = 0;
    
    int node;
    long unsigned int distance;

    unsigned long c = 0;
    unsigned long a = 0;
    
    unsigned long start;
    unsigned long end;

    /* For dist factor */
    int remote_core_id;
    int dist_scaled;
    unsigned int message_1;
    unsigned int message_2;
    
    std::clock_t c_start;
    std::clock_t c_end;

    /* sw message */
    int curr_loc = 0;

    if (core_id == 0) {
        
        DISTANCE[0] = 0;
        task child_task = {0,0};
        task_queue[core_id].push(child_task);   

        /* Init core locks */
        for (int i = 0; i < NUM_CORES; i++) {
            pthread_mutex_init(&core_locks[i], NULL);
        }         
    }

    pthread_barrier_wait(&barrier);

     auto t1 = std::chrono::high_resolution_clock::now();
    
    while (gDone) { 
      
        pthread_mutex_lock(&core_locks[core_id]);
        int msg_loc_curr = MSG_LOC[core_id];
        pthread_mutex_unlock(&core_locks[core_id]);

        while (curr_loc != msg_loc_curr) {
            node        = MSG_QUEUE[core_id][curr_loc].index;
            distance    = MSG_QUEUE[core_id][curr_loc].priority;
            
            task child_task = {node, distance};
            task_queue[core_id].push(child_task);   

            curr_loc = (curr_loc + 1) % MSG_QUEUE_SIZE;   
        }

        
        if (task_queue[core_id].empty()) {
            if (term++ < 1000) continue;
            TERMINATE[core_id] = 1;
            
            int flag = 1;
            for (int i = 0; i < NUM_CORES; i++) {
                if (i == core_id) continue;
                if (TERMINATE[i] == 0) {flag = 0; break;}
            }
                

            if (flag == 1) break;
            else continue;
            
        }
        else {
            TERMINATE[core_id] = 0;
        }
        
       
        task curr_task = task_queue[core_id].top();
        task_queue[core_id].pop();
        
        curr_node = curr_task.node;
        curr_distance = curr_task.new_dist;

        int neighbors = rp[curr_node + 1] - rp[curr_node];
        for (int i = 0; i < neighbors; i++) {
    

            neighbor = ci[rp[curr_node] + i]; 
            // ai[rp[curr_node] + i]
            if (DISTANCE[neighbor] > curr_distance + 1) {
                dist = curr_distance + 1;
  
                /* Atomic swap */
                c = 0;
                do { 
                    a = DISTANCE[neighbor];
                    if (a > dist)
                        c = __sync_bool_compare_and_swap(&DISTANCE[neighbor], a, dist);
                    else 
                        break;
                }
                while (!c);


                /* Distribute load */
                if (c) {
                    if (distr < DIST_FACTOR) {
                        task child_task = {neighbor, dist};
                        task_queue[core_id].push(child_task);
                        distr = (distr + 1) % 10; 
                        
                    }
                 
                    else {
                        ctr =  (ctr + 1) % NUM_CORES;
                        if (ctr == core_id)  ctr = (ctr + 1) % NUM_CORES;
                        
                        pthread_mutex_lock(&core_locks[ctr]);
                        MSG_QUEUE[ctr][MSG_LOC[ctr]].index    = neighbor;
                        MSG_QUEUE[ctr][MSG_LOC[ctr]].priority = dist;
                        MSG_LOC[ctr] = (MSG_LOC[ctr] + 1) % MSG_QUEUE_SIZE;
                        pthread_mutex_unlock(&core_locks[ctr]);

                        distr = (distr + 1) % 10; 
                    }
                }

            }
        }
    }

    pthread_barrier_wait(&barrier);             
    /* Calculate time */
    if (core_id == 0) {
        auto t2 = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        std::cout << duration << endl;        
    }       
    
}


int main(int argc, char *argv[])
{  
  

    NUM_CORES = atoi(argv[1]);
    DIST_FACTOR = atoi(argv[2]);
    
    int ret; 
    ret = pthread_barrier_init(&barrier, &attr, NUM_CORES);

    /* Thread Code */
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
