#pragma once

#include <stdlib.h> // provides EXIT_*


// a class to manage messages
class Message{
private:
    static int p_verbosity;
    static int p_myRank;
    static int p_nb_proc;
    static int p_nb_threads;

 public:
    static void Initialize(int argc, char *argv[]);
    static void InfoRoot(int level, const char *format, ...);
    static void InfoRoot(const char *format, ...);
    static void Info(int level, const char *format, ...);
    static void Info(const char *format, ...);
    static void WarningRoot(const char *format, ...);
    static void WarningRoot(int level, const char *format, ...);
    static void Warning(const char *format, ...);
    static void Warning(int level, const char *format, ...);
    static void DebugRoot(int level, const char *format, ...);
    static void DebugRoot(const char *format, ...);
    static void Debug(int level, const char *format, ...);
    static void Debug(const char *format, ...);
    static void Error(int level, const char *format, ...);
    static void Error(const char *format, ...);
    static int Precision(){return 17;} //set decimal precision for output file
    static int GetRank(){return p_myRank;};
    static int GetNProc(){return p_nb_proc;};
    static int GetNumThreads(){return  p_nb_threads;};
    static int GetThreadNum();
    static bool RootMpi(){return (p_myRank == 0);}
    static int Finalize(int status);
    static void Help();
    //MPI
    static void Barrier();
};
