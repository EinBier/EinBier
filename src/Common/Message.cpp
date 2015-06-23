#include <iostream>
#include <fstream>
#include <sstream> //for osstream
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
#include <algorithm> //for remove_if, isspace

#include <Common/Message.h>

#if defined(HAVE_PETSC)
#include <petsc.h>
#endif

#if defined(HAVE_MPI)
#include <mpi.h>
#endif

#if defined(HAVE_OMP)
#include <omp.h>
#endif

int Message::m_verbosity = 4;
int Message::m_myRank = 0;
int Message::m_nb_proc = 1;
int Message::m_nb_threads = 1;
//PARAMETERS
//==============
//-------
void Message::Initialize(int argc, char *argv[])
{
#if defined(HAVE_MPI)
    MPI_Init(&argc, &argv);
    //MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &m_nb_proc);
    if(m_myRank == 0)
        Message::Info("Launched with MPI (%d processes)", m_nb_proc);
#endif
#if defined(HAVE_PETSC)
    PetscInitialize(&argc, &argv, PETSC_NULL, PETSC_NULL);
    Message::InfoRoot("Launched with PETSc (%d processes)", m_nb_proc);
#endif
#if defined(HAVE_OMP)
#pragma omp parallel
    {
        m_nb_threads = omp_get_num_threads();
    }
    Message::Info("Launched with OpenMP (%d threads)", m_nb_threads);
#endif
    //Parsing eventual options...
    int i = 1;
    bool showHelp = 0;
    while (i < argc) {
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i] + 1, "-help")) {
                showHelp = 1;
                i++;
            } else if (!strcmp(argv[i] + 1, "v")) {
                m_verbosity = atoi(argv[i+1]);
                i += 2;
                Message::InfoRoot("Verbosity set to: %d",m_verbosity);
            } else {
                Warning("What the hell is this option (skipped) ? (%s)", argv[i] + 1);
                i++;
            }
        }
        else {
            Warning("What the hell is this option (skipped) ? (%s)", argv[i]);
            i++;
        }
    }
    if (showHelp) {
        Message::Help();
        Message::Finalize(EXIT_SUCCESS);
    }
}

//Info...
void Message::Info(const char *format, ...)
{
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Info(0, str);
}

//with verbosity
void Message::Info(int level, const char *format, ...)
{
    if (level > m_verbosity)
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
#if defined (HAVE_MPI)
    fprintf(stdout, "Info[%d] : %s\n", m_myRank, str);
#else
    fprintf(stdout, "Info : %s\n", str);
#endif
    return;
}

void Message::InfoRoot(const char *format, ...)
{
    if (!Message::RootMpi())
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Info(str);
}

void Message::InfoRoot(int level, const char *format, ...)
{
    if (!Message::RootMpi())
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Info(level, str);
}


//Warning...
void Message::Warning(const char *format, ...)
{
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Warning(0, str);
}

void Message::Warning(int level, const char *format, ...)
{
    if (level > m_verbosity)
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    //to write in bold (\33[1m) + red (\33[31m)
    const char *c0 = "", *c1 = "";
    c0 = "\33[1m\33[31m"; c1 = "\33[0m";
    //
#if defined (HAVE_MPI)
    fprintf(stdout, "%sWarning[%d] : %s%s\n", c0, str, c1);
#else
    fprintf(stdout, "%sWarning : %s%s\n", c0, str, c1);
#endif
}

void Message::WarningRoot(const char *format, ...)
{
    if (!Message::RootMpi())
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Warning(format, str);
}

void Message::WarningRoot(int level, const char *format, ...)
{
    if (!Message::RootMpi())
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Warning(level, str);
}


//Debug...
void Message::Debug(const char *format, ...)
{
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Debug(0, str);
}

void Message::Debug(int level, const char *format, ...)
{
    if (level > m_verbosity)
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    //to write in bold (\33[1m) + red (\33[31m)
    const char *c0 = "", *c1 = "";
    c0 = "\33[1m\33[34m"; c1 = "\33[0m";
    //
#if defined (HAVE_MPI)
    fprintf(stdout, "%sDebug[%d]: %s%s\n", c0, m_myRank, str, c1);
#else
    fprintf(stdout, "%sDebug : %s%s\n", c0, str, c1);
#endif
}


void Message::DebugRoot(const char *format, ...)
{
    if(!Message::RootMpi())
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Debug(str);
}

void Message::DebugRoot(int level, const char *format, ...)
{
    if(!Message::RootMpi())
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Debug(level, str);
}

void Message::Error(const char *format, ...)
{
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    Error(0, str);
}

//with verbosity
void Message::Error(int level, const char *format, ...)
{
    if(level > m_verbosity)
        return;
    char str[1024];
    va_list args;
    va_start (args, format);
    vsnprintf (str, 1024, format, args);
    va_end (args);
    //to write in bold (\33[1m) + red (\33[31m)
    const char *c0 = "", *c1 = "";
    c0 = "\33[1m\33[31m"; c1 = "\33[0m";
    //
#if defined (HAVE_MPI)
    fprintf(stdout, "%sError[%d] : %s%s\n", c0, m_myRank, str, c1);
#else
    fprintf(stdout, "%sError : %s%s\n", c0, str, c1);
#endif

//Bert: Should we abord here ?
    return;
}


// Show help of MonteCarlo (options, ...)
void Message::Help()
{
    if(m_myRank>0)
        return;
    std::cout << "EinBier\n";
    std::cout << "S. Tournier and B. Thierry\n";
    std::cout << "For now, the only thing you can do is drink a beer.\n";
}

//To quit properly like a boss
int Message::Finalize(int status)
{
#if defined(HAVE_MPI)
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
#endif
    if(status == EXIT_SUCCESS)
        Message::Info("Exit with success (status %d)", status);
    else
        Message::Warning("Exit with error (status %d).", status);
    return status;
}


int Message::GetThreadNum()
{
#ifdef HAVE_OMP
    return omp_get_thread_num();
#else
    return 0;
#endif
}

void Message::Barrier()
{
#ifdef HAVE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
#endif
    return;
}
