/* > Includes *****************************************************************/

#include "../inc/consoleLogger.hh"

/* > Defines ******************************************************************/

#define SHARE_FILE PROJECT_ROOT "data/consoleLogger"

/* > Methods ******************************************************************/

ConsoleLogger::ConsoleLogger()
{
  m_is_initializated = false;
  m_console_filename = "";
}

ConsoleLogger::~ConsoleLogger()
{
  if (m_is_initializated)
  {
    TRACE_INFO("Closing (%s) terminal", m_console_filename.c_str());

  // Close terminal window
    system((std::string("ps -ft ") + m_console_filename +
            std::string(" -o pid | grep [0-9] | xargs kill -9")).c_str());
  }
}

void ConsoleLogger::init()
{

  TRACE_INFO("Initializate console logger...");

  if (!std::ifstream(SHARE_FILE))
  {
    TRACE_WARNING("%s doesn't exist! Creating file...", SHARE_FILE);
  }
  
  std::fstream file;
  file.open(SHARE_FILE, std::fstream::in | std::fstream::out | std::fstream::trunc);

  if (!file)
  {
    FATAL_ERROR(ERROR_CANNOT_ACCESS_TO_FILE, "Cannot open %s file!",
                SHARE_FILE);
  }

  system("gnome-terminal -e \"bash -c \\\"tty > " SHARE_FILE "; exec bash\\\"\"");
  
  std::getline(file, m_console_filename);
  file.close();

  m_is_initializated = true;

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  this -> clear();

  return;
}

inline void ConsoleLogger::clear() const
{
  system((std::string("clear > ") + m_console_filename).c_str());
  return;
} 