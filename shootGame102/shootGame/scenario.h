#ifndef _SCENARIO_H
#define _SCENARIO_H


    int LoadAndSetEnemyLib(char *,Enemy *);
    void OptimizationSenarioString(char *,int);
    void unityNewLine(char *,int);
    void removeComment(char *,int);
    void removeSpace(char *,int);
    char* readLine(char *,char *,int ,int ,int *);
    Command CreateCommand(char *);
    int ApplyEnemyData(Enemy *,Command);
#endif