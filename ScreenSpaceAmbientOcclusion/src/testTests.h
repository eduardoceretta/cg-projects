/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011
 *
 *  Tests the application
 */
#ifndef _TESTS_H_
#define _TESTS_H_

#include <string>

/**
 * Tests Defines
 */
#define TESTS
#ifdef TESTS
#  define LOG_TESTS
#  define TIME_TEST
#  define SCREENSHOT_TEST
#endif



using namespace std;

/**
 * Logger Singleton.
 */
class TestLogger
{
public:
  /**
   * Get Singleton
   */
  static TestLogger* inst();

  /**
   * Define FileName Before first get of singleton.
   *  If not defined a unique name will be generated based on the date.
   */
  static void setFileName(string fileName);

  /**
   * Define Path before first get of singleton.
   *  If not defined current location will be used.
   */
  static void setPath(string path);
  
  /**
   * Closes the file
   */
  ~TestLogger();

  /**
   * Closes the file and frees memory.
   *  Next call of inst will create a new log.
   */
  void closeLog();

  /**
   * Logs the string
   */
  void log(string str);
  void logLine(string str);

private:
  static TestLogger * s_inst; /**< Singleton Instance */
  static string s_fileName;   /**< Log File Name */
  static string s_path;       /**< Log File Path*/
  FILE *m_fp;                 /**< Output File */

  /**
   * Singleton Constructor
   */
  TestLogger();

  /**
   * Create File
   */
  void init();
 
};


/**
 * Calculate the mean time and percentage that each kernel takes to execute.
 */
#ifdef TIME_TEST
#include <vector>
//#include "Objects/Timer.h"
#include "GLUtils/GLTimer.h"

#define MEAN_NUMBER 100

class TestApp;

class TimeTest
{
  //Timer m_timer; /**< Timer that controls the execution steps */
  GLTimer m_glTimer; /**< GLTimer that controls the GPU execution steps */

  struct Step{
    string name;
    void (*func)(TestApp*);
    TestApp* arg;
    // milliseconds
    double singleGPUMeasuredTime;
    //double singleCPUMeasuredTime;
    double meanGPUMeasuredTime;
    //double meanCPUMeasuredTime;
  };

  vector<Step> m_steps;
  
  // milliseconds
  double m_singleGPUMeasuredTotalTime;
  //double m_singleCPUMeasuredTotalTime;
  double m_meanGPUMeasuredTotalTime;
  //double m_meanCPUMeasuredTotalTime;

public:
  TimeTest();

  /**
   *	Clear all information. Erase all the steps
   */
  void clearProfiler();

  /**
   *	Add a new profile step. A step is composed by a function.
   */
  void pushBackStep(string name, void(*func)(TestApp*), TestApp * arg);

  /**
   *	Execute the time profile of the multiple steps.
   *  Each step is executed individually and has its time measured.
   *  Then each step is executed n times again and the mean time is obtained.
   *  The total time and the total mean time are also calculated.
   */
  void timeProfile();

  /**
   *	Return a string with the lua file format of the results.
   */
  string getFormatedResult();
};
#endif

#ifdef SCREENSHOT_TEST
#include "GLUtils/GLTextureObject.h"


class TestApp;

class ScreenShotTest
{
  GLTextureObject *m_texDebug;
public:
  ScreenShotTest();
  ~ScreenShotTest();

  string save(void (*func)(TestApp *), TestApp * argument, GLint texid, string path, string fileName = "");
};
#endif

#endif