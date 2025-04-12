#include "linear_allocator.hxx"


void example_using_the_buffer(){
	std::cout << "Using buffer\n";
	size_t type_size = strlen("@type");
	const size_t CAPACITY = 27;

	char mock_file[CAPACITY] = {
		"string @type = Hello World"
	};

	// typedef char_buffer is the same as linear_allocator<char>(CAPACITY)
	char_buffer line(CAPACITY);

	//copying the contents of mock_file into the internal buffer
	line.append(mock_file, CAPACITY);

	//Heap Allocated copy of the entire buffer, you own it and have to free it
	char *_copy_of_line = line.clone_buffer();
	printf("  This is a copy of the buffer, you own it\n    Copied data: %s\n", _copy_of_line);
	delete[] _copy_of_line; _copy_of_line = nullptr;

	std::cout << '\n';


	//another buffer since why not
	char_buffer parsed_line(line.get_max_capacity());

	//caching the pointer
	const char *const line_start = line.borrow_buffer();
	for(size_t i = 0; i < line.get_max_capacity(); i++){
		if(line_start[i] == 'H'){
			parsed_line.append(&line_start[i], line.get_max_capacity()-i-1);
			break;
		}
	}

	printf("  This is borrowed pointer you dont own it.\n   Borrowed Data: %s\n", parsed_line.borrow_buffer());
	std::cout << '\n';

	std::string copying_data_safely_to_another_source = std::string(parsed_line.borrow_buffer(), parsed_line.get_max_capacity());
	printf("  Extracting data without redundant copy using borrow_buffer and std::string.\n    String: %s\n", copying_data_safely_to_another_source.c_str());

	std::cout << '\n';

}


void example_validate_ptr(){
	std::cout << "Invalid/Valid pointer Example:\n";
	size_t start_capacity = 16;
	char_buffer pointer_example(start_capacity);

	char *invalid = (char *)6;
	char *valid = pointer_example.use_buffer();

	std::cout << "  Invalid ptr since its not from the buffer\n    Validate Pointer Result: " << pointer_example.validate_ptr(invalid) << '\n';
	std::cout << '\n';
	std::cout << "  Valid ptr since use_buffer() returns a refrence to the buffer\n    Validate Pointer Result: " << pointer_example.validate_ptr(valid) << '\n';
	std::cout << '\n';

	pointer_example.absolute_seek(6);
	char *invalid_from_growth = pointer_example.use_current_ptr();

	size_t new_size = 32;
	pointer_example.realloc(new_size);

	std::cout << "  Invalid ptr since realloc was called\n    Validate Pointer Result: " << pointer_example.validate_ptr(invalid_from_growth);
	std::cout << '\n';

}

int main(){

	example_using_the_buffer();

	example_validate_ptr();

	std::cout << "RUN SUCCESSFULL"<< std::endl;

	return 0;
}
