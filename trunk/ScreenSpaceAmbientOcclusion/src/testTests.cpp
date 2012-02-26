/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011
 *
 *  Tests the application
 */
#include <iostream>
#include <GL\glew.h>
#include <GL\glut.h>
#include <cmath>
#include <ctime>


#include "testTests.h"
#include "testApp.h"
#include "defines.h"
/*******************************************/
/* TestLogger                              */
/*******************************************/
TestLogger * TestLogger::s_inst(NULL);
string TestLogger::s_fileName("");
string TestLogger::s_path("");

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
    cout << "Log File Saved at: " << s_path  << s_fileName << endl;
  }
#endif // LOG_TESTS

  if(s_inst)
    delete s_inst;
  s_inst = NULL;
  s_fileName = "";
  s_path = "";
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

  if(s_fileName == "")
  {
    char date[100];
    sprintf(date, "_%d_%d_%d-%d_%d_%d-%ld", ttm->tm_year + 1900, ttm->tm_mon + 1, ttm->tm_mday,
      ttm->tm_hour, ttm->tm_min, ttm->tm_sec, clock());
    s_fileName = string("log_") + string(date) + ".log";
  }

  string fileName = s_path + s_fileName;
  m_fp = fopen(fileName.c_str(), "w");
  MyAssert("Log File Name Not Found: "+fileName, m_fp);
  
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

void TestLogger::setPath( string path )
{
  if(!s_inst)
    s_path = path;
}


/*******************************************/
/* TimeTest                                */
/*******************************************/
#ifdef TIME_TEST
TimeTest::TimeTest()
:m_singleGPUMeasuredTotalTime(-1.0)
//,m_singleCPUMeasuredTotalTime(-1.0)
,m_meanGPUMeasuredTotalTime(-1.0)
//,m_meanCPUMeasuredTotalTime(-1.0)
{
  m_glTimer.init();
}

void TimeTest::clearProfiler()
{
  m_singleGPUMeasuredTotalTime = (-1.0);
  //m_singleCPUMeasuredTotalTime = (-1.0);
  m_meanGPUMeasuredTotalTime = (-1.0);
  //m_meanCPUMeasuredTotalTime = (-1.0);
  m_steps.clear();
  //m_timer.reset();
  m_glTimer.reset();
  m_glTimer.init();
}

void TimeTest::pushBackStep(string name, void(*func)(TestApp*), TestApp * arg)
{
  struct Step step = {
    name, func, arg, -1.0, -1.0//, -1.0, -1.0
  };
  m_steps.push_back(step);
}

void TimeTest::timeProfile()
{
  vector<Step> :: iterator it;
  /************************/
  /*  STEP TIME           */
  /************************/
  if(m_steps.size() > 1)
    for(it = m_steps.begin(); it != m_steps.end(); ++it)
    {
      //Single
      //Single
      m_glTimer.start();
        it->func(it->arg);
      m_glTimer.stop();

      //m_timer.reset();
      //  it->func(it->arg);
      //it->singleCPUMeasuredTime = m_timer.getTimeInMilliseconds();
      it->singleGPUMeasuredTime = m_glTimer.getElapsedTimeInMilliseconds();

      //Mean
      //Mean
      m_glTimer.start();
      for(int i = 0; i < MEAN_NUMBER; ++i)
        it->func(it->arg);
      m_glTimer.stop();

      //m_timer.reset();
      //for(int i = 0; i < MEAN_NUMBER; ++i)
      //  it->func(it->arg);
      //it->meanCPUMeasuredTime = m_timer.getTimeInMilliseconds() / MEAN_NUMBER;
      it->meanGPUMeasuredTime = m_glTimer.getElapsedTimeInMilliseconds() / MEAN_NUMBER;
    }
  /************************/
  /*  SNIGLE TOTAL TIME   */
  /************************/
  m_glTimer.start();
  for(it = m_steps.begin(); it != m_steps.end(); ++it)
    it->func(it->arg);
  m_glTimer.stop();

  //m_timer.reset();
  //for(it = m_steps.begin(); it != m_steps.end(); ++it)
  //  it->func(it->arg);
  //m_singleCPUMeasuredTotalTime = m_timer.getTimeInMilliseconds();
  m_singleGPUMeasuredTotalTime = m_glTimer.getElapsedTimeInMilliseconds();

  /************************/
  /*  MEAN TOTAL TIME     */
  /************************/
  m_glTimer.start();
  for(it = m_steps.begin(); it != m_steps.end(); ++it)
    for(int i = 0; i < MEAN_NUMBER; ++i)
      it->func(it->arg);
  m_glTimer.stop();

  //m_timer.reset();
  //for(it = m_steps.begin(); it != m_steps.end(); ++it)
  //  for(int i = 0; i < MEAN_NUMBER; ++i)
  //    it->func(it->arg);
  //m_meanCPUMeasuredTotalTime = m_timer.getTimeInMilliseconds() / MEAN_NUMBER;
  m_meanGPUMeasuredTotalTime = m_glTimer.getElapsedTimeInMilliseconds()/ MEAN_NUMBER;
}

string TimeTest::getFormatedResult()
{
  string str = "";
  char tmp[500] = "";
  
  str+=tmp; sprintf(tmp, "      [\"total\"] = {\n");
  str+=tmp; sprintf(tmp, "        [\"GPU\"] = {\n");   
  str+=tmp; sprintf(tmp, "          [\"Single\"] = %Lf,\n", m_singleGPUMeasuredTotalTime);   
  str+=tmp; sprintf(tmp, "          [\"Mean\"] = %Lf,\n", m_meanGPUMeasuredTotalTime);   
  str+=tmp; sprintf(tmp, "        },\n"); 
  //str+=tmp; sprintf(tmp, "        [\"CPU\"] = {\n");   
  //str+=tmp; sprintf(tmp, "          [\"Single\"] = %Lf,\n", m_singleCPUMeasuredTotalTime);   
  //str+=tmp; sprintf(tmp, "          [\"Mean\"] = %Lf,\n", m_meanCPUMeasuredTotalTime);   
  //str+=tmp; sprintf(tmp, "        },\n"); 
  str+=tmp; sprintf(tmp, "      },\n"); 

  if(m_steps.size() > 1)
  {
    str+=tmp; sprintf(tmp, "      [\"steps\"] = {\n"); 
    vector<Step> :: iterator it;
    for(it = m_steps.begin(); it != m_steps.end(); ++it)
    {
      str+=tmp; sprintf(tmp, "        [\"%s\"] = {\n", it->name.c_str());   
      str+=tmp; sprintf(tmp, "          [\"GPU\"] = {\n");   
      str+=tmp; sprintf(tmp, "            [\"Single\"] = %Lf,\n", it->singleGPUMeasuredTime);   
      str+=tmp; sprintf(tmp, "            [\"Mean\"] = %Lf,\n", it->meanGPUMeasuredTime);   
      str+=tmp; sprintf(tmp, "          },\n"); 
      //str+=tmp; sprintf(tmp, "          [\"CPU\"] = {\n");   
      //str+=tmp; sprintf(tmp, "            [\"Single\"] = %Lf,\n", it->singleCPUMeasuredTime);   
      //str+=tmp; sprintf(tmp, "            [\"Mean\"] = %Lf,\n", it->meanCPUMeasuredTime);   
      //str+=tmp; sprintf(tmp, "          },\n"); 
      str+=tmp; sprintf(tmp, "        },\n"); 
    }
    str+=tmp; sprintf(tmp, "      },\n"); 
  }

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
{
}

ScreenShotTest::~ScreenShotTest()
{
  delete m_texDebug;
}

string ScreenShotTest::save(void (*func)(TestApp *), TestApp * argument, GLint texid, string path, string fileName)
{
  if(!func)
      return "";

  func(argument);

  if(fileName == "")
  {
    time_t rawtime;
    time(&rawtime);

    struct tm *ttm = localtime(&rawtime);
    char date[100];
    sprintf(date, "%d_%d_%d-%d_%d_%d-%ld_", ttm->tm_year + 1900, ttm->tm_mon + 1, ttm->tm_mday,
                                           ttm->tm_hour, ttm->tm_min, ttm->tm_sec, clock());
    fileName = string("screen_") + string(date);
  }
  fileName += ".png";

  m_texDebug->setId(texid);
  m_texDebug->write2DToFile(path, fileName);

  return fileName;
}
#endif


