#ifndef LINEAR_ALLOCATOR_HXX
#define LINEAR_ALLOCATOR_HXX

#include <iostream>
#include <sys/types.h>
#include <string.h>

template< typename T>
class linear_allocator{

	private:
	size_t buffer_cursor 		= 0;
	size_t current_capacity 	= 0;
	size_t max_capacity 		= 0;

	T * buffer 	= nullptr;

	void _grow_buffer();
	public:
	linear_allocator(const linear_allocator &) = delete;
	linear_allocator(linear_allocator &&) = delete;
	linear_allocator &operator=(const linear_allocator &) = delete;
	linear_allocator &operator=(linear_allocator &&) = delete;
	linear_allocator(size_t arena_capacity);
	~linear_allocator();

	size_t get_max_capacity();
	size_t get_current_capacity();


	/**
	* resize the current buffer to store a new_capacity.
	* NOTE: any data in the previous buffer will be lost.
	*/
	void realloc(size_t new_capacity);

	/**
	* Optimised inlined data copier.
	* Starts writing from cursor position until append_amound.
	* NOTE: will grow buffer if append_amount will supass the buffer
	* capacity.
	*/
	void inline append(const void *src_buffer, size_t append_amount);

	/**
	* Optimised inlined data copier.
	* Starts writing from cursor position until append_amound.
	* NOTE: will grow buffer if append_amount will supass the buffer
	* capacity.
	*/
	void inline const_append(const void *const src_buffer, size_t append_amount);

	/**
	 * Borrow an IMUTABLE refrence to the buffer start.
	 */
	const T* const &borrow_buffer();

	/**
	 * get a raw MUTABLE refrence to the buffer start.
	 */
	T* &use_buffer();


	/**
	 * Borrow an IMUTABLE refrence to the buffer from the current cursor position.
	 */
	const T* const borrow_current_ptr();

	/**
	 * get a raw MUTABLE refrence to the buffer from the current cursor position.
	 */
	T* use_current_ptr();

	/**
	 * set the absolute the position of the cursor in the buffer.
	 */
	void absolute_seek(size_t new_cursor_position);

	/**
	 * Change the position of the cursor in the buffer relative to where it already is.
	 */
	void relative_seek(ssize_t relative_shift);

	/**
	 * returns a UNOWNED HEAP COPY of the entire buffer.
	 */
	T *clone_buffer();

	/**
	 * returns a UNOWNED HEAP COPY of the buffer from 0 till the current curosor poisition.
	 */
	T *clone_til_cursor();

	/**
	 * returns a UNOWNED HEAP COPY of the buffer from the start untill segment_size.
	 * NOTE, if segment_size is larger than the buffers max capacity,
	 *   a copy of the entire buffer will be returned in place and a warning is printed if enabled
	 *   warning can be suppressed by compiling with -DDONT_WARN
	 */
	T *clone_segment(size_t segment_size);

	/**
	 * Helper function that determines if a pointer still is valid inside the current buffer
	 * true if valid, false otherwise.
	 */
	bool validate_ptr(T *possible_invalid_ptr);

};

//typdefs
typedef linear_allocator<char> char_buffer;
typedef linear_allocator<unsigned char> uchar_buffer;

typedef linear_allocator<short> short_buffer;
typedef linear_allocator<unsigned short> ushort_buffer;

typedef linear_allocator<int> int_buffer;
typedef linear_allocator<unsigned int> uint_buffer;

typedef linear_allocator<ssize_t> long_buffer;
typedef linear_allocator<size_t> ulong_buffer;

//definition

template<typename T>
bool linear_allocator<T>::validate_ptr(T *possible_invalid_ptr){
	return possible_invalid_ptr >= linear_allocator::buffer && possible_invalid_ptr <= linear_allocator::buffer + linear_allocator::current_capacity;
}

template<typename T>
size_t linear_allocator<T>::get_max_capacity(){
	return linear_allocator::max_capacity;
}

template<typename T>
size_t linear_allocator<T>::get_current_capacity(){
	return linear_allocator::current_capacity;
}

template<typename T>
linear_allocator<T>::linear_allocator(size_t arena_capacity){
	linear_allocator::current_capacity = arena_capacity;
	linear_allocator::max_capacity = arena_capacity;

	linear_allocator::buffer = new T[arena_capacity];
}

template<typename T>
linear_allocator<T>::~linear_allocator(){
	delete[] linear_allocator::buffer;
}

template<typename T>
void linear_allocator<T>::realloc(size_t new_capacity){

	if(new_capacity > linear_allocator::max_capacity){
		T *_new_buffer = new T[new_capacity];
		linear_allocator::current_capacity = new_capacity;

		delete linear_allocator::buffer;
		linear_allocator::buffer = _new_buffer;

		return;
	}

	linear_allocator::buffer_cursor = 0;
	linear_allocator::current_capacity = new_capacity;

}

template<typename T>
void inline linear_allocator<T>::append(const void *src_buffer, size_t append_amount){

	if(linear_allocator::buffer_cursor + append_amount > linear_allocator::max_capacity){
		linear_allocator::_grow_buffer();
	}else if(linear_allocator::buffer_cursor + append_amount > linear_allocator::current_capacity){
		linear_allocator::current_capacity += append_amount;
	}

	const size_t *_ = (const size_t *)src_buffer;
	while(append_amount > 8){
		(size_t&)linear_allocator::buffer[buffer_cursor] = *_++;
		linear_allocator::buffer_cursor +=8;
		append_amount -=8;
	}

	const char* __ = (const char*)_;
	while(append_amount--){
		linear_allocator::buffer[buffer_cursor++] = *__++;
	}

}
/*
template<typename T>
void inline linear_allocator<T>::const_append(const void *const src_buffer, size_t append_amount){

	if(linear_allocator::buffer_cursor + append_amount > linear_allocator::max_capacity){
		linear_allocator::_grow_buffer();
	}else if(linear_allocator::buffer_cursor + append_amount > linear_allocator::current_capacity){
		linear_allocator::current_capacity += append_amount;
	}

	const size_t *_ = (const size_t *)src_buffer;
	for(size_t i = 0; append_amount > 8; i += 8, append_amount -= 8){
		(size_t&)linear_allocator::buffer[buffer_cursor] = *(_+i);
		linear_allocator::buffer_cursor +=8;
	}

	const char* __ = (const char*)_;

	for(size_t i = 0; append_amount > 8; i++, append_amount--){
		(size_t&)linear_allocator::buffer[buffer_cursor] = *(__+i);
		linear_allocator::buffer_cursor +=8;
	}
}
*/

template<typename T>
const T* const linear_allocator<T>::borrow_current_ptr(){
	return &linear_allocator::buffer[buffer_cursor];
}

template<typename T>
T* linear_allocator<T>::use_current_ptr(){
	return &linear_allocator::buffer[buffer_cursor];
}

template<typename T>
const T* const &linear_allocator<T>::borrow_buffer(){
	return linear_allocator::buffer;
}

template<typename T>
T* &linear_allocator<T>::use_buffer(){
	return linear_allocator::buffer;
}

template<typename T>
T *linear_allocator<T>::clone_buffer(){
	T * _new_buffer = new T[linear_allocator::max_capacity];

	memcpy(_new_buffer, linear_allocator::buffer, linear_allocator::max_capacity);

	return _new_buffer;
}

template<typename T>
T *linear_allocator<T>::clone_til_cursor(){
	T * _new_buffer = new T[linear_allocator::buffer_cursor];

	memcpy(_new_buffer, linear_allocator::buffer, linear_allocator::buffer_cursor);

	return _new_buffer;
}

template<typename T>
T *linear_allocator<T>::clone_segment(size_t segment_size){
	if(segment_size > linear_allocator::max_capacity){
#ifndef DONT_WARN
		std::cerr << "[WARNING] Segment Size provided was larger than the current capacity of buffer." <<
			"\n  The entire buffer was returned instead\n" <<
			"  Requested size: " << segment_size;
#endif

		return linear_allocator::clone_buffer();
	}
	T * _new_buffer = new T[linear_allocator::buffer_cursor];

	memcpy(_new_buffer, linear_allocator::buffer, segment_size);

	return _new_buffer;
}


template<typename T>
void linear_allocator<T>::absolute_seek(size_t new_cursor_position){
	if(new_cursor_position >= linear_allocator::max_capacity){
#ifndef DONT_WARN
		//ERROR
		std::cerr <<
			"[WARNING] Absolute Seek failed, cursor has not been changed\n[REASON] Attempting to seek beyound available buffer lenght\n  Range is: 0 : " <<
			linear_allocator::max_capacity <<
			".\n  Provided seek value: " <<
			new_cursor_position <<
			" is not in the range.";
#endif
		return;
	}

	linear_allocator::buffer_cursor = new_cursor_position;

}

template<typename T>
void linear_allocator<T>::relative_seek(ssize_t relative_shift){
	if(relative_shift < 0){
		if(static_cast<size_t>(-relative_shift) > linear_allocator::buffer_cursor ){
#ifndef DONT_WARN
		//ERROR
			std::cerr <<
				"[WARNING] Relative Seek failed, cursor has not been changed\n[REASON] Attempting to seek under available buffer range\n  Range is: 0 : " <<
				linear_allocator::max_capacity <<
				".\n  Provided seek value: " <<
				relative_shift <<
				" results in" << linear_allocator::buffer_cursor - relative_shift << " which is not in the range.";
#endif
			return;
		}
		linear_allocator::buffer_cursor -= relative_shift;
		return;
	}

	if (relative_shift + linear_allocator::buffer_cursor > linear_allocator::max_capacity){
		//ERROR
#ifndef DONT_WARN
		std::cerr <<
			"[WARNING] Relative Seek failed, cursor has not been changed\n[REASON] Attempting to seek above available buffer range\n  Range is: 0 : " <<
			linear_allocator::max_capacity <<
			".\n  Provided seek value: " <<
			relative_shift <<
			" results in" << linear_allocator::buffer_cursor - relative_shift << " which is not in the range.";
#endif
		return;
	}

	linear_allocator::buffer_cursor += relative_shift;
}

//privates

template<typename T>
void linear_allocator<T>::_grow_buffer(){
	linear_allocator::max_capacity *= 2;
	linear_allocator::current_capacity = linear_allocator::max_capacity;

	// NOTE might infitely loop if values arent correct. need testing
	T *_double_size_buffer = new T[max_capacity];
	linear_allocator::buffer_cursor = 0;
	append(_double_size_buffer, linear_allocator::max_capacity/2);

	delete linear_allocator::buffer;
	linear_allocator::buffer = _double_size_buffer;

}

#endif
