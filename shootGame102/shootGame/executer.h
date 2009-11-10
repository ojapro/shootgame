#ifndef _EXECUTER_H
#define _EXECUTER_H

    #define EXECUTE_FAILED  0
    #define EXECUTE_SUCCESS 1
    #define EXECUTE_WAIT    2
    #define COMMAND_END     3
    #define INVALID_EXECUTE_COMMAND 4

    #define PROMOTE_FALED   0
    #define PROMOTE_SUCCESS 1
    #define PROMOTE_WAIT    2
    #define END_OF_SCENARIO 3

    #define PLACE_FALED     0
    #define PLACE_SUCCESS   1


    int PromoteScenario(Scenario *);
    int ExecuteScenario(Command *,clock_t*);
    int PlaceEnemy(char *,int,int,int);
    Enemy SearchLib(int ,Enemy *);


#endif