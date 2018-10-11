#ifndef HADAMARD_H
#define HADAMARD_H

#include <vector>
#include <cstdio>
#include <algorithm>

namespace // Annonymous to keep namespace clean
{
	// Type defs for the hadamard matrix
	typedef std::vector<std::vector<char>> hadamard_matrix;
	typedef std::vector<std::vector<char>>::iterator hadamard_matrix_itr;
}

// some helper functions for hadamard matricies
namespace hadamard_utils
{
	void print_matrix(hadamard_matrix& matrix, const char * title);
	
	template<typename T>
	void vector_copy_concat(std::vector<T>& vector);
};

// Hadamard class: contains hadamard matrix generation and code 
// generation functions
class hadamard
{
	public:
		// Public functions
		hadamard(size_t hadamard_size);
		~hadamard() {};
		void generate_code_words(size_t hadamard_size, size_t code_index);		

	private:
		// Private Vars
		size_t m_hadamard_size;
		size_t m_hadamard_dimension;
		hadamard_matrix m_matrix;
		
		// Private Functions
		void generate_matrix(hadamard_matrix& matrix, size_t size);
};

#endif //HADAMARD_H

