#include <stdint.h>
#include <cmath>
#include "hadamard.h"

void hadamard_utils::print_matrix(hadamard_matrix& matrix, const char * title)
{
	printf("%s\r\n", title);
	printf("Dimensions - rows: %zu, cols: %zu\r\n", matrix.size(), matrix[0].size());

	int row = 1;
	for(auto& line : matrix)
	{
		int col = 1;
		for (auto& val : line)
		{
			printf("% 2d ", val);
			++col;
		}
		printf("\r\n");
		++row;
	}	
}

template<typename T>
void hadamard_utils::vector_copy_concat(std::vector<T>& vector)
{
	auto vector_size = vector.size();
	vector.resize(2 * vector_size);
	std::copy_n(vector.begin(), vector_size, vector.begin() + vector_size);
}


hadamard::hadamard(size_t hadamard_size) :
	m_hadamard_size(hadamard_size),
	m_hadamard_dimension((size_t) pow(2, hadamard_size)),
	m_matrix{{1}}
{
	printf("Generating Hadamard Matrix %zu - dimension size: %zux%zu\r\n",
		m_hadamard_size, m_hadamard_dimension, m_hadamard_dimension);
		
	generate_matrix(m_matrix, m_hadamard_dimension);

	printf("\r\nMatrix total size: %zu\r\n", m_matrix.size() * m_matrix[0].size());
	printf("\r\ndone\r\n");
}


/* Recursivley multiplies the matrix, as shown below, until
 * matrix size 'size' has been reached.
 * 
 * M -->  M | M
 *        --+--
 *        M |-M
 */
void hadamard::generate_matrix(hadamard_matrix& matrix, size_t size)
{
	if (matrix.size() < size)
	{
		printf("\r\n------ Generating Next ------\r\n");
		
		// 1. Copy the columns across such that:
		// M --> M | M
		size_t matrix_size = matrix.size();
		for(size_t i = 0; i < matrix_size; i++)
		{
			hadamard_utils::vector_copy_concat(matrix[i]);
		}
		
		// 2. Copy the rows down such that: 
		// M | M --> M | M
		//           --+--
		//           M | M
		hadamard_utils::vector_copy_concat(matrix);
		
		// 3. Invert the values in the 'bottom-right-quarter' of the matrix such that:
		// M | M       M | M
		// --+--  -->  --+--
		// M | M       M |-M
		for (size_t row = matrix.size() / 2; row < matrix.size(); ++row)
		{
			for (size_t col = matrix[row].size() / 2; col < matrix[row].size(); ++col)
			{
				matrix[row][col] *= -1;
			}
		}		
		
		// Print result
		hadamard_utils::print_matrix(matrix, "after");		
		
		// Recurse
		generate_matrix(matrix, size);
	}
}

// Generates vector for code words for a given hadamard size and code index
// The result is a vector of 16-bit words set MSB first
void hadamard::generate_code_words(size_t hadamard_size, size_t code_index)
{
	printf("\r\nGENERATING CODE WORD - H: %zu, Index: %zu\r\n",
		hadamard_size, code_index);

	// Validity Checks
	if (code_index >= m_matrix.size())
	{
		printf("code_index %zu is invalid, the matrix has %zu codes: %d to %zu\r\n",
			code_index, m_hadamard_dimension, 0, m_hadamard_dimension - 1);
		return;
	}

	if (hadamard_size > m_hadamard_size)
	{
		printf("hadamard_size %zu is invalid, the matrix hadamard_size is %zu\r\n",
			hadamard_size, m_hadamard_size);
		return;
	}
	
	size_t code_size = (size_t) pow(2, hadamard_size);
	if (code_index >= code_size)
	{
		printf("code_index %zu is greater then the table with hardamard_size %zu, valid codes: %d to %zu\r\n",
			code_index, hadamard_size, 0, code_size - 1);
		return;
	}	
	
	std::vector<uint16_t> code_words;	
	size_t bit_index = 16;
	uint16_t word = 0; // Set all bits to zero
	// Loop through the bits of the code at index 'code_index' and
	// generate the code_words
	for(auto& val : m_matrix[code_index])
	{
		printf("% 2d ", (val + 1) / 2);
		
		// If the code bit is 1 then set that bit in the word to 1
		// Note: -1 is treated as 0 which the bit is set to at initialisation
		if (val > 0)
		{
			word |= 1 << (bit_index - 1);
		}
		
		// Decrent the bit index until we have read in an entire word.
		// When the entire word is read, store it and reset the index.
		--bit_index;	
		if (bit_index == 0)
		{
			code_words.push_back(word);
			word = 0;
			bit_index = 16;
		}
		
		// A code can be requested that is from a matrix smaller then the generated matrix
		// (the generated matrix is a super set of all smaller matricies), therefore a code
		// that was requested could be smaller then the complete matrix code bits. Once we 
		// have got all the code bits then write any remainig partial words and then stop.
		--code_size;
		if (code_size == 0)
		{
			// E.g. we requested a spreading code of length 4 then we would not have pushed
			// the word back yet.
			if (bit_index < 16)
			{
				code_words.push_back(word);
			}
			break;
		}
	}
	
	printf("\r\nLong Code Word: ");
	for (auto code_word : code_words)
	{
		printf("%04x", code_word);
	}
	printf("\r\n");
}


int main()
{
	// Test code...
	
	// Generate a hadamard matrix
	hadamard * p_had = new hadamard(5);
	
	// Now generate some codes
	p_had->generate_code_words(5, 33);
	p_had->generate_code_words(5, 31);
	p_had->generate_code_words(4, 13);
	p_had->generate_code_words(4, 16);
	
	
	printf("\r\ndone\r\n");
	return 0;
}

