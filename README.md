## Dynamic Linear Allocator (a.k.a. Scratch Buffer)

### you can run the help.sh script for an interactive and plain way to get the info you want


This is a custom allocator. Implementation found in linear_allocator.hxx.
It's meant to be a lightweight, raw, and flexible way to handle small to medium-sized buffers
without constantly relying on new/delete calls when working with dynamic or changing data.

It tries to stay predictable and memory-safe within reasonable limits, while keeping overhead low.

The header file contains both declarations and definitions since it's a templated class.
Meaning it can work with any data type. It's mainly meant for working with characters,
but you can easily adapt it for other types too.

To use it, just include the header in your C++ file and construct the allocator with your desired size and type.
Some typedefs are also provided for easier setup and use.

After construction a buffer is available to use.
# Writing into the buffer
- **append**:
Use append to write data into the buffer.
Pass in a pointer to the source you want to append and amount to append
Append works by moving a cursor inside the buffer and writing to it
If the size to append will surpass the buffer size, i.e cursor + data_size > buffer size,
the buffer will double in size, move the data in new buffer and continue appending


# Interacting with the buffer
The buffer itself is private by default, but four functions are available for convenience:
 - **borrow_buffer**:
returns an IMMUTABLE reference to the buffer pointer (T*&).
The data in the buffer is also IMMUTABLE.
Use this when you just need to read from the start up to a certain point.
Be careful not to go out of bounds — use get_max_capacity() or get_current_capacity() to check the safe readable range.

- **get_buffer**:
returns a raw reference to the buffer pointer (T*&).
This is NOT memory safe. If used incorrectly, it *will* modify the buffer and can go out of bounds.
Use with caution and only if you know what you're doing.

- **borrow_current_ptr**:
returns an IMMUTABLE reference to the current ptr(the cursor) inside buffer (T*).
The data in the buffer is also IMMUTABLE.
Use this when you just need to read from the start up to a certain point.
Be careful not to go out of bounds — use get_max_capacity() or get_current_capacity() to check the safe readable range.

__IMPORTANT NOTE__: The pointer returned by this function WILL NOT ALWAYSBE SAFE unlike get_buffer/borrow_buffer since,
This function just returns a pointer inside the current buffer if the buffer grows or shrinks THE POINTER WILL BE INVALID
Use validate_ptr() to check if the pointer belongs to the buffer

 - **get_current_ptr**:
returns a raw reference to the current pointer (the cursor) inside buffer (T*).
This is NOT memory safe. If used incorrectly, it *will* modify the buffer and can go out of bounds.
Use with caution and only if you know what you're doing.

__IMPORTANT NOTE__: The pointer returned by this function WILL NOT ALWAYSBE SAFE unlike get_buffer/borrow_buffer since,
This function just returns a pointer inside the current buffer if the buffer grows or shrinks THE POINTER WILL BE INVALID
Use validate_ptr() to check if the pointer belongs to the buffer

____NOTE____: The pointer to the buffer is always valid — you're getting a reference to the pointer itself,
not the buffer. So you don’t have to worry about dangling pointers when resizing, old pointers are still valid thou.
This also means you can safely resize and control the buffer without your references breaking.

# Reallocating
- **realloc**:
Use this function to resize the buffer to your needs.
<<<<<<< HEAD
__IMPORTANT NOTE__: All pointers within the buffer will become unsafe or unaccessable since it resets the cursor and creates a new buffer
=======
<<<<<<< HEAD
__IMPORTANT NOTE__: All pointers within the buffer will become unsafe or unaccessable since it resets the cursor and creates a new buffer
=======
__IMPORTANT NOTE__: All inside the buffer and pointer within the buffer will become unsafe and unaccessable
>>>>>>> 419923d (Fixed append bug where it didn't grow and safely copy data if append size was double the buffer size.)
>>>>>>> ed3cfcb (Fixed issue where buffer wouldnt grow and safe data properly when appending data larger than twice buffer size)

# Copying info from the buffer
The following functions provide a way to get copies of data
- **clone_buffer**:
Returns a pointer to an UNOWNED HEAP copy of the entire buffer.
Be sure to use delete[] on the pointer when you done
__NOTE__: This will return garbage data if you have written less into the buffer than its capacity

 - **clone_til_cursor**:
Returns a pointer to an UNOWNED HEAP copy of the buffer from start untill where the cursor is.
Unlike clone_buffer this is less likely to return old data or uninitilised garbage as long as you understand what you doing,
However seeking before calling will effect returned data
Be sure to use delete[] on the pointer when you done

 - **clone_segment**:
Returns a pointer to an UNOWNED HEAP copy of size segment_size starting from buffer start.
similar to clone_till_cursor but can copy less or more than where the cursor it. curosor independant
If the segment_size is larger than the buffers capacity it will safely return the whole buffer and print a warning with info
__NOTE__: this will also return old/uninitilsed/garbage if you arent carefull with your logic
Be sure to use delete[] on the pointer when you done

# Seeking:
The following two functions are available to change the cursor:
- **absolute_seek**:
Simple function that sets the cursor to the number passed in, i.e 0 will set cursor back to start, 10 to buffer start + 10, and so on
__NOTE__: This is a size_t parameter therefore negative numbers will result in undefined behaviour.
If you attempt to seek beyond the buffers limit, the cursor will not change and a warning is printed with info

- **relative_seek** :
Simple function that move the cursor from where it is to +/- number passed in, i.e -1 will move cursor back 1 position
__NOTE__: This is a ssize_t parameter so the only limitation would be if your buffer is larger than the negative or positive
signable range you would need to do multiple moves to get to a position in it
If you attempt to seek out of the buffers limit, the cursor will not change and a warning is printed with info

# Tools:
- **validate_ptr**:
A saftey check bool pass in your pointer from the buffer and itll return true if the pointer is valid for the buffer, false otherwise

### Addition info
The DEFAULT move and copy constructors have been deleted to prevent memory issues and race conditions,
however if you require them, custom ones can be easily implemented

Functions that can run into errors do not quit or throw exceptions instead a warning is printed out to std err stream with infomation on
how the function failed and what was done to handle the error. by defualt the warnings are enabled but you can silence them using -DDONT_WARN when compiling.
