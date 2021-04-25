#include "calculator_process.h"
#include <dirent.h>
#include <fstream>
#include <unistd.h>
//#include <sys/wait.h>
#include <iostream>

const std::string WP43S_BIN_DIR = "/home/pi/Developer/gitlab.com/Over_score/wp43s/";

CCaculatorProcess::CCaculatorProcess()
{
    calculatorBinName = WP43S_BIN_DIR + WP43S_PROCESS_NAME;
}

CCaculatorProcess::~CCaculatorProcess(){}

void CCaculatorProcess::StartOrFindProcess()
{
    iprocessId = FindProcessByName();
    if (-1 == iprocessId)
    {
        iprocessId = this->StartProcess();
    }
}

const bool CCaculatorProcess::IsCalculatorProcessRunning()
{
    return (-1 != FindProcessByName(WP43S_PROCESS_NAME));
}

const int CCaculatorProcess::StartProcess()
{
    /*
    int status;
    int pid = fork();
    // first we fork the process
    if (pid)
    {
       // pid != 0: this is the parent process (i.e. our process)
       waitpid(pid, &status, 0); // wait for the child to exit
    } 
    else
    {
        execl(calculatorBinName.c_str(), calculatorBinName.c_str(), NULL);
    }
    return status;
    */
   system(calculatorBinName.c_str());
   return 0;
}

const int CCaculatorProcess::FindProcessByName(const std::string procName)
{
    std::cout << "DEBUG: Looking for process " << procName << std::endl;
    int pid = -1;
    // Open the /proc directory
    DIR *dp = opendir("/proc");
    if (dp != NULL)
    {
        // Enumerate all entries in directory until process found
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            // Skip non-numeric entries
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
                // Read contents of virtual /proc/{pid}/cmdline file
                std::string cmdPath = std::string("/proc/") + dirp->d_name + "/cmdline";
                std::ifstream cmdFile(cmdPath.c_str());
                std::string cmdLine;
                std::getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    // Keep first cmdline item which contains the program path
                    size_t pos = cmdLine.find('\0');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    // Keep program name only, removing the path
                    pos = cmdLine.rfind('/');
                    if (pos != std::string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    // Compare against requested process name
                    std::cout << "DEBUG: comparing "<< procName << " and "<< cmdLine << std::endl;
                    if (procName == cmdLine)
                    {
                        std::cout << "DEBUG: Process Found !!!" << std::endl;
                        pid = id;
                    }
                }
            }
        }
    }

    closedir(dp);
    std::cout << "DEBUG: The process id found is " << pid << std::endl;
    return pid;
}