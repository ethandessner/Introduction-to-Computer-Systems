#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"

/*This function initializes a Calendar struct based on the parameters 
  provided.*/
int init_calendar(const char *name, int days, 
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar){
    
    if(calendar != NULL && name != NULL && days >= 1){
        (*calendar) = malloc(sizeof(Calendar));
        if(calendar){
            (*calendar)->name = malloc(sizeof(char) * (strlen(name) + 1));
            strcpy((*calendar)->name, name);
            (*calendar)->events = calloc(days, sizeof(Event));
            (*calendar)->days = days;
            (*calendar)->total_events = 0;
            (*calendar)->comp_func = comp_func;
            (*calendar)->free_info_func = free_info_func;
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*This function prints, to the designated output stream, the calendar’s name, 
  days, and total number of events if print_all is true; otherwise this 
  information is not printed. Information about each event (name, start time,
  and duration) is printed regardless the value of print_all.*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all){
    Event *traverse;
    int x = 0;

    if(calendar && output_stream){
        if(print_all){
            fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
            fprintf(output_stream, "Days: %d\n", calendar->days);
            fprintf(output_stream, "Total Events: %d\n\n", 
                                        calendar->total_events);   
        }
        fprintf(output_stream, "**** Events ****\n");
        if(calendar->total_events > 0){
            for(x = 0; x < calendar->days; x++){
                printf("Day %d\n", x + 1);
                traverse = calendar->events[x];
                while(traverse != NULL){
                    fprintf(output_stream, "Event's Name: \"%s\", ", 
                                traverse->name);
                    fprintf(output_stream, "Start_time: %d, ", 
                                traverse->start_time);
                    fprintf(output_stream, "Duration: %d\n", 
                                traverse->duration_minutes);
                    traverse = traverse->next;            
                }
            }
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*This function returns a pointer (via the out parameter event) to the event 
  with the specified name (if any). If the event parameter is NULL, no pointer 
  is returned. Notice the out parameter event should not be modified unless
  the event is found. The function returns FAILURE if calendar and/or name are 
  NULL. The function returns SUCCESS if the event was found and FAILURE 
  otherwise.*/
int find_event(Calendar *calendar, const char *name, Event **event){
    Event *find_this;
    int x = 0;

    if(calendar && name){
        for(x = 0; x < calendar->days; x++){
            find_this = calendar->events[x];
            while(find_this && strcmp(find_this->name, name) != 0){
                find_this = find_this->next;
            }
            if(find_this && event){
                *event = find_this;
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

/*This function adds the specified event to the list associated with the day 
  parameter. The event is added in increasing sorted order using the comparison 
  function (comp_func) that allows comparison of two events. The function 
  allocates memory for the new event and for the event’s name. Other fields of 
  the event struct are initialized based on the parameter values. This function 
  returns FAILURE if calendar and/or name are NULL, start time is invalid 
  (must be a value between 0 and 2400, inclusive), duration_minutes is less than 
  or equal to 0, day is less than 1 or greater than the number of calendar days, 
  if the event already exist, or if any memory allocation fails. Otherwise the 
  function will return SUCCESS.*/
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day){
    Event *new_event, *curr, *prev, *event;

    if(calendar && name){
        if((start_time >= 0 && start_time <= 2400)){ 
            if(duration_minutes > 0 && day > 0 && day <= calendar->days){
                if(find_event(calendar, name, &event) == FAILURE){
                    new_event = malloc(sizeof(Event));
                    new_event->name = malloc((strlen(name) + 1) * sizeof(char));
                    strcpy(new_event->name, name);
                    new_event->duration_minutes = duration_minutes;
                    new_event->start_time = start_time;
                    new_event->info = info;  
                    curr = calendar->events[day - 1];
                    /*if list is empty*/
                    if(curr == NULL){
                        calendar->events[day - 1] = new_event;
                        new_event->next = NULL;
                /*comparing the head with new_event*/
                    }else if(curr != NULL){
                        if(calendar->comp_func(curr, new_event) > 0){
                            calendar->events[day - 1] = new_event;
                            new_event->next = curr;
                        }else{
                            prev = curr;
                            curr = curr->next;
                        /*iterate until comp_func is greater than 0
                        this will require the program to put new event inbetween
                        prev and curr*/
                        while(curr && calendar->
                                      comp_func(curr, new_event) <= 0){
                            prev = curr;
                            curr = curr->next;
                        }
                        new_event->next = curr;
                        prev->next = new_event;
                        }
                    }
                    (calendar->total_events)++;
                    return SUCCESS;
                }
            }
        }
    }
    return FAILURE;
}

/*This function returns a pointer (via the out parameter event) to the event 
  with the specified name in the specified day (if such event exists). If the 
  event parameter is NULL, no pointer is returned. Notice the out parameter 
  event should not be modified unless the event is found. The function returns 
  FAILURE if calendar and/or name are NULL, or if the day parameter is less than 
  1 or greater than the number of calendar days. The function returns SUCCESS if 
  the event is found and FAILURE otherwise.*/
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event){
    Event *find_this;
    int x = 0;
    if(calendar && name && day > 0 && day <= calendar->days){
        for(x = 0; x < calendar->days; x++){
            /*we use x + 1 because days starts at 1*/
            if((x + 1) == day){
                /*set the find_this event to the start of the linked list 
                  of the desired day*/
                find_this = calendar->events[x];
                while(find_this && strcmp(find_this->name, name) != 0){
                    find_this = find_this->next;
                }
                if(find_this && event){
                    *event = find_this;
                    return SUCCESS;
                }
            }
        }
    }
    return FAILURE;
}

/*This function removes the specified event from the calendar, returning any 
  memory allocated for the event. If the event has an info field other than NULL 
  and a free_info_func exists, the function is called on the info field. The
  number of calendar events is adjusted accordingly. This function returns 
  FAILURE if calendar and/or name are NULL or if the event cannot be found; 
  otherwise the function returns SUCCESS.*/
int remove_event(Calendar *calendar, const char *name){
    Event *prev, *curr, **event = NULL;
    int x = 0;

    if(calendar && name && find_event(calendar, name, event) == FAILURE){
        for(x = 0; x < calendar->days; x++){
            curr = calendar->events[x];
            if(curr != NULL){
                if(strcmp(curr->name, name) == 0){
                    calendar->events[x] = curr->next;
                    curr->next = NULL;
                }else{
                    while(curr && strcmp(curr->name, name) != 0){
                        prev = curr;
                        curr = curr->next;
                    }
                    prev->next = curr->next;
                    curr->next = NULL;
                }
                free(curr->name);
                if(curr->info && calendar->free_info_func){
                    calendar->free_info_func(curr->info);
                }
                free(curr);
                (calendar->total_events)--;
                return SUCCESS;
            }
        }
    }
    return FAILURE;
}

/*This function returns the info pointer associated with the specified event. 
  The function returns NULL if the event is not found. Assume the calendar and 
  name parameters are different than NULL.*/
void *get_event_info(Calendar *calendar, const char *name){
    Event *event;
    if(find_event(calendar, name, &event) == FAILURE){
        return NULL;
    }
    return event->info;
}

/*This function removes all the events for the specified day, setting the event 
  list to empty. The total number of events is adjusted accordingly. If an event 
  has an info field other than NULL and a free_info_func exists, the function is 
  called on the info field. This function returns FAILURE if calendar is NULL, 
  or if the day is less than 1 or greater than the calendar days; otherwise the 
  function returns SUCCESS.*/
int clear_day(Calendar *calendar, int day){
    Event *prev, *curr;

    if(calendar && day > 0 && day <= calendar->days){
        if(calendar->total_events > 0){
            curr = calendar->events[day - 1];
            while(curr){
                prev = curr;
                curr = curr->next;
                free(prev->name);
                if(prev->info && calendar->free_info_func){
                    calendar->free_info_func(prev->info);
                }
                free(prev);
                (calendar->total_events)--;
            }
            calendar->events[day - 1] = NULL;
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*The calendar is looped through and clear_day is called for each index of the
  calendar. Clear_day was created first for the purpose of using it for
  clear_calendar and destroy_calendar*/
int clear_calendar(Calendar *calendar){
    int x = 0;

    if(calendar){
        for(x = 0; x < calendar->days; x++){
            clear_day(calendar, (x + 1));
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*The name parameter is freed before clear_calendar is called. Once 
  clear_calendatr is called, the events parameter is freed. Finally calendar
  itself is freed.*/
int destroy_calendar(Calendar *calendar){

    if(calendar){
        free(calendar->name);
        clear_calendar(calendar);
        free(calendar->events);
        free(calendar);
        return SUCCESS;
    }
    return FAILURE;
}