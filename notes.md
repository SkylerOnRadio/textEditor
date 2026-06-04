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

- Trying to figure out how to detect CTRL, i found `#define CTRL(key) (key & 037)` i didn't know what that was and tried to find out, but google searches gave me nothing. So I used AI to figure out what it did
  - It said its a macro to convert and key press to the CTRL+key version of it
  - 031 is octal system, because in C using 0 to start a number means it is octal, it is equivalent to 0x1F and 0001 1111
  - & is a bitwise AND operator, takes two equal sized binary numbers and returns the and of each individual bit
  - So it returns the AND of the key and 0001 1111, why??
  - Acts as a filter since the first 0's forces the first 3 bits of the operation to be 0 and the rest of the bits are dependent on the key.
  - So CTRL+key characters are the same as when we do this operation, this is because of how the letters ASCII are set
    - Bits 1 to 5 are for determining the letter pressed
    - Bits 6 & 7 are used to identify the type of character that has been pressed

      Bit 6 Bit 7 Type of Character
      0 0 Control Character
      0 1 Number & Punctuation
      1 0 Uppercase
      1 1 Lowercase

    - So MSB----->0101 0101<-----LSB, the MSB is the Bit 7, so by making the first three characters 0, we artificially turn them into a control character

> Research more on this for the video for a better explanation.

- Tried to use `KEY_BACKSPACE` but the switch statement is not working for it so I just ended up using ASCII number, I then added the enter key too so now the text editor goes to the new line after pressing enter, I then thought about having to move the cursor on pressing the key buttons, which made me realize that my current method will not really do well. Since the letter are stored as a linked list, the insertion and deletion happen at O(1) time complexity only if the letter is at the start of the end, but will take a lot of time if we write in between the first and last node. So editing in the middle for large texts will be very difficult. In addition to that, the function to add the letter currently is what sets the pointer, and the cursor is dependent on it so we will have to change the cursor to be independent of that function.
