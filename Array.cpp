// =============================================================================
// Array Object (Implementation)
// Programmed by Francois Lamini
// =============================================================================

#include "Array.h"

class cObject {

  public:
    std::string name;

    cObject();
    cObject(std::string name);
    ~cObject();

};

Codeloader::cArray<cObject> Dummy_Routine(Codeloader::cArray<cObject>& array);

// **************************************************************************
// Program Entry Point
// **************************************************************************

int main(int argc, char** argv) {
  try {
    Codeloader::cArray<cObject> array;
    array.Add().name = "First";
    array.Add().name = "Second";
    array.Add().name = "Third";
    Codeloader::cArray<cObject> array_2;
    array_2.Add().name = "Another";
    Codeloader::cArray<cObject> new_array = Dummy_Routine(array_2);
  }
  catch (std::string error) {
    std::cout << error << std::endl;
  }
  std::cout << "Done." << std::endl;
  return 0;
}

// **************************************************************************
// Test Routine
// **************************************************************************

Codeloader::cArray<cObject> Dummy_Routine(Codeloader::cArray<cObject>& array) {
  Codeloader::cArray<cObject> new_array = array;
  new_array.Add().name = "Tenth";
  array.Remove(0);
  return new_array;
}

// **************************************************************************
// Test Object
// **************************************************************************

cObject::cObject() {
  // std::cout << "Default object constructor called." << std::endl;
}

cObject::cObject(std::string name) {
  this->name = name;
  std::cout << "Object constructor called for " << name << "." << std::endl;
}

cObject::~cObject() {
  if (this->name.length() > 0) {
    std::cout << "Object destructor called for " << this->name << "." << std::endl;
  }
}

namespace Codeloader {

  // **************************************************************************
  // Array Implementation
  // **************************************************************************

  /**
   * Creates a new array with the default size.
   */
  template <typename T> cArray<T>::cArray() {
    std::cout << "Default constructor called." << std::endl;
    this->Init();
  }

  /**
   * Copies another array to this one.
   * @param other The other array.
   */
  template <typename T> cArray<T>::cArray(cArray<T>& other) {
    std::cout << "Array copier called." << std::endl;
    this->Init();
    this->Copy(other);
  }

  /**
   * Assigns another array to this one.
   * @param other The other array.
   * @return This array.
   */
  template <typename T> cArray<T>& cArray<T>::operator= (cArray<T>& other) {
    std::cout << "Assignment operator called." << std::endl;
    this->Init();
    this->Copy(other);
    return *this;
  }

  /**
   * Frees the array.
   */
  template <typename T> cArray<T>::~cArray() {
    std::cout << "Array freed." << std::endl;
    this->Free();
  }

  /**
   * Adds an item to the array.
   * @param item The item.
   * @throws An error if we ran out of space.
   */
  template <typename T> void cArray<T>::Add(T item) {
    int size = this->Get_Size();
    if (this->pointer == size) { // Ran out of space?
      this->Alloc_Block();
    }
    (*this)[this->pointer++] = item;
  }

  /**
   * Extends the array by one item. The item is not initialized.
   * @return The reference to the item inside the array.
   */
  template <typename T> T& cArray<T>::Add() {
    int size = this->Get_Size();
    if (this->pointer == size) { // Ran out of space?
      this->Alloc_Block();
    }
    return (*this)[this->pointer++];
  }

  /**
   * Removes an item from the array.
   * @param index The index of the item to remove.
   * @return The item that was removed.
   */
  template <typename T> T cArray<T>::Remove(int index) {
    T item = (*this)[index];
    for (int item_index = index + 1; item_index < this->pointer; item_index++) {
      (*this)[item_index - 1] = (*this)[item_index];
    }
    this->pointer--;
    return item;
  }

  /**
   * Pushes an item onto the back of an array.
   * @param item The item to push.
   */
  template <typename T> void cArray<T>::Push(T item) {
    this->Add(item);
  }

  /**
   * Removes an item from the back of the array.
   * @return The item removed.
   */
  template <typename T> T cArray<T>::Pop() {
    return this->Remove(this->pointer - 1);
  }

  /**
   * Adds an item to the front of the array.
   * @param item The item to add.
   */
  template <typename T> void cArray<T>::Unshift(T item) {
    int size = this->Get_Size();
    if (this->pointer == size) { // Ran out of space?
      this->Alloc_Block();
    }
    // Shift everything over to make space.
    this->pointer++; // Increase size.
    for (int item_index = this->pointer - 1; item_index > 0; item_index--) {
      (*this)[item_index] = (*this)[item_index - 1];
    }
    (*this)[0] = item;
  }

  /**
   * Removes an item from the front of the array.
   * @return The item in the front of the array.
   */
  template <typename T> T cArray<T>::Shift() {
    return this->Remove(0);
  }

  /**
   * Inserts an item at the specified index into the array.
   * @param item The item to insert.
   * @param index The index to insert the item before.
   * @throws An error if there is no more space in the array or the index is invalid.
   */
  template <typename T> void cArray<T>::Insert(T item, int index) {
    int size = this->Get_Size();
    if (this->pointer == size) { // Ran out of space?
      this->Alloc_Block();
    }
    this->pointer++;
    for (int item_index = this->pointer - 1; item_index > index; item_index--) {
      (*this)[item_index] = (*this)[item_index - 1];
    }
    (*this)[index] = item;
  }

  /**
   * Gets the number of items in the array.
   * @return The number of items in the array.
   */
  template <typename T> int cArray<T>::Count() {
    return this->pointer;
  }

  /**
   * Allocates a new block and expands the table if necessary.
   */
  template <typename T> void cArray<T>::Alloc_Block() {
    this->Reallocate_Table();
    this->items[this->block_count++] = new T[BLOCK_SIZE];
  }

  /**
   * Reallocates the table to make room for more blocks if needed.
   */
  template <typename T> void cArray<T>::Reallocate_Table() {
    if (this->block_count == this->table_size) { // Not enough room?
      // Reallocate table.
      this->table_size += TABLE_INCR;
      T** table = new T* [this->table_size];
      // Copy entries to new table.
      for (int entry_index = 0; entry_index < this->block_count; entry_index++) {
        table[entry_index] = this->items[entry_index]; // Reassign block to new table.
      }
      if (this->items) {
        delete[] this->items; // Remove.
      }
      this->items = table; // Reassign.
    }
  }

  /**
   * Copies the internal array.
   * @param source The source array.
   */
  template <typename T> void cArray<T>::Copy(cArray<T>& other) {
    T** table = new T* [other.table_size];
    for (int entry_index = 0; entry_index < other.block_count; entry_index++) {
      table[entry_index] = new T[BLOCK_SIZE];
      for (int item_index = 0; item_index < BLOCK_SIZE; item_index++) {
        table[entry_index][item_index] = other.items[entry_index][item_index];
      }
    }
    this->Free();
    this->items = table;
    this->table_size = other.table_size;
    this->block_count = other.block_count;
    this->pointer = other.pointer;
  }

  /**
   * Accesses an item from the array.
   * @param index The index of the item.
   * @return The item.
   * @throws An error if the index is not valid.
   */
  template <typename T> T& cArray<T>::operator[] (int index) {
    Check_Condition(((index >= 0) && (index < this->pointer)), "Index is out of bounds.");
    int item_index = index % BLOCK_SIZE;
    int block_index = index / BLOCK_SIZE;
    return this->items[block_index][item_index];
  }

  /**
   * Prints out the array contents.
   */
  template <typename T> void cArray<T>::Print() {
    std::cout << "Array" << std::endl;
    for (int item_index = 0; item_index < this->pointer; item_index++) {
      // std::cout << "[" << item_index << "]=" << (*this)[item_index] << std::endl;
    }
  }

  /**
   * Initializes the array.
   */
  template <typename T> void cArray<T>::Init() {
    this->table_size = 0;
    this->block_count = 0;
    this->pointer = 0;
    this->items = NULL;
    this->Alloc_Block();
  }

  /**
   * Frees the array of blocks.
   */
  template <typename T> void cArray<T>::Free() {
    if (this->items) {
      for (int entry_index = 0; entry_index < this->block_count; entry_index++) {
        delete[] this->items[entry_index]; // Will call value destructor if object.
      }
      delete[] this->items;
    }
  }

  /**
   * Gets the max size of the array.
   * @return The size of the array in items.
   */
  template <typename T> int cArray<T>::Get_Size() {
    return (this->block_count * BLOCK_SIZE);
  }

  /**
   * Clears out the array.
   */
  template <typename T> void cArray<T>::Clear() {
    this->pointer = 0;
  }

  /**
   * Accesses an item from the front of the array.
   * @return The item on the front.
   */
  template <typename T> T& cArray<T>::Peek_Front() {
    return (*this)[0];
  }

  /**
   * Accesses an item from the back of the array.
   * @return The item on the back.
   */
  template <typename T> T& cArray<T>::Peek_Back() {
    return (*this)[this->pointer - 1];
  }

  // **************************************************************************
  // Utility Functions
  // **************************************************************************

  /**
   * Checks to see if a condition holds. If not throws an error.
   * @param condition The condition to test.
   * @param message The error message.
   * @throws An error if the condition is false.
   */
  void Check_Condition(bool condition, std::string message) {
    if (!condition) {
      throw std::string(message);
    }
  }

}
