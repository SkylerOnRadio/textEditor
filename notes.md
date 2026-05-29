# Notes on the project detailing the difficulties and aims and processes I used

## Difficulties

- Using unique pointer in the linked list

---

## Aims

- Can write and read from the editor
- Menu screen
- Undo/Redo
- Text wrapping toggle
- Shortcuts
- Vim modes

## Falters

- Used AI to learn how to use the unique pointer in a linked list
  - Unique pointers are only needed when we need a pointer to own the memory
  - If there is a concept of ownership here then how is this one different from the ownership in Rust
  - you can use auto while using `std::make_unique<Type>` but I chose not to use it cause it kinda confused me, thinking there was a reason to use it. I write it for clearer code
  - We don't use weak pointer for end because
    i) we can't since weak pointer needs a shared pointer
    ii) raw pointers to point to an object that will exist while the pointer is being used is fine and standard
  - C++ had it so that to transfer object from one variable to another you had to make a copy of the object and destroy the original object. Obviously this is overhead and unnecesary so `std::move` was made. It changes the ownership of the memory in the heap in our context i think.

- Forgot to implement a way to get out of the app, since using raw disables CTRL+C
  - made the exit be done by ctrl+q
