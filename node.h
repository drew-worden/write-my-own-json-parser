#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace JSON
{
	// Forward declaration of the Node class.
	class Node;

	// Define aliases for common JSON data structures.
	using Object = std::map<std::string, std::shared_ptr<Node>>;
	using List = std::vector<std::shared_ptr<Node>>;

	// Enumeration to represent different JSON value types.
	enum class Type
	{
		OBJECT,
		LIST,
		STRING,
		NUMBER,
		BOOLEAN,
		NULL_TYPE
	};

	// Union to store different JSON values.
	union Values
	{
		Object *object;
		List *list;
		std::string *s;
		float f;
		bool b;
	};

	// The Node class represents a JSON-like node with a value and type.
	class Node
	{
		Values values; // Union to store different JSON values.
		Type type; // Type of the JSON value.

	public:
		Node() {}

		Node(Type t)
		{
			type = t;
		}

		// Getter functions for different JSON value types.
		auto returnObject()
		{
			if (type == Type::OBJECT)
			{
				return *values.object;
			}
			throw std::logic_error("Improper return.");
		}

		auto returnList()
		{
			if (type == Type::LIST)
			{
				return *values.list;
			}
			throw std::logic_error("Improper return.");
		}

		auto returnString()
		{
			if (type == Type::STRING)
			{
				return *values.s;
			}
			throw std::logic_error("Improper return.");
		}

		auto returnFloat()
		{
			if (type == Type::NUMBER)
			{
				return values.f;
			}
			throw std::logic_error("Improper return.");
		}

		// Setter functions for different JSON value types.
		void setObject(Object *object)
		{
			this->values.object = object;
			type = Type::OBJECT;
		}

		void setList(List *list)
		{
			this->values.list = list;
			type = Type::LIST;
		}

		void setString(std::string *s)
		{
			this->values.s = s;
			type = Type::STRING;
		}

		void setNumber(float f)
		{
			this->values.f = f;
			type = Type::NUMBER;
		}

		void setBoolean(bool v)
		{
			this->values.b = v;
			type = Type::BOOLEAN;
		}

		void setNull()
		{
			type = Type::NULL_TYPE;
		}

		// Function to convert the node to a string with indentation.
		std::string toString(int indentationLevel)
		{
			std::string spaceString = std::string(indentationLevel, ' ');
			std::string outputString = "";

			switch (type)
			{
			case Type::STRING:
			{
				outputString += spaceString + *values.s;
				break;
			}

			case Type::NUMBER:
			{
				outputString += spaceString + std::to_string(values.f);
				break;
			}

			case Type::BOOLEAN:
			{
				outputString += spaceString + (values.b ? "true" : "false");
				break;
			}

			case Type::NULL_TYPE:
			{
				outputString += spaceString + "null";
				break;
			}

			case Type::LIST:
			{
				std::cout << "[";
				int index = 0;
				for (auto node : (*values.list))
				{
					outputString += node->toString(indentationLevel + 1);
					if (index < (*values.list).size() - 1)
					{
						outputString += spaceString + ", ";
					}
					index++;
				}
				outputString += spaceString + "]\n";
				break;
			}

			case Type::OBJECT:
			{
				outputString += "{\n";
				for (Object::iterator i = (*values.object).begin();
					 i != (*values.object).end(); i++)
				{
					outputString += spaceString + i->first + ": ";
					outputString += i->second->toString(indentationLevel + 1);
					Object::iterator next = i;
					next++;
					if ((next) != (*values.object).end())
					{
						outputString += spaceString + ",";
					}
					outputString += "}\n";
				}
			}
			}
			outputString += "}\n";
			return outputString;
		}

		// Function to print the node to the console with indentation.
		void printNode(int indentationLevel)
		{
			std::cout << toString(indentationLevel);
		}
	};
} // namespace JSON
