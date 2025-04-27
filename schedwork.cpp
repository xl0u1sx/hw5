#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

bool findSchedule(
    const AvailabilityMatrix& availability,
    const size_t workersPerDay,
    const size_t maxShiftsPerWorker,
    DailySchedule& schedule,
    vector<size_t>& shiftsWorked,
    size_t day,
    size_t pos);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    
    sched.clear();
    
    const size_t numDays = avail.size();
    const size_t numWorkers = avail[0].size();
    sched.resize(numDays, vector<Worker_T>(dailyNeed, INVALID_ID));    
    vector<size_t> shiftsWorked(numWorkers, 0);

    return findSchedule(avail, dailyNeed, maxShifts, sched, shiftsWorked, 0, 0);
}

bool findSchedule(
    const AvailabilityMatrix& availability,
    const size_t workersPerDay,
    const size_t maxShiftsPerWorker,
    DailySchedule& schedule,
    vector<size_t>& shiftsWorked,
    size_t day,
    size_t pos
)
{
    const size_t numDays = availability.size();
    const size_t numWorkers = availability[0].size();
    
    // finished
    if(day >= numDays) {
        return true;
    }
    
    // move to next day when current complete
    if(pos >= workersPerDay) {
        return findSchedule(availability, workersPerDay, maxShiftsPerWorker, 
                           schedule, shiftsWorked, day + 1, 0);
    }
    
    // try each worker for current 
    for(size_t worker = 0; worker < numWorkers; worker++) {
        if(!availability[day][worker]) {
            continue;
        }
        if(shiftsWorked[worker] >= maxShiftsPerWorker) {
            continue;
        }
        bool alreadyWorking = false;
        for(size_t i = 0; i < pos; i++) {
            if(schedule[day][i] == worker) {
                alreadyWorking = true;
                break;
            }
        }
        if(alreadyWorking) {
            continue;
        }
        
        // try scheduling this worker
        schedule[day][pos] = worker;
        shiftsWorked[worker]++;
        
        if(findSchedule(availability, workersPerDay, maxShiftsPerWorker, 
                       schedule, shiftsWorked, day, pos + 1)) {
            return true;
        }
        
        // undo this try someone else
        schedule[day][pos] = INVALID_ID;
        shiftsWorked[worker]--;
    }
    
    // couldn't find a valid worker
    return false;
}

