#pragma once
#include "Datum.h"
#include "Stack.h"
#include "Vector.h"
#include "HashMap.h"
#include "Action.h"

namespace FieaGameEngine
{
	class ExpressionParser final
	{
	public:
		struct Token final
		{
			std::string _token{};
			Datum* _value { nullptr };
			int _precedence{};
			bool _isOperator{ false };
			bool _isVariable{ false };
			Datum::DatumTypes _type{ Datum::DatumTypes::Unknown };

			Token() = delete;
			Token(const std::string& token, int precendence, bool isVariable, bool isOperator, Datum::DatumTypes type = Datum::DatumTypes::Unknown);

			const std::string& GetAsString(Action& action);
			int GetAsInt(Action& action);
			float GetAsFloat(Action& action);
			glm::vec4 GetAsVector(Action& action);
			glm::mat4 GetAsMatrix(Action& action);
		};

	private:
		Vector<Token> _output{};
		Stack<Token> _operators{};

		inline static HashMap<std::string, ExpressionParser::Token> _operatorDefinitions
		{
			std::pair{"+", Token{"+", 4, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"-", Token{"-", 4, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"*", Token{"*", 3, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"/", Token{"/", 3, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"%", Token{"%", 3, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"=", Token{"=", 14, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"==", Token{"==", 7, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"!=", Token{"!=", 7, false, true, Datum::DatumTypes::Unknown}},
			std::pair{">", Token{">", 6, false, true, Datum::DatumTypes::Unknown}},
			std::pair{">=", Token{">=", 6, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"<", Token{"<", 6, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"<=", Token{"<=", 6, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"!", Token{"!", 2, false, true, Datum::DatumTypes::Unknown}},
			std::pair{"(", Token{"(", 1, false, true, Datum::DatumTypes::Unknown}},
			std::pair{")", Token{")", 1, false, true, Datum::DatumTypes::Unknown}}
		};

	public:

		void ParseExpression(const std::string& expression, Action* action);
		void Reset();
		Vector<Token> Output();

	private:
		void ParseToken(std::string& token, Action* action);
		Datum::DatumTypes ParseType(std::string& token);
		Datum::DatumTypes CheckVector(std::string& token);
		Datum::DatumTypes CheckMatrix(std::string& token);
		Datum::DatumTypes CheckInt(std::string& token);
		Datum::DatumTypes CheckFloat(std::string& token);
		Datum::DatumTypes CheckString(std::string& token);

		using TypeCheck = Datum::DatumTypes(ExpressionParser::*)(std::string&);

		inline static const std::array<TypeCheck, 5> _typeChecks
		{
			&ExpressionParser::CheckVector,
			&ExpressionParser::CheckMatrix,
			&ExpressionParser::CheckString,
			&ExpressionParser::CheckFloat,
			&ExpressionParser::CheckInt
		};
	};
}