#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/**/
static int test1() {
   Calendar *calendar;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS) {
      if(print_calendar(calendar, stdout, 1) == SUCCESS) {
         if(destroy_calendar(calendar) == SUCCESS){
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}
/*This tests if days can pass as negative*/
static int test2() {
   Calendar *calendar;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(clear_day(calendar, -5) == FAILURE){
         if(destroy_calendar(calendar) == SUCCESS){
            return SUCCESS;
         }
      }
   }

   return SUCCESS;
}

/*Testing if you can add two different events at the same time on the same day*/
static int test3(){
   Calendar *calendar;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(add_event(calendar, "hiking trip", 1200, 240, NULL, 4) == SUCCESS){
      if(add_event(calendar, "boating trip", 1200, 240, NULL, 4) == SUCCESS){
            if(destroy_calendar(calendar) == SUCCESS){
            return SUCCESS;
            }
         }
      }
   }
   return FAILURE;
}

/*Testing if you can find an event that is not in the calendar*/
static int test4(){
   Calendar *calendar;
   Event *event;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      add_event(calendar, "hiking trip", 1200, 240, NULL, 4);
      if(find_event(calendar, "I'm not here!", &event) == FAILURE){
         if(destroy_calendar(calendar) == SUCCESS){
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}

/*Testing if you can find an event in a day*/
static int test5(){
   Calendar *calendar;
   Event *event;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      add_event(calendar, "hiking trip", 1200, 240, NULL, 4);
      if(find_event_in_day(calendar, "hiking trip", 4, &event) == SUCCESS){
         if(destroy_calendar(calendar) == SUCCESS){
            return SUCCESS;
         }
      }
   }
   return FAILURE;
}

/*Testing if you can find two different events in a day*/
static int test6(){
   Calendar *calendar;
   Event *event;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(add_event(calendar, "hiking trip", 1200, 240, NULL, 4) == SUCCESS &&
         add_event(calendar, "boating trip", 1500, 240, NULL, 4) == SUCCESS){
         if(find_event_in_day(calendar, "hiking trip", 4, &event) == SUCCESS 
            && find_event_in_day(calendar, "boating trip", 4, &event) 
               == SUCCESS){
               if(destroy_calendar(calendar) == SUCCESS){
                  return SUCCESS;
               }
         }
      }
   }
   return FAILURE;
}

/*Testing if you can remove an event after the event has already been removed*/
static int test7(){
   Calendar *calendar;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(add_event(calendar, "hiking trip", 1200, 240, NULL, 4) == SUCCESS){
         if(remove_event(calendar, "hiking trip") == SUCCESS){
            if(remove_event(calendar, "hiking trip") == FAILURE){
               if(destroy_calendar(calendar) == SUCCESS){
                  return SUCCESS;
               }
            }
         }
      }
   }
   return FAILURE;
}

/*Testing basic get_event_info if the event is in the calendar*/
static int test8(){
   Calendar *calendar;
   Event *event;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(add_event(calendar, "hiking trip", 1200, 240, NULL, 4) == SUCCESS){
         if(find_event(calendar, "hiking trip", &event) == SUCCESS){
            if(get_event_info(calendar, "hiking trip") == NULL){
               if(destroy_calendar(calendar) == SUCCESS){
                  return SUCCESS;
               }
            }
         }
      }
   }
   return FAILURE;
}

/*Testing if you can add two separate days*/
static int test9(){
   Calendar *calendar;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(add_event(calendar, "hiking trip", 1200, 240, NULL, 4) == SUCCESS){
         if(add_event(calendar, "hiking trip", 1200, 240, NULL, 5) == FAILURE){
               if(destroy_calendar(calendar) == SUCCESS){
                  return SUCCESS;
               }
         }
      }
   }
   return FAILURE;
}

/*Testing basic add and clear days*/
static int test10(){
   Calendar *calendar;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(add_event(calendar, "hiking trip", 1200, 240, NULL, 4) == SUCCESS){
         if(add_event(calendar, "boating trip", 1200, 240, NULL, 5) == SUCCESS){
            if(clear_day(calendar, 4) == SUCCESS && clear_day(calendar, 5) 
               == SUCCESS){
               if(destroy_calendar(calendar) == SUCCESS){
                  return SUCCESS;
               }
            }
         }
      }
   }
   return FAILURE;
}

/*Testing basic add and clear calendar*/
static int test11(){
   Calendar *calendar;
   int days = 7;

   if(init_calendar("Calendar Tests", days, comp_minutes, NULL, &calendar) 
      == SUCCESS){
      if(add_event(calendar, "hiking trip", 1200, 240, NULL, 4) == SUCCESS){
         if(add_event(calendar, "boating trip", 1200, 240, NULL, 5) == SUCCESS){
            if(clear_calendar(calendar) == SUCCESS){
               if(destroy_calendar(calendar) == SUCCESS){
                  return SUCCESS;
               }
            }
         }
      }
   }
   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/
   printf("Test 1:\n");
   if (test1() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 2:\n");
   if (test2() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 3:\n");
   if (test3() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 4:\n");
   if (test4() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 5:\n");
   if (test5() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 6:\n");
   if (test6() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 7:\n");
   if (test7() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 8:\n");
   if (test8() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 9:\n");
   if (test9() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
    printf("Test 10:\n");
   if (test10() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }
   printf("Test 11:\n");
   if (test11() == FAILURE){ 
      printf("uh oh...\n");
      result = FAILURE;
   }else{
      printf("SUCCESS\n");
   }

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
