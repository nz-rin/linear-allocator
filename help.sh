#!/usr/bin/env bash

if [[ -z $@ ]]; then
	echo "Welcome, use --help for more info"
	exit
fi

header(){
	echo "Dynamic Linear Allocator (a.k.a. Scratch Buffer)"
	echo ""
	echo "This is a custom allocator. Implementation found in linear_allocator.hxx."
	echo "It's meant to be a lightweight, raw, and flexible way to handle small to medium-sized buffers"
	echo "    without constantly relying on new/delete calls when working with dynamic or changing data."
	echo ""
	echo "It tries to stay predictable and memory-safe within reasonable limits, while keeping overhead low."
	echo ""
	echo "The header file contains both declarations and definitions since it's a templated class"
	echo "    — meaning it can work with any data type. It's mainly meant for working with characters,"
	echo "    but you can easily adapt it for other types too."
	echo ""
	echo "To use it, just include the header in your C++ file and construct the allocator with your desired size and type."
	echo "    Some typedefs are also provided for easier setup and use."
	echo ""
	echo "After construction a buffer is available to use."
	echo ""
}

append(){
	echo " > Use append to write data into the buffer."
	echo "   - append:"
	echo "    Pass in a pointer to the source you want to append and amount to append"
	echo "    Append works by moving a cursor inside the buffer and writing to it"
	echo "    If the size to append will surpass the buffer size, i.e cursor + data_size > buffer size,"
	echo "    the buffer will double in size, move the data in new buffer and continue appending"
	echo ""
}
interacting_with_buffer(){
	echo " > Interacting with the buffer"
	echo "   The buffer itself is private by default, but four functions are available for convenience:"
}
borrow_buffer(){
	echo "   - borrow_buffer:"
	echo "        returns an IMMUTABLE reference to the buffer pointer (T*&)."
	echo "        The data in the buffer is also IMMUTABLE."
	echo "        Use this when you just need to read from the start up to a certain point."
	echo "        Be careful not to go out of bounds — use get_max_capacity() or get_current_capacity() to check the safe readable range."
	echo ""
}
get_buffer(){
	echo "   - get_buffer:"
	echo "        returns a raw reference to the buffer pointer (T*&)."
	echo "        This is NOT memory safe. If used incorrectly, it WILL modify the buffer and can go out of bounds."
	echo "        Use with caution and only if you know what you're doing."
	echo ""
}
borrow_current_ptr(){
	echo "   - borrow_current_ptr:"
	echo "        returns an IMMUTABLE reference to the current ptr(the cursor) inside buffer (T*)."
	echo "        The data in the buffer is also IMMUTABLE."
	echo "        Use this when you just need to read from the start up to a certain point."
	echo "        Be careful not to go out of bounds — use get_max_capacity() or get_current_capacity() to check the safe readable range."
	echo ""
	echo "        IMPORTANT NOTE: The pointer returned by this function WILL NOT ALWAYS  BE SAFE unlike get_buffer/borrow_buffer since,"
	echo "                        This function just returns a pointer inside the current buffer if the buffer grows or shrinks THE POINTER WILL BE INVALID"
	echo "                        Use validate_ptr() to check if the pointer belongs to the buffer"
	echo ""
}
get_current_ptr(){
	echo "   - get_current_ptr:"
	echo "        returns a raw reference to the current pointer (the cursor) inside buffer (T*)."
	echo "        This is NOT memory safe. If used incorrectly, it WILL modify the buffer and can go out of bounds."
	echo "        Use with caution and only if you know what you're doing."
	echo ""
	echo "        IMPORTANT NOTE: The pointer returned by this function WILL NOT ALWAYS  BE SAFE unlike get_buffer/borrow_buffer since,"
	echo "                        This function just returns a pointer inside the current buffer if the buffer grows or shrinks THE POINTER WILL BE INVALID"
	echo "                        Use validate_ptr() to check if the pointer belongs to the buffer"
	echo ""
	echo "   NOTE: The pointer to the buffer is always valid — you're getting a reference to the pointer itself,"
	echo "        not the buffer. So you don’t have to worry about dangling pointers when resizing, old pointers are still valid thou."
	echo "        This also means you can safely resize and control the buffer without your references breaking."
}
realloc(){
	echo " > Reallocating"
	echo "   -realloc:"
	echo "       Use this function to resize the buffer to your needs."
	echo "       IMPORTANT NOTE: All inside the buffer and pointer within the buffer will become unsafe and unaccessable"
}
copy_header(){
	echo " > copying info from the buffer"
	echo "   The following functions provide a way to get copies of data"
}
clone_buffer(){
	echo "   -clone_buffer:"
	echo "        Returns a pointer to an UNOWNED HEAP copy of the entire buffer."
	echo "        Be sure to use delete[] on the pointer when you done"
	echo "        NOTE: This will return garbage data if you have written less into the buffer than its capacity"
	echo ""
}
clone_til_cursor(){
	echo "   -clone_til_cursor:"
	echo "        Returns a pointer to an UNOWNED HEAP copy of the buffer from start untill where the cursor is."
	echo "        Unlike clone_buffer this is less likely to return old data or uninitilised garbage as long as you understand what you doing,"
	echo "        However seeking before calling will effect returned data"
	echo "        Be sure to use delete[] on the pointer when you done"
	echo ""
}
clone_segment(){
	echo "   -clone_segment:"
	echo "        Returns a pointer to an UNOWNED HEAP copy of size segment_size starting from buffer start."
	echo "        similar to clone_till_cursor but can copy less or more than where the cursor it. curosor independant"
	echo "        If the segment_size is larger than the buffers capacity it will safely return the whole buffer and print a warning with info"
	echo "        NOTE: this will also return old/uninitilsed/garbage if you arent carefull with your logic"
	echo "        Be sure to use delete[] on the pointer when you done"
	echo ""
}
seeking_header(){
	echo " > Seeking:"
	echo "   The following two functions are available to change the cursor:"
}
absolute_seek(){
	echo "   -absolute_seek:"
	echo "       Simple function that sets the cursor to the number passed in, i.e 0 will set cursor back to start, 10 to buffer start + 10, and so on"
	echo "       NOTE: This is a size_t parameter therefore negative numbers will result in undefined behaviour."
	echo "             If you attempt to seek beyond the buffers limit, the cursor will not change and a warning is printed with info"
	echo ""
}
relative_seek(){
	echo "   -relative_seek:"
	echo "       Simple function that move the cursor from where it is to +/- number passed in, i.e -1 will move cursor back 1 position"
	echo "       NOTE: This is a ssize_t parameter so the only limitation would be if your buffer is larger than the negative or positive"
	echo "             signable range you would need to do multiple moves to get to a position in it"
	echo "             If you attempt to seek out of the buffers limit, the cursor will not change and a warning is printed with info"
	echo ""
}
tools(){
	echo " > Tools:"
}
validate_ptr(){
	echo "   - validate_ptr:"
	echo "        A saftey check bool pass in your pointer from the buffer and itll return true if the pointer is valid for the buffer, false otherwise"
	echo ""
}

extra(){
	echo " > Addition info"
	echo "   The DEFAULT move and copy constructors have been deleted to prevent memory issues and race conditions,"
	echo "    however if you require them, custom ones can be easily implemented"
}


if [[ $1 == "--help" ]]; then
	echo "HELP INFO:"
	echo "Run with following for"
	echo "  --compile : Info on compiling and flags."
	echo "  --general : Info on the code."
	echo "  --example : Examples of how to use the code."
	echo "  --func    : Lists useable functions than have help info."
	exit
fi


if [[ $1 == "--compile" ]]; then
	echo "To compile with code include the headerfile in your required file and use it."
	echo "The source code can be compiled with the following flags:"
	echo "  -DDONT_WARN : Disables Runtime Warnings, by defaults functions that can run into error will not throw/crash or exit,"
	echo "                instead the function prints, into std err, a msg and what went wrong, the cause, and what was done to to handle the error."
	exit
fi

if [[ $1 == "--general" ]]; then
	header
	append
	interacting_with_buffer
	borrow_buffer
	get_buffer
	borrow_current_ptr
	get_current_ptr
	realloc
	copy_header
	clone_buffer
	clone_til_cursor
	clone_segment
	seeking_header
	absolute_seek
	relative_seek
	tools
	validate_ptr
	extra
	exit
fi

if [[ $1 == "--example" ]]; then
	echo "Example.cxx contains a small demo of the different ways you can and shouldnt use the code"
	echo "works with clang aswell, Copy and run:"
	echo "g++ Example.cxx -o Example && ./Example"
	exit
fi

if [[ $1 == "--func" ]]; then
	echo "FUNCTIONS:"
	echo "  Run with following flags to just view the documentation for the function"
	echo "  --append"
	echo "  --realloc"
	echo "  --clone_buffer"
	echo "  --borrow_buffer"
	echo "  --get_buffer"
	echo "  --borrow_current_ptr"
	echo "  --get_current_ptr"
	echo "  --clone_buffer"
	echo "  --clone_til_cursor"
	echo "  --clone_segment"
	echo "  --absolute_seek"
	echo "  --relative_seek"
	echo "  --validate_ptr"
	echo "  --constructor"
	exit
fi

# done with vim macro magic
if [[ $1 == "--append" ]]; then
	append
	exit
fi

if [[ $1 == "--realloc" ]]; then
	realloc
	exit
fi

if [[ $1 == "--borrow_buffer" ]]; then
	borrow_buffer
	exit
fi
if [[ $1 == "--get_buffer" ]]; then
	get_buffer
	exit
fi

if [[ $1 == "--borrow_current_ptr" ]]; then
	borrow_current_ptr
	exit
fi

if [[ $1 == "--get_current_ptr" ]]; then
	get_current_ptr
	exit
fi

if [[ $1 == "--clone_buffer" ]]; then
	clone_buffer
	exit
fi
if [[ $1 == "--clone_til_cursor" ]]; then
	clone_til_cursor
	exit
fi
if [[ $1 == "--clone_segment" ]]; then
	clone_segment
	exit
fi
if [[ $1 == "--absolute_seek" ]]; then
	absolute_seek
	exit
fi
if [[ $1 == "--relative_seek" ]]; then
	relative_seek
	exit
fi
if [[ $1 == "--validate_ptr" ]]; then
	validate_ptr
	exit
fi
if [[ $1 == "--constructor" ]]; then
	extra
	exit
fi

echo "Welcome, the flag passed in was not recognised,"
echo "use --help for more info"
echo "If the flag you passed in was for a function's definition, it may be undocumentend or misspelt"
echo "  Unkown >> " $1
