// =============================================================================
// Array Object (Definitions)
// Programmed by Francois Lamini
// =============================================================================

#include <iostream>
#include <string>

namespace Codeloader {

  template <typename T> class cArray {

    public:
      enum Settings {
        BLOCK_SIZE = 256,
        TABLE_INCR = 10
      };

      T** items;
      int pointer;
      int block_count;
      int table_size;

      cArray();
      cArray(const cArray& other);
      cArray(cArray& other);
      ~cArray();
      cArray& operator= (const cArray& other);
      cArray& operator= (cArray& other);
      T& operator[] (int index);
      void Add(T item);
      T& Add();
      T Remove(int index);
      void Push(T item);
      T Pop();
      void Unshift(T item);
      T Shift();
      void Insert(T item, int index);
      int Count();
      void Alloc_Block();
      void Copy(cArray& other);
      void Print();
      void Init();
      void Reallocate_Table();
      void Free();
      int Get_Size();
      void Clear();
      T& Peek_Front();
      T& Peek_Back();

  };

  void Check_Condition(bool condition, std::string message);

}
