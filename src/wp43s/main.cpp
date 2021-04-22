#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "wp43s.h"
#include "DEV_Config.h"
#include <iostream>
#include "testfeature.h"

using namespace std;

void  Handler(int signo)
{
    //System Exit
    cout << endl << "Handler:exit" << endl;
    DEV_Module_Exit();

    exit(0);
}


int main(int argc, char *argv[])
{
    signal(SIGINT, Handler);
    test_feature(argc, argv);
    return 0;
}