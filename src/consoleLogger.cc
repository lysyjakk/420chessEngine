/* > Includes *****************************************************************/

#include "../inc/consoleLogger.hh"

/* > Defines ******************************************************************/

#define SHARE_FILE PROJECT_ROOT "data/consoleLogger"

/* > Methods ******************************************************************/

void ConsoleLogger::init()
{
  if (!std::ifstream(SHARE_FILE))
  {
    TRACE_WARNING("%s doesn't exist! Creating file...", SHARE_FILE, NULL);
  }
  
  std::ifstream file(SHARE_FILE);

  if (!file)
  {
    FATAL_ERROR(ERROR_CANNOT_CREATE_FILE, "Cannot open %s file!",
                SHARE_FILE, NULL);
  }

  system("gnome-terminal -e \"bash -c \\\"tty > " SHARE_FILE "; exec bash\\\"\"");
  
  std::getline(file, m_console_filename);
  file.close();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  this -> clear();
}

void ConsoleLogger::clear()
{
  system((std::string("clear > ") + m_console_filename).c_str());
}

void ConsoleLogger::print(const std::string& format, Args ... args)
{
    size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if (size <= 0)
    {
      FATAL_ERROR(ERROR_DURING_FORMATTING, "Error during formatting!", NULL);
    }
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    snprintf( buf.get(), size, format.c_str(), args ... );
    std::cout << std::string( buf.get(), buf.get() + size - 1 );
}