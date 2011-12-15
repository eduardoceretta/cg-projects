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
#include "app.h"
/*******************************************/
/* TestLogger                              */
/*******************************************/
TestLogger * TestLogger::s_inst(NULL);
string TestLogger::s_fileName("");

TestLogger* TestLogger::inst()
{
  if(!s_inst)
  {
    s_inst = new TestLogger();
#ifdef LOG_TESTS
    s_inst->init();
#endif
  }
  return s_inst;
}


TestLogger::~TestLogger()
{
}

void TestLogger::closeLog()
{
#ifdef LOG_TESTS
  if(m_fp)
  {
    logLine("}");
    fclose(m_fp);
    m_fp = NULL;
  }
#endif // LOG_TESTS

  if(s_inst)
    delete s_inst;
  s_inst = NULL;
  s_fileName = "";
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

  string fileName = "";
  if(s_fileName == "")
  {
    char date[100];
    sprintf(date, "_%d_%d_%d-%d_%d_%d-%ld", ttm->tm_year + 1900, ttm->tm_mon + 1, ttm->tm_mday,
      ttm->tm_hour, ttm->tm_min, ttm->tm_sec, clock());
    fileName = string(LOG_TEST_PATH) + string("log_") + string(date) + ".log";
  }else fileName = s_fileName;

  m_fp = fopen(fileName.c_str(), "w");
  
  //log(string("SSAO LogFile. - Created: ") + string(asctime(ttm)) + string("\n\n"));
  logLine("return {");
  logLine(string("[\"program_name\"] = [[") + string(APP_NAME) + string("]],"));
  char *tmp = asctime(ttm);
  tmp[strlen(tmp)-1] = '\0';
  logLine(string("[\"creation_date\"] = [[") + string(tmp) + string("]],"));
}

void TestLogger::setFileName( string fileName )
{
  if(!s_inst)
    s_fileName = fileName;
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
  kVoxelizationTime = kConeTracingTime = 0.0;

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
  //str+=tmp; sprintf(tmp, "\n-----------TIMER TEST-----------------\n"); 
  //str+=tmp; sprintf(tmp, "Num Frames  Tested:%d\n", framesCounter);
  //str+=tmp; sprintf(tmp, "Total         Time:%lf sec\n", totalTime/framesCounter); 
  //str+=tmp; sprintf(tmp, "KernelColor   Time:%lf sec - %.2f ", kColorTime/framesCounter, 100. * kColorTime / totalTime);
  //str+=tmp; sprintf(tmp, "KernelDefPeel Time:%lf sec - %.2f ", kDeferedPeelingTime/framesCounter, 100. * kDeferedPeelingTime / totalTime); str+= "%%\n";
  //str+=tmp; sprintf(tmp, "KernelSSAO    Time:%lf sec - %.2f ", kSSAOTime/framesCounter, 100. * kSSAOTime / totalTime); str+= "%%\n";
  //str+=tmp; sprintf(tmp, "KernelBlur    Time:%lf sec - %.2f ", kBlurTime/framesCounter, 100. * kBlurTime / totalTime); str+= "%%\n";
  //str+=tmp; sprintf(tmp, "KernelCombine Time:%lf sec - %.2f ", kCombineTime/framesCounter, 100. * kCombineTime / totalTime); str+= "%%\n";
  //str+=tmp; sprintf(tmp, "Voxelization Time:%lf sec - %.2f", kVoxelizationTime/framesCounter, 100. * kVoxelizationTime / totalTime); 
  //str+=tmp; sprintf(tmp, "KernelConeTracing Time:%lf sec - %.2f", kConeTracingTime/framesCounter, 100. * kConeTracingTime / totalTime); str+= "%%\n";
  //str+=tmp; sprintf(tmp, "--------------------------------------\n\n"); str+= "%%\n";
  //str+=tmp;

  str+=tmp; sprintf(tmp, "[\"time_tests\"] = {\n"); 
  str+=tmp; sprintf(tmp, "  [\"frames_tested\"] = %d,\n", framesCounter); 
  str+=tmp; sprintf(tmp, "  [\"total_time\"] = %lf, --(sec)\n", totalTime/framesCounter); 
  str+=tmp; sprintf(tmp, "  [\"voxelization_time\"] = %lf, --(sec)\n", kVoxelizationTime/framesCounter); 
  str+=tmp; sprintf(tmp, "  [\"voxelization_percent\"] = %.2f, --(%%)\n", 100. * kVoxelizationTime / totalTime); 
  str+=tmp; sprintf(tmp, "  [\"conetracing_time\"] = %lf, --(sec)\n", kConeTracingTime/framesCounter); 
  str+=tmp; sprintf(tmp, "  [\"conetracing_percent\"] = %.2f, --(%%)\n", 100. * kConeTracingTime / totalTime); 
  str+=tmp; sprintf(tmp, "},\n"); 
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

void ScreenShotTest::save(GLint texid, string path /*= ""*/,  string type /*= ""*/)
{
  if(m_frameCounter < 0 || m_frameCounter%SCREEN_TEST_INTERSCREEN_FRAMES != 0)
    return;
  m_texDebug->setId(texid);

  time_t rawtime;
  time(&rawtime);

  struct tm *ttm = localtime(&rawtime);
  char date[100];
  sprintf(date, "%d_%d_%d-%d_%d_%d-%ld_", ttm->tm_year + 1900, ttm->tm_mon + 1, ttm->tm_mday,
                                         ttm->tm_hour, ttm->tm_min, ttm->tm_sec, clock());
  string fileName = string("screen_")+ string(date) + type + ".png";
  m_texDebug->write2DToFile(path, fileName);
  
  //TestLogger::inst()->logLine(string("ScreenShot Test - Created File: ") + fileName);
  TestLogger::inst()->logLine(string("  [[") + fileName + string("]],"));
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

void ScreenShotTest::reset()
{
  m_frameCounter = -SCREEN_TEST_INTERSCREEN_FRAMES;
}


bool ScreenShotTest::isOver()
{
  return SCREEN_TEST_CAM_ROT_ANGLE*m_frameCounter/SCREEN_TEST_INTERSCREEN_FRAMES >= 360;
}
#endif


