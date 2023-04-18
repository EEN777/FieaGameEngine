#include "ExpressionParser.h"

namespace FieaGameEngine
{
	ExpressionParser::Token::Token(const std::string& token, int precendence, bool isVariable, bool isOperator, Datum::DatumTypes type) :
		_token{ token }, _precedence{ precendence }, _isVariable{ isVariable }, _isOperator{ isOperator }, _type{ type }
	{

	}

	const std::string& ExpressionParser::Token::GetAsString(Action& action)
	{
		if (_isVariable)
		{
			Datum* variable = action.Search(_token);
			assert(variable != nullptr);
			assert(variable->Type() == Datum::DatumTypes::String);
			return variable->GetAsString();
		}

		return _token;
	}

	int ExpressionParser::Token::GetAsInt(Action& action)
	{
		if (_isVariable)
		{
			Datum* variable = action.Search(_token);
			assert(variable != nullptr);
			assert(variable->Type() == Datum::DatumTypes::Integer);
			return variable->GetAsInt();
		}

		assert(_type == Datum::DatumTypes::Integer);
		return std::stoi(_token);
	}

	float ExpressionParser::Token::GetAsFloat(Action& action)
	{
		if (_isVariable)
		{
			Datum* variable = action.Search(_token);
			assert(variable != nullptr);
			assert(variable->Type() == Datum::DatumTypes::Float);
			return variable->GetAsFloat();
		}

		assert(_type == Datum::DatumTypes::Float);
		return std::stof(_token);
	}

	glm::vec4 ExpressionParser::Token::GetAsVector(Action& action)
	{
		if (_isVariable)
		{
			Datum* variable = action.Search(_token);
			assert(variable != nullptr);
			assert(variable->Type() == Datum::DatumTypes::Vector);
			return variable->GetAsVector();
		}

		glm::vec4 vec{};
		int result = sscanf_s(_token.c_str(), " vec4(%f, %f, %f, %f) ", &vec.x, &vec.y, &vec.z, &vec.w);
		assert(result == 4);
		result;
		return vec;
	}

	glm::mat4 ExpressionParser::Token::GetAsMatrix(Action& action)
	{
		if (_isVariable)
		{
			Datum* variable = action.Search(_token);
			assert(variable != nullptr);
			assert(variable->Type() == Datum::DatumTypes::Matrix);
			return variable->GetAsMatrix();
		}

		glm::vec4 vec1{};
		glm::vec4 vec2{};
		glm::vec4 vec3{};
		glm::vec4 vec4{};
		int result = sscanf_s(_token.c_str(), " mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f)) ",
			&vec1.x, &vec1.y, &vec1.z, &vec1.w,
			&vec2.x, &vec2.y, &vec2.z, &vec2.w,
			&vec3.x, &vec3.y, &vec3.z, &vec3.w,
			&vec4.x, &vec4.y, &vec4.z, &vec4.w
		);
		assert(result == 16);
		result;
		return glm::mat4{ vec1, vec2, vec3, vec4 };
	}

	void ExpressionParser::ParseExpression(const std::string& expression, Action* action)
	{
		std::size_t position = expression.find(" ");
		std::string token = expression.substr(0, position);
		ParseToken(token, action);
		if (position != expression.npos)
		{
			std::string remainingExpression = expression.substr(position + 1, expression.npos);
			if (remainingExpression.empty())
			{
				while (!_operators.IsEmpty())
				{
					auto operation = _operators.Top();
					_output.PushBack(operation);
					_operators.Pop();
				}
				return;
			}
			else
			{
				ParseExpression(remainingExpression, action);
			}
		}

		while (!_operators.IsEmpty())
		{
			_output.PushBack(_operators.Top());
			_operators.Pop();
		}
	}

	void ExpressionParser::Reset()
	{
		_output.Clear();
		_operators.Clear();
	}

	Vector<ExpressionParser::Token> ExpressionParser::Output()
	{
		return _output;
	}
	
	void ExpressionParser::ParseToken(std::string& token, Action* action)
	{
		auto position = _operatorDefinitions.Find(token);
		if (position != _operatorDefinitions.end())
		{
			auto operation = position->second;

			if (operation._token == ")")
			{
				bool sendingToOutput{ true };
				while (sendingToOutput)
				{
					auto topOperation = _operators.Top();
					if (topOperation._token == "(")
					{
						sendingToOutput = false;
					}
					else
					{
						_output.PushBack(topOperation);
					}
					_operators.Pop();
				}
			}
			else
			{
				if (!_operators.IsEmpty())
				{
					auto topOperation = _operators.Top();
					if (topOperation._token != "(" && operation._token != "(")
					{
						if (operation._precedence >= topOperation._precedence)
						{
							_output.PushBack(topOperation);
							_operators.Pop();
						}
					}
				}
				_operators.Push(operation);
			}
		}
		else
		{
			Datum* value = action->Search(token);
			if (value == nullptr)
			{
				auto type = ParseType(token);
				_output.PushBack({ token, 0, false, false, type });
			}
			else
			{
				_output.PushBack({ token, 0, true, false, value->Type() });
			}
		}
	}

	Datum::DatumTypes ExpressionParser::ParseType(std::string& token)
	{
		for (auto& check : _typeChecks)
		{
			auto type = (this->*check)(token);
			if (type != Datum::DatumTypes::Unknown)
			{
				return type;
			}
		}
		return Datum::DatumTypes::Unknown;
	}

	Datum::DatumTypes ExpressionParser::CheckVector(std::string& token)
	{
		auto type = Datum::DatumTypes::Unknown;
		auto position = token.find("vec4");
		if (position != token.npos)
		{
			type = Datum::DatumTypes::Vector;
		}
		return type;
	}

	Datum::DatumTypes ExpressionParser::CheckMatrix(std::string& token)
	{
		auto type = Datum::DatumTypes::Unknown;
		auto position = token.find("mat4x4");
		if (position != token.npos)
		{
			type = Datum::DatumTypes::Matrix;
		}
		return type;
	}

	Datum::DatumTypes ExpressionParser::CheckInt(std::string& token)
	{
		for (auto& character : token)
		{
			if (!std::isdigit(character))
			{
				return Datum::DatumTypes::Unknown;
			}
		}
		return Datum::DatumTypes::Integer;
	}

	Datum::DatumTypes ExpressionParser::CheckFloat(std::string& token)
	{
		auto type = Datum::DatumTypes::Unknown;
		auto position = token.find("_f");
		if (position != token.npos)
		{
			token.erase(position);
			type = Datum::DatumTypes::Float;
		}
		return type;
	}

	Datum::DatumTypes ExpressionParser::CheckString(std::string& token)
	{
		auto type = Datum::DatumTypes::Unknown;
		auto position = token.find("_s");
		if (position != token.npos)
		{
			token.erase(position);
			type = Datum::DatumTypes::String;
		}
		return type;
	}
}
