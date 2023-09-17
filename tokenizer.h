#include <fstream>
#include <vector>
#include <map>
#include <stdexcept>
#include <stack>
#include <iostream>

// Enumeration representing different types of JSON tokens.
enum class TokenType
{
	CURLY_OPEN,
	CURLY_CLOSE,
	COLON,
	STRING,
	NUMBER,
	ARRAY_OPEN,
	ARRAY_CLOSE,
	COMMA,
	BOOLEAN,
	NULL_TYPE
};

// Structure representing a JSON token with a value and type.
struct Token
{
	std::string value; // Value of the token (if applicable).
	TokenType type; // Type of the token.

	// Function to convert the token to a string representation.
	std::string toString()
	{
		switch (type)
		{
		case TokenType::CURLY_OPEN:
			return "Curly open";
		case TokenType::CURLY_CLOSE:
			return "Curly close";
		case TokenType::COLON:
			return "COLON";
		case TokenType::NUMBER:
			return "Number: " + value;
		case TokenType::STRING:
			return "String: " + value;
		case TokenType::ARRAY_OPEN:
			return "Array open";
		case TokenType::ARRAY_CLOSE:
			return "Array close";
		case TokenType::COMMA:
			return "Comma";
		case TokenType::BOOLEAN:
			return "Boolean: " + value;
		case TokenType::NULL_TYPE:
			return "Null";
		}
	}
};

// Tokenizer class for parsing a JSON file into tokens.
class Tokenizer
{
	std::fstream file; // Input file stream.
	size_t previousPosition; // Store the previous read position.

public:
	// Constructor that opens the specified file for reading.
	Tokenizer(std::string filename)
	{
		file.open(filename, std::ios::in);
		if (!file.good())
		{
			std::cout << "File open error." << std::endl;
		}
	}

	// Helper function to skip whitespace characters and get the next non-whitespace character.
	auto getWithoutWhiteSpace()
	{
		char c = ' ';
		while ((c == ' ' || c == '\n'))
		{
			file.get(c);

			if ((c == ' ' || c == '\n') && !file.good())
			{
				throw std::logic_error("Ran out of tokens.");
			}
			else if (!file.good())
			{
				return c;
			}
		}
		return c;
	}

	// Function to get the next token from the input stream.
	auto getToken()
	{
		char c;
		if (file.eof())
		{
			std::cout << "Exhausted tokens." << std::endl;
		}
		previousPosition = file.tellg();
		c = getWithoutWhiteSpace();

		std::cout << c << std::endl;

		struct Token token;
		if (c == '"')
		{
			token.type = TokenType::STRING;
			token.value = "";
			file.get(c);
			while (c != '"')
			{
				token.value += c;
				file.get(c);
			}
		}
		else if (c == '{')
		{
			token.type = TokenType::CURLY_OPEN;
		}
		else if (c == '}')
		{
			token.type = TokenType::CURLY_CLOSE;
		}
		else if (c == '-' || (c >= '0' && c <= '9'))
		{
			token.type = TokenType::NUMBER;
			token.value = "";
			token.value += c;
			std::streampos previousCharacterPosition = file.tellg();

			while ((c == '-') || (c >= '0' && c <= '9') || c == '.')
			{
				previousCharacterPosition = file.tellg();
				file.get(c);

				if (file.eof())
				{
					break;
				}
				else
				{
					if ((c == '-') || (c >= '0' && c <= '9') || (c == '.'))
					{
						token.value += c;
					}
					else
					{
						file.seekg(previousCharacterPosition);
					}
				}
			}
		}
		else if (c == 'f')
		{
			token.type = TokenType::BOOLEAN;
			token.value = "False";
			file.seekg(4, std::ios_base::cur);
		}
		else if (c == 't')
		{
			token.type = TokenType::BOOLEAN;
			token.value = "True";
			file.seekg(4, std::ios_base::cur);
		}
		else if (c == 'n')
		{
			token.type = TokenType::NULL_TYPE;
			file.seekg(3, std::ios_base::cur);
		}
		else if (c == '[')
		{
			token.type = TokenType::ARRAY_OPEN;
		}
		else if (c == ']')
		{
			token.type = TokenType::ARRAY_CLOSE;
		}
		else if (c == ':')
		{
			token.type = TokenType::COLON;
		}
		else if (c == ',')
		{
			token.type = TokenType::COMMA;
		}
		return token;
	}

	// Function to check if there are more tokens in the input stream.
	auto hasMoreTokens()
	{
		return !file.eof();
	}

	// Function to rollback to the previous token in the input stream.
	void rollbackToken()
	{
		if (file.eof())
		{
			file.clear();
		}
		file.seekg(previousPosition);
	}
};
