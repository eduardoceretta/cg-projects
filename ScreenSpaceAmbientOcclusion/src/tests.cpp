/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011
 *
 *  Tests the aplication
 */
#include <iostream>
#include <GL\glew.h>
#include <GL\glut.h>
#include <cmath>
#include <ctime>


#include "tests.h"
/*******************************************/
/* TestLogger                              */
/*******************************************/
TestLogger * TestLogger::m_inst(NULL);

TestLogger* TestLogger::inst()
{
  if(!m_inst)
  {
    m_inst = new TestLogger();
#ifdef LOG_TESTS
    m_inst->init();
#endif
  }
  return m_inst;
}

TestLogger::~TestLogger()
{
#ifdef LOG_TESTS
  fclose(m_fp);
#endif // LOG_TESTS
}

void TestLogger::log( string str )
{
#ifdef LOG_TESTS
  fprintf(m_fp, str.c_str());
#endif
}

void TestLogger::logLine( string str )
{
#ifdef LOG_TESTS
  fprintf(m_fp, "%s\n", str.c_str());
#endif // LOG_TESTS
}

TestLogger::TestLogger()
{
}

void TestLogger::init()
{
  time_t rawtime;
  time(&rawtime);

  struct tm *ttm = localtime(&rawtime);
  char date[100];
  sprintf(date, "_%d_%d_%d-%d_%d_%d-%ld", ttm->tm_year + 1900, ttm->tm_mon, ttm->tm_mday,
    ttm->tm_hour, ttm->tm_min, ttm->tm_sec, clock());
  string fileName = string("log_") + string(date) + ".log";
  m_fp = fopen(fileName.c_str(), "w");
  
  log(string("SSAO LogFile. - Created: ") + string(asctime(ttm)) + string("\n\n"));
}


/*******************************************/
/* TimeTest                                */
/*******************************************/
#ifdef TIME_TEST
TimeTest::TimeTest()
{
  reset();
}

void TimeTest::reset()
{
  kColorTime = kDeferedPeelingTime = kSSAOTime = kCombineTime = kBlurTime = totalTime = 0.0;
  framesCounter = 0;
}

void TimeTest::resetTimer()
{
  timer.reset();
}

double TimeTest::getTime()
{
  return timer.getTime();
}

void TimeTest::update()
{
  framesCounter++;
}

void TimeTest::printPartialResults()
{
  if(framesCounter >= TIME_TEST_FRAME_COUNT)
  {
    printResults();
    reset();
  }
}

void TimeTest::logPartialResults()
{
  if(framesCounter >= TIME_TEST_FRAME_COUNT)
  {
    logResults();
    reset();
  }
}

void TimeTest::printResults()
{
  printf(results().c_str());
}

void TimeTest::logResults()
{
  TestLogger::inst()->log(results());
}

std::string TimeTest::results()
{
  string str = "";
  char tmp[500] = "";
  str+=tmp; sprintf(tmp, "\n-----------TIMER TEST-----------------\n"); 
  str+=tmp; sprintf(tmp, "Num Frames  Tested:%d\n", framesCounter);
  str+=tmp; sprintf(tmp, "Total         Time:%lf sec\n", totalTime/framesCounter); 
  str+=tmp; sprintf(tmp, "KernelColor   Time:%lf sec - %.2f ", kColorTime/framesCounter, 100. * kColorTime / totalTime);
  str+=tmp; sprintf(tmp, "KernelDefPeel Time:%lf sec - %.2f ", kDeferedPeelingTime/framesCounter, 100. * kDeferedPeelingTime / totalTime); str+= "%%\n";
  str+=tmp; sprintf(tmp, "KernelSSAO    Time:%lf sec - %.2f ", kSSAOTime/framesCounter, 100. * kSSAOTime / totalTime); str+= "%%\n";
  str+=tmp; sprintf(tmp, "KernelBlur    Time:%lf sec - %.2f ", kBlurTime/framesCounter, 100. * kBlurTime / totalTime); str+= "%%\n";
  str+=tmp; sprintf(tmp, "KernelCombine Time:%lf sec - %.2f ", kCombineTime/framesCounter, 100. * kCombineTime / totalTime); str+= "%%\n";
  str+=tmp; sprintf(tmp, "--------------------------------------\n\n"); str+= "%%\n";
  str+=tmp;
  return str;
}
#endif

/*******************************************/
/* ScreenShotTest                          */
/*******************************************/
#ifdef SCREENSHOT_TEST
#include <ctime>
ScreenShotTest::ScreenShotTest()
:m_texDebug(new GLTextureObject())
,m_frameCounter(-SCREEN_TEST_INTERSCREEN_FRAMES)
{
}

ScreenShotTest::~ScreenShotTest()
{
  delete m_texDebug;
}

void ScreenShotTest::save(GLint texid, string type /*= ""*/)
{
  if(m_frameCounter < 0 || m_frameCounter%SCREEN_TEST_INTERSCREEN_FRAMES != 0)
    return;
  m_texDebug->setId(texid);

  time_t rawtime;
  time(&rawtime);

  struct tm *ttm = localtime(&rawtime);
  char date[100];
  sprintf(date, "_%d_%d_%d-%d_%d_%d-%ld", ttm->tm_year + 1900, ttm->tm_mon, ttm->tm_mday,
                                         ttm->tm_hour, ttm->tm_min, ttm->tm_sec, clock());
  string fileName = string("screen_") + type + string(date) + ".png";
  m_texDebug->write2DToFile(fileName);
  
  TestLogger::inst()->logLine(string("ScreenShot Test - Created File: ") + fileName);
}

void ScreenShotTest::configureCamera(SphereGLCameraHandler *cam)
{
  if(m_frameCounter < 0 || m_frameCounter%SCREEN_TEST_INTERSCREEN_FRAMES != 0)
    return;
  cam->setSphereAlpha(30);
  cam->setSphereBeta(SCREEN_TEST_CAM_ROT_ANGLE*m_frameCounter/SCREEN_TEST_INTERSCREEN_FRAMES);
}

void ScreenShotTest::update()
{
  m_frameCounter++;
}

bool ScreenShotTest::isTestEnded()
{
  return SCREEN_TEST_CAM_ROT_ANGLE*m_frameCounter/SCREEN_TEST_INTERSCREEN_FRAMES >= 360;
}
#endif


