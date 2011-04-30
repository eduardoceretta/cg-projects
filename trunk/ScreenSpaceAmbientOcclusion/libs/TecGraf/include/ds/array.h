// Dynamic array
// celes@tecgraf.puc-rio.br
// Jul 2002

//TODO: AppendRaw()

#ifndef ds_array_h
#define ds_array_h

#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "defines.h"

template <class TYPE> 
class DSAPI DsArray
{
protected:
  int    d_nmax;
  int    d_n;
  float  d_incr;
  TYPE*  d_v;
  char*  d_name;
public:
  DsArray (float incr=2.0F) 
    : d_nmax(0), d_n(0), d_incr(incr), d_v(0)
  {
    d_name = 0;
  }
  virtual ~DsArray()
  {
    Empty();
    free(d_name);
  }
  void Empty ()
  {
    for (int i=0; i<d_nmax; ++i) 
      d_v[i].~TYPE();
    d_n = d_nmax = 0;
    free(d_v);
    d_v = 0;
  }
  void SetName (char* name)
  {
    d_name = (char*)realloc(d_name,strlen(name)+1);
    strcpy(d_name,name);
  }
  TYPE& operator[] (int i)
  {
#ifdef _DEBUG
    if (i >= d_n) {
      printf("DS: operator[] old semantics is deprecated: [] cannot be used without "
             "a previous Resize to fit the given index. (given index %d, size %d)\n",
             i,
             d_n);
      assert(0);
    }
#endif
    return d_v[i];
  }
  const TYPE& operator[] (int i) const
  {
    return d_v[i];
  }
  TYPE Get (int i) const
  {
    assert(ValidIndex(i));
    return d_v[i];
  }
  TYPE& GetRef (int i) const
  {
    assert(ValidIndex(i));
    return d_v[i];
  }
  /**
   * Obtains the address of item in position 'i'.
   */
  TYPE* GetAddress (int i) const
  {
    assert(ValidIndex(i));
    return d_v+i;
  }
  /**
   * Obtains the address of item in position 'i' (read-only).
   */
  const TYPE* GetAddressConst (int i) const
  {
    assert(ValidIndex(i));
    return d_v+i;
  }
  void Set (int i,TYPE& t)
  {
    assert(ValidIndex(i));
    d_v[i] = t;
  }
  void SetConst (int i,TYPE t)
  {
    assert(ValidIndex(i));
    d_v[i] = t;
  }
  /**
   * Appends an element to the end of the array.
   */
  void Append (const TYPE& t)
  {
    Resize(d_n+1);
    d_v[d_n-1] = t;
  }
  // Appends n elements of TYPE.
  void Append (int n, const TYPE* tv)
  {
    Resize(d_n+n);
    TYPE* v = d_v+d_n-n;
    for (int i=0; i<n; i++)
      *v++ = tv[i];
  }
  /**
   * Inserts element 't' at position 'pos'.
   * If 'pos' is negative, inserts at the end of the list.
   */
  void Insert (const TYPE& t, int pos=-1)
  {
    Resize(d_n+1);
    if (pos < 0 || pos >= d_n-1)
      d_v[d_n-1] = t;
    else
    {
      for (int i=d_n-2; i>=pos; i--)
        d_v[i+1] = d_v[i];
      d_v[pos] = t;
    }
  }
  // Removes entry at the given index. 
  void Remove (int i)
  {
    d_n--;
    if (i < d_n)
      memmove(d_v+i, d_v+i+1, (d_n-i)*sizeof(TYPE));
  }
  void Invert ()
  {
    for (int i=0, j=d_n-1; i<j; i++, j--)
    {
      TYPE t = d_v[i];
      d_v[i] = d_v[j];
      d_v[j] = t;
    }
  }
  TYPE* GetArray ()
  {
    return d_v;
  }
  const TYPE* GetArrayConst () const
  {
    return d_v;
  }
  int Size (void) const
  { 
    return d_n; 
  }
  /**
   * Returns the current allocated number of elements.
   */
  int AllocatedSize () const
  { 
    return d_nmax; 
  }
  /**
   * Obtains the current number of bytes used by the array.
   */
  size_t GetNumBytes () const
  {
    return Size()*sizeof(TYPE);
  }
  /**
   * Obtains the current number of bytes allocated by the array.
   */
  size_t GetNumAllocatedBytes () const
  {
    return AllocatedSize()*sizeof(TYPE);
  }
  /**
   * Returns 'true' if 'i' is a valid index
   * for 'Get'. (i between 0 and size-1).
   */
  bool ValidIndex (int i) const
  {
    return 0 <= i && i < Size();
  }
  /**
   * Resizes array to hold 'n' elements.
   * If 'exact_size' is true, assumes Append() won't be called,
   * so the array is resized to exactly hold 'n' elements.
   * If 'exact_size' is false, assumes Append() might be called
   * afterwards, so the array is grown using the given resize increment.
   */
  void Resize (int n, bool exact_size=false)
  {
    Reserve(n, exact_size);
    d_n = n;
  }
  /**
   * Resizes array and sets all new elements to value 'v'.
   * If 'exact_size' is true, assumes Append() won't be called,
   * so the array is resized to exactly hold 'n' elements.
   * If 'exact_size' is false, assumes Append() might be called
   * afterwards, so the array is grown using the given resize increment.
   */
  void ResizeAndSet (int n, TYPE& v, bool exact_size=false)
  {
    int oldsize = Size();
    Resize(n, exact_size);
    for (int i=oldsize; i<n; ++i)
      Set(i, v);
  }
  /**
   * Resizes array and sets all new elements to value 'v'.
   * If 'exact_size' is true, assumes Append() won't be called,
   * so the array is resized to exactly hold 'n' elements.
   * If 'exact_size' is false, assumes Append() might be called
   * afterwards, so the array is grown using the given resize increment.
   */
  void ResizeAndSetConst (int n, TYPE v, bool exact_size=false)
  {
    int oldsize = Size();
    Resize(n, exact_size);
    for (int i=oldsize; i<n; ++i)
      SetConst(i, v);
  }
  /**
   * Resizes array and fills the space filled by new elements with
   * zeroes, using memset for it.
   * If 'exact_size' is true, assumes Append() won't be called,
   * so the array is resized to exactly hold 'n' elements.
   * If 'exact_size' is false, assumes Append() might be called
   * afterwards, so the array is grown using the given resize increment.
   */
  void ResizeWithZeros (int n, bool exact_size=false)
  {
    int oldsize = Size();
    Resize(n, exact_size);
    if (n > oldsize)
      memset(GetAddress(oldsize), 0, (n - oldsize)*sizeof(TYPE));
  }
  /**
   * Packs the internal array to fit the current number of elements
   * exactly.
   */
  void Pack ()
  {
    if (d_n == 0)
      return;
    // destroy unused elements
    for (int i=d_n; i<d_nmax; ++i) 
      d_v[i].~TYPE();
    // realloc to fit
    d_v = (TYPE*) realloc(d_v,d_n*sizeof(TYPE));
    d_nmax = d_n;
  }
  void Reserve (int n, bool exact_size=false)
  {
    if (n>d_nmax)
    {
      int prev_nmax = d_nmax;
#if 0
      if (d_name)
        printf("DS: realloc(%s,%d->",d_name,d_nmax);
#endif
      if (exact_size)
        d_nmax = n;
      else {
        do
        {
          int new_nmax = d_nmax ? (int)(d_incr*d_nmax) : 2;
          if (new_nmax == d_nmax)
            d_nmax = new_nmax+1;
          else
            d_nmax = new_nmax;
        } while (n>d_nmax);
      }
#if 0
      if (d_name)
        printf("%d)\n",d_nmax);
#endif
      d_v = (TYPE*) realloc(d_v,d_nmax*sizeof(TYPE));
      for (int i=prev_nmax; i<d_nmax; ++i)
        new(&d_v[i]) TYPE; /* Call constructor for new objects */
    }
    if (n>d_n) d_n = n;
  }
  /**
   * This method can be called in case a client wants to take over the built array.
   * The object state will be as if Empty() was called.
   * The returned array must be freed with DestroyTakenOverArray().
   */
  TYPE* TakeOverArray ()
  {
    d_n = d_nmax = 0;
    TYPE* ret = d_v;
    d_v = NULL;
    return ret;
  }
  /**
   * Releases resources of an array that was taken over via TakeOverArray().
   */
  static void DestroyTakenOverArray (TYPE* buffer, int size)
  {
    for (int i=0; i<size; ++i) 
      buffer[i].~TYPE();
    free(buffer);
  }
};

#endif

