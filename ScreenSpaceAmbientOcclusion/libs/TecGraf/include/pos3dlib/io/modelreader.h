/* *************************************************************
 *
 * Class:   modelreader.h
 * Created: 25/5/2006 - 12:09
 * Author:  malf
 * $Id: modelreader.h,v 1.2 2010/12/03 18:43:10 rodesp Exp $
 *
 * Description: 
 *
 * *************************************************************/

#ifndef P3D_MODEL_READER_H
#define P3D_MODEL_READER_H

#include <pos3dlib/defines.h>
#include "../model/model.h"

/**
    Listener for events generated during the reading.
*/
class POS3DLIB_API P3DModelReaderListener
{
public:
  virtual ~P3DModelReaderListener() {}

  /**
      Method invoked when a warning is reported during the reading.
  */
  virtual void WarningReported(const char* msg) {}

  /**
      Method invoked when the label of the current reading field
     has changed.
  */
  virtual bool ProgressLabelChanged(const char* msg) { return true; }

  /**
      Method invoked when the percentage ratio of the read data
    has changed.
  */
  virtual bool ProgressRatioChanged(double ratio) { return true; }
};


/***************************************
  class P3DModelReader
****************************************/
class POS3DLIB_API P3DModelReader
{
private:
  char m_error[2048];
  bool m_error_flag;
  P3DModelReaderListener* m_listener;

public:
  P3DModelReader();

  virtual ~P3DModelReader();

  /**
      Notifies the reader that an error has ocurred.
  */
  int SetError(const char* err);

  int SetErrorf(const char* format, ...);

  /**
      Gets last notified error.
  */
  char* GetLastError();

  /**
      Returns whether input connection is opened or not.
  */
  virtual bool IsOpen() =0;

  /**
      Opens input connection to start reading the file.
    @return If OK: 1, else 0.
  */
  virtual int Open() =0;

  /**
      Closes input connection.
    @return If OK: 1, else 0.
  */
  virtual int Close() =0;

  /**
      Reads input data to an empty model.
    @return If OK: 1, else 0.
  */
  virtual int Read() =0;

  void SetListener(P3DModelReaderListener* l);

  P3DModelReaderListener* GetListener();
};

#endif