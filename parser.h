#include "node.h"
#include "tokenizer.h"
#include <memory>

// Parser class for parsing JSON-like data from a file into a tree structure.
class Parser
{
	std::fstream file; // Input file stream
	std::shared_ptr<JSON::Node> root; // The root of the parsed JSON tree
	std::unique_ptr<JSON::Node> current; // Current node being parsed
	Tokenizer tokenizer; // Tokenizer used for reading and parsing tokens

public:
	// Constructor that takes a filename and initializes the tokenizer.
	Parser(const std::string filename) : tokenizer(filename) {}

	// Function to start parsing the input JSON data.
	void parse()
	{
		std::string key = "";
		while (tokenizer.hasMoreTokens())
		{
			Token token;
			try
			{
				token = tokenizer.getToken();
				std::cout << token.toString() << std::endl;

				switch (token.type)
				{
				case TokenType::CURLY_OPEN:
				{
					std::shared_ptr<JSON::Node> parsedObject = parseObject();
					parsedObject->printNode(0);
					if (!root)
					{
						root = parsedObject;
					}
					break;
				}
				case TokenType::ARRAY_OPEN:
				{
					std::shared_ptr<JSON::Node> parsedList = parseList();
					if (!root)
					{
						root = parsedList;
					}
					break;
				}
				case TokenType::STRING:
				{
					tokenizer.rollbackToken();
					std::shared_ptr<JSON::Node> parsedString = parseString();
					if (!root)
					{
						root = parsedString;
					}
					break;
				}
				case TokenType::NUMBER:
				{
					tokenizer.rollbackToken();
					std::shared_ptr<JSON::Node> parsedNumber = parseNumber();
					if (!root)
					{
						root = parsedNumber;
					}
					break;
				}
				case TokenType::BOOLEAN:
				{
					tokenizer.rollbackToken();
					std::shared_ptr<JSON::Node> parsedBoolean = parseBoolean();
					break;
				}
				}
			}
			catch (std::logic_error e)
			{
				break;
			}
		}
	}

	// Function to parse a JSON object.
	std::shared_ptr<JSON::Node> parseObject()
	{
		std::cout << "Parsing object " << std::endl;
		std::shared_ptr<JSON::Node> node = std::make_shared<JSON::Node>();
		JSON::Object *keyObjectMap = new JSON::Object();
		bool hasCompleted = false;

		while (!hasCompleted)
		{
			if (tokenizer.hasMoreTokens())
			{
				Token nextToken = tokenizer.getToken();
				std::string key = nextToken.value;
				std::cout << key << std::endl;
				tokenizer.getToken();
				nextToken = tokenizer.getToken();
				std::shared_ptr<JSON::Node> node;

				switch (nextToken.type)
				{
				case TokenType::STRING:
				{
					tokenizer.rollbackToken();
					(*keyObjectMap)[key] = parseString();
					break;
				}
				case TokenType::ARRAY_OPEN:
				{
					(*keyObjectMap)[key] = parseList();
					break;
				}
				case TokenType::NUMBER:
				{
					tokenizer.rollbackToken();
					(*keyObjectMap)[key] = parseNumber();
					break;
				}
				case TokenType::CURLY_OPEN:
				{
					(*keyObjectMap)[key] = parseObject();
					break;
				}
				case TokenType::BOOLEAN:
				{
					tokenizer.rollbackToken();
					(*keyObjectMap)[key] = parseBoolean();
					break;
				}
				case TokenType::NULL_TYPE:
				{
					(*keyObjectMap)[key] = parseNull();
					break;
				}
				}

				nextToken = tokenizer.getToken();
				if (nextToken.type == TokenType::CURLY_CLOSE)
				{
					hasCompleted = true;
					break;
				}
				else
				{
					throw std::logic_error("No more tokens");
				}
			}
		}
		node->setObject(keyObjectMap);
		return node;
	}

	// Function to parse a JSON string.
	std::shared_ptr<JSON::Node> parseString()
	{
		std::cout << "Parsing String" << std::endl;
		std::shared_ptr<JSON::Node> node = std::make_shared<JSON::Node>();
		Token token = tokenizer.getToken();
		std::string *sValue = new std::string(token.value);
		node->setString(sValue);
		return node;
	}

	// Function to parse a JSON number.
	std::shared_ptr<JSON::Node> parseNumber()
	{
		std::cout << "Parsing Number" << std::endl;
		std::shared_ptr<JSON::Node> node = std::make_shared<JSON::Node>();
		Token nextToken = tokenizer.getToken();
		std::string value = nextToken.value;
		std::cout << value << std::endl;
		float fValue = std::stof(value);
		node->setNumber(fValue);
		return node;
	}

	// Function to parse a JSON list.
	std::shared_ptr<JSON::Node> parseList()
	{
		std::cout << "Parsing List" << std::endl;
		std::shared_ptr<JSON::Node> node = std::make_shared<JSON::Node>();
		JSON::List *list = new JSON::List();
		bool hasCompleted = false;

		while (!hasCompleted)
		{
			if (!tokenizer.hasMoreTokens())
			{
				throw std::logic_error("No more tokens");
			}
			else
			{
				Token nextToken = tokenizer.getToken();
				std::shared_ptr<JSON::Node> node;
				switch (nextToken.type)
				{
				case TokenType::ARRAY_OPEN:
				{
					node = parseList();
					break;
				}
				case TokenType::CURLY_OPEN:
				{
					node = parseObject();
					break;
				}
				case TokenType::STRING:
				{
					tokenizer.rollbackToken();
					node = parseString();
					break;
				}
				case TokenType::NUMBER:
				{
					tokenizer.rollbackToken();
					node = parseNumber();
					break;
				}
				case TokenType::BOOLEAN:
				{
					tokenizer.rollbackToken();
					node = parseBoolean();
					break;
				}
				case TokenType::NULL_TYPE:
				{
					node = parseNull();
					break;
				}
				}
				list->push_back(node);
				nextToken = tokenizer.getToken();
				if (nextToken.type == TokenType::ARRAY_CLOSE)
				{
					hasCompleted = true;
				}
			}
		}
		node->setList(list);
		return node;
	}

	// Function to parse a JSON boolean.
	std::shared_ptr<JSON::Node> parseBoolean()
	{
		std::cout << "Parsing boolean" << std::endl;
		std::shared_ptr<JSON::Node> node = std::make_shared<JSON::Node>();
		Token nextToken = tokenizer.getToken();
		node->setBoolean(nextToken.value == "True" ? true : false);
		return node;
	}

	// Function to parse a JSON null value.
	std::shared_ptr<JSON::Node> parseNull()
	{
		std::cout << "Parsing null" << std::endl;
		std::shared_ptr<JSON::Node> node = std::make_shared<JSON::Node>();
		node->setNull();
		return node;
	}
};
