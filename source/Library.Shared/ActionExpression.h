#pragma once
#include "Action.h"
#include "ExpressionParser.h"

namespace FieaGameEngine
{
	class ActionExpression : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action);

		Vector<ExpressionParser::Token> _output{};
		Stack<ExpressionParser::Token> _outputStack{};

		std::string _expression{};


	public:
		ActionExpression();
		void AcceptOutput(Vector<ExpressionParser::Token>&& output);
		Vector<ExpressionParser::Token>& GetOutput();
		virtual void Update(const GameTime& gameTime) override;
		static Vector<Signature> Signatures();
		void Reparse();
		virtual ~ActionExpression() {};

	private:
		void ExecuteExpression(Vector<ExpressionParser::Token>&);
		void MultiplyIntegers();
		void MultiplyFloats();
		void MultiplyVectors();
		void MultiplyMatrices();
		void DivideIntegers();
		void DivideFloats();
		void DivideVectors();
		void DivideMatrices();
		void RemainderIntegers();
		void AddIntegers();
		void AddFloats();
		void AddVectors();
		void AddMatrices();
		void ConcatenateStrings();
		void SubtractIntegers();
		void SubtractFloats();
		void SubtractVectors();
		void SubtractMatrices();
		void GreaterThanIntegers();
		void GreaterThanFloats();
		void GreaterThanString();
		void GreaterThanEqualToIntegers();
		void GreaterThanEqualToFloats();
		void GreaterThanEqualToString();
		void LessThanIntegers();
		void LessThanFloats();
		void LessThanString();
		void LessThanEqualToIntegers();
		void LessThanEqualToFloats();
		void LessThanEqualToString();
		void EqualToIntegers();
		void EqualToFloats();
		void EqualToString();
		void NotEqualToIntegers();
		void NotEqualToFloats();
		void NotEqualToString();
		void LogicalNot();
		void AssignIntegers();
		void AssignFloats();
		void AssignVectors();
		void AssignMatrices();
		void AssignStrings();
		int BoolToInt(bool result);
		std::pair<ExpressionParser::Token, ExpressionParser::Token> TwoVariableOperation();
		ExpressionParser::Token SingleVariableOperation();

		using ArithmeticOperation = void(ActionExpression::*)();


		inline static const std::array<ArithmeticOperation, 5> _multiplicationFunctions
		{
			&ActionExpression::MultiplyIntegers,
			&ActionExpression::MultiplyFloats,
			nullptr,
			&ActionExpression::MultiplyVectors,
			&ActionExpression::MultiplyMatrices
		};

		inline static const std::array<ArithmeticOperation, 5> _divisionFunctions
		{
			&ActionExpression::DivideIntegers,
			&ActionExpression::DivideFloats,
			nullptr,
			&ActionExpression::DivideVectors,
			&ActionExpression::DivideMatrices
		};

		inline static const std::array<ArithmeticOperation, 5> _additionFunctions
		{
			&ActionExpression::AddIntegers,
			&ActionExpression::AddFloats,
			&ActionExpression::ConcatenateStrings,
			&ActionExpression::AddVectors,
			&ActionExpression::AddMatrices
		};

		inline static const std::array<ArithmeticOperation, 5> _subtractionFunctions
		{
			&ActionExpression::SubtractIntegers,
			&ActionExpression::SubtractFloats,
			nullptr,
			&ActionExpression::SubtractVectors,
			&ActionExpression::SubtractMatrices
		};

		inline static const std::array<ArithmeticOperation, 5> _remainderFunctions
		{
			&ActionExpression::RemainderIntegers,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		};

		inline static const std::array<ArithmeticOperation, 5> _assignmentFunctions
		{
			&ActionExpression::AssignIntegers,
			&ActionExpression::AssignFloats,
			&ActionExpression::AssignStrings,
			&ActionExpression::AssignVectors,
			&ActionExpression::AssignMatrices
		};

		inline static const std::array<ArithmeticOperation, 5> _greaterThanFunctions
		{
			&ActionExpression::GreaterThanIntegers,
			&ActionExpression::GreaterThanFloats,
			&ActionExpression::GreaterThanString,
			nullptr,
			nullptr
		};

		inline static const std::array<ArithmeticOperation, 5> _greaterThanEqualToFunctions
		{
			&ActionExpression::GreaterThanEqualToIntegers,
			&ActionExpression::GreaterThanEqualToFloats,
			&ActionExpression::GreaterThanEqualToString,
			nullptr,
			nullptr
		};

		inline static const std::array<ArithmeticOperation, 5> _lessThanFunctions
		{
			&ActionExpression::LessThanIntegers,
			&ActionExpression::LessThanFloats,
			&ActionExpression::LessThanString,
			nullptr,
			nullptr
		};

		inline static const std::array<ArithmeticOperation, 5> _lessThanEqualToFunctions
		{
			&ActionExpression::LessThanEqualToIntegers,
			&ActionExpression::LessThanEqualToFloats,
			&ActionExpression::LessThanEqualToString,
			nullptr,
			nullptr
		};

		inline static const std::array<ArithmeticOperation, 5> _equalToFunctions
		{
			&ActionExpression::EqualToIntegers,
			&ActionExpression::EqualToFloats,
			&ActionExpression::EqualToString,
			nullptr,
			nullptr
		};

		inline static const std::array<ArithmeticOperation, 5> _notEqualToFunctions
		{
			&ActionExpression::NotEqualToIntegers,
			&ActionExpression::NotEqualToFloats,
			&ActionExpression::NotEqualToString,
			nullptr,
			nullptr
		};

		inline static const std::array<ArithmeticOperation, 5> _logicalNotFunction
		{
			&ActionExpression::LogicalNot,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		};


		inline static HashMap<std::string, std::array<ArithmeticOperation, 5>> _functionArrays
		{
			std::pair{"+", _additionFunctions},
			std::pair{"-", _subtractionFunctions},
			std::pair{"*", _multiplicationFunctions},
			std::pair{"/", _divisionFunctions},
			std::pair{"%", _remainderFunctions},
			std::pair{"=", _assignmentFunctions},
			std::pair{"==", _equalToFunctions},
			std::pair{"!=", _notEqualToFunctions},
			std::pair{">", _greaterThanFunctions},
			std::pair{">=", _greaterThanEqualToFunctions},
			std::pair{"<", _lessThanFunctions},
			std::pair{"<=", _lessThanEqualToFunctions},
			std::pair{"!", _logicalNotFunction},
		};
	};

	ConcreteFactory(ActionExpression, Scope);
}

