#include <iostream>
#include "parser.h"

// Main function to parse a JSON-like file.
int main(int argc, char *argv[])
{
	// Check if a filename argument was provided.
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1; // Return an error code.
	}

	// Get the filename from the command line arguments.
	std::string filename = argv[1];

	// Create an instance of the Parser class with the specified filename.
	Parser parser(filename);

	// Call the parse method to start parsing the JSON-like data.
	parser.parse();

	// Exit the program.
	return 0;
}