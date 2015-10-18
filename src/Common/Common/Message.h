#pragma once

#include <stdlib.h> // provides EXIT_*


class Message{
private:
    static int m_verbosity;
    static int m_myRank;
    static int m_nb_proc;
    static int m_nb_threads;
    static bool m_debug;

 public:
    static void Initialize(int argc, char *argv[]);
    static void InfoRoot(int level, const char *format, ...);
    static void InfoRoot(const char *format, ...);

    static void Print(int withInfo, int level, const char *format, ...);
    static void Print(int withInfo, const char *format, ...);
    static void Print(const char *format, ...);

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
    static int GetRank(){return m_myRank;}
    static int GetNProc(){return m_nb_proc;}
    static int GetNumThreads(){return  m_nb_threads;}
    static int GetThreadNum();
    static bool RootMpi(){return (m_myRank == 0);}
    static int Finalize(int status);
    static void Help();
    //MPI
    static void Barrier();

    static void setDebug(){m_debug = true; Debug("mode activacted");}
    static void unsetDebug(){Debug("mode desactivacted"); m_debug = false;}
};
