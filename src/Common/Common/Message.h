#pragma once

// a class to manage messages
class Message{
 private:
  static int m_verbosity;
  static int m_myRank;
  static int m_nb_proc;
  static int m_nb_threads;

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
  static int Precision(){return 17;} //set decimal precision for output file
  static int GetRank(){return m_myRank;};
  static int GetNProc(){return m_nb_proc;};
  static int GetNumThreads(){return  m_nb_threads;};
  static int GetThreadNum();
  static bool RootMpi(){return (m_myRank == 0);}
  static void Finalize(int status);
  static void Help();
  //MPI
  static void Barrier();
};

