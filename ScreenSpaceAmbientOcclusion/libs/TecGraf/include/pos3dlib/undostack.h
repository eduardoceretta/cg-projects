#ifndef _LIMITED_STACK_H_
#define _LIMITED_STACK_H_

#include <pos3dlib/defines.h>

template <class TYPE>
class POS3DLIB_API LimitedStack
{
public:
  LimitedStack (int max_depth)
    :m_undo_stack_top(0),
    m_undo_stack_size(0),
    m_max_depth(max_depth)
  {
    m_undo_stack = new TYPE[max_depth];
  }

  ~LimitedStack ()
  {
    delete[] m_undo_stack;
  }

  //Insert at top 
  //if stack is full overwrites the last
  void Push (TYPE t)
  {
    m_undo_stack[m_undo_stack_top] = t;
    m_undo_stack_top = (m_undo_stack_top + 1) % m_max_depth;
    m_undo_stack_size = (m_undo_stack_size < m_max_depth ) ? m_undo_stack_size + 1 : m_max_depth;
  }
  
  //Removes the top and returns it
  TYPE Pop ()
  {
    assert (m_undo_stack_size > 0);
    m_undo_stack_top = (m_undo_stack_top > 0) ? m_undo_stack_top - 1 : m_max_depth - 1;
    m_undo_stack_size--;

    TYPE t = m_undo_stack[m_undo_stack_top];
    return t;
  }

  int Size () const
  {
    return m_undo_stack_size;
  }

  void Clear()
  {
    m_undo_stack_size = 0;
    m_undo_stack_top = 0;
  }
private:
  TYPE* m_undo_stack;
  int m_undo_stack_size;
  int m_undo_stack_top;
  int m_max_depth;
};

#endif