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

#define LOG_TEST_PATH "./resources/Tests/ScreenShots/"

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
#include "Objects/Timer.h"
#define TIME_TEST_FRAME_COUNT 100

class TimeTest
{
  Timer timer; /**< Timer that control the execution steps */


  /**
   * Reset the Test.
   */
  void reset();
  
  /**
   * String with the results
   */
  string results();

public:
  /**
   * Respective kernels time accumulators
   */
  double kColorTime;
  double kDeferedPeelingTime;
  double kSSAOTime;
  double kCombineTime;
  double kBlurTime;


  double kVoxelizationTime;
  double kConeTracingTime;

  double totalTime; /**< Total time Accumulator */

  Timer totalTimer; /**< Total time Timer */

  int framesCounter; /**< Number of times results were collected */

  TimeTest();

  /**
   * Reset the variable Timer.
   */
  void resetTimer();

  /**
   * Return Timer diff time.
   */
  double getTime();

  /**
   * Increment the frame counter needed for the mean
   */
  void update();
  
  /**
   * Print/Log Partial Time Results.
   *  The time are calculated TIME_FRAME_COUNT times and 
   *  the results are the mean and total time percentage.
   */
  void printPartialResults();
  void logPartialResults();
  
  /**
   * Print/Log Time Results.
   *  Print the current the mean and total time percentage.
   */
  void printResults();
  void logResults();
};
#endif

#ifdef SCREENSHOT_TEST
#include "GLUtils/GLTextureObject.h"
#include "Handlers/SphereGLCameraHandler.h"
#define SCREEN_TEST_INTERSCREEN_FRAMES 20
#define SCREEN_TEST_CAM_ROT_ANGLE 90
#define SCREEN_TEST_PATH "./resources/Tests/ScreenShots"


class ScreenShotTest
{
  GLTextureObject *m_texDebug;
  int m_frameCounter;
public:
  ScreenShotTest();
  ~ScreenShotTest();

  void update();
  bool isOver();
  void reset();
 
  void save(GLint texid, string path = "",  string type = "");

  void configureCamera(SphereGLCameraHandler *cam);
};
#endif

#endif