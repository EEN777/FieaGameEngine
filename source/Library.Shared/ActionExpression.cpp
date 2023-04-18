#include "ActionExpression.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionExpression);

	ActionExpression::ActionExpression() :
		Action(ActionExpression::TypeIdClass())
	{
	}

	void ActionExpression::AcceptOutput(Vector<ExpressionParser::Token>&& output)
	{
		_output = std::move(output);
	}

	Vector<ExpressionParser::Token>& ActionExpression::GetOutput()
	{
		return _output;
	}

	void ActionExpression::Update(const GameTime& gameTime)
	{
		auto output = _output;
		ExecuteExpression(output);
		gameTime;
	}

	Vector<Signature> ActionExpression::Signatures()
	{
		Vector<Signature> signatures
		{
			FieaGameEngine::Signature{ "Expression", Datum::DatumTypes::String, 1, offsetof(ActionExpression, _expression) }
		};

		for (auto& signature : Action::Signatures())
		{
			signatures.EmplaceBack(signature);
		}

		return signatures;
	}

	void ActionExpression::Reparse()
	{
		ExpressionParser expressionParser{};
		expressionParser.ParseExpression(_expression, this);
		AcceptOutput(std::move(expressionParser.Output()));
	}

	void ActionExpression::ExecuteExpression(Vector<ExpressionParser::Token>& output)
	{
		if (output.Size() > 0)
		{
			auto position{ output.begin() };
			bool operatorFound{ false };
			while (!operatorFound)
			{
				auto& token = *position;
				if (token._isOperator)
				{
					operatorFound = true;
				}
				else
				{
					++position;
				}
				_outputStack.Push(token);
			}
			output.Remove(output.begin(), ++position);
			auto operation = _outputStack.Top();
			_outputStack.Pop();
			std::array<ArithmeticOperation, 5> functionTable = _functionArrays.At(operation._token);
			ArithmeticOperation func = functionTable.at(std::size_t(_outputStack.Top()._type));
			assert(func != nullptr);
			(this->*func)();
			ExecuteExpression(output);
		}
		else
		{
			_outputStack.Clear();
			return;
		}
	}
	void ActionExpression::MultiplyIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		int result = lhs.GetAsInt(*this) * rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Integer });

	}
	void ActionExpression::MultiplyFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		float result = lhs.GetAsFloat(*this) * rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Float });
	}
	void ActionExpression::MultiplyVectors()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Vector);
		assert(lhs._type == rhs._type);
		glm::vec4 result = lhs.GetAsVector(*this) * rhs.GetAsVector(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Vector });
	}
	void ActionExpression::MultiplyMatrices()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Matrix);
		assert(lhs._type == rhs._type);
		glm::mat4 result = lhs.GetAsMatrix(*this) * rhs.GetAsMatrix(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Matrix });
	}
	void ActionExpression::DivideIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		int result = lhs.GetAsInt(*this) / rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::DivideFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		float result = lhs.GetAsFloat(*this) / rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Float });
	}
	void ActionExpression::DivideVectors()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Vector);
		assert(lhs._type == rhs._type);
		glm::vec4 result = lhs.GetAsVector(*this) / rhs.GetAsVector(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Vector });
	}
	void ActionExpression::DivideMatrices()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Matrix);
		assert(lhs._type == rhs._type);
		glm::mat4 result = lhs.GetAsMatrix(*this) / rhs.GetAsMatrix(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Matrix });
	}
	void ActionExpression::RemainderIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		int result = lhs.GetAsInt(*this) % rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::AddIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		int result = lhs.GetAsInt(*this) + rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::AddFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		float result = lhs.GetAsFloat(*this) + rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Float });
	}
	void ActionExpression::AddVectors()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Vector);
		assert(lhs._type == rhs._type);
		glm::vec4 result = lhs.GetAsVector(*this) + rhs.GetAsVector(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Vector });
	}
	void ActionExpression::AddMatrices()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Matrix);
		assert(lhs._type == rhs._type);
		glm::mat4 result = lhs.GetAsMatrix(*this) + rhs.GetAsMatrix(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Matrix });
	}
	void ActionExpression::ConcatenateStrings()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::String);
		assert(lhs._type == rhs._type);
		std::string result = lhs.GetAsString(*this) + rhs.GetAsString(*this);
		_outputStack.Push({ result, 0, false, false, Datum::DatumTypes::String });
	}
	void ActionExpression::SubtractIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		int result = lhs.GetAsInt(*this) - rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::SubtractFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		float result = lhs.GetAsFloat(*this) - rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Float });
	}
	void ActionExpression::SubtractVectors()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Vector);
		assert(lhs._type == rhs._type);
		glm::vec4 result = lhs.GetAsVector(*this) - rhs.GetAsVector(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Vector });
	}
	void ActionExpression::SubtractMatrices()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Matrix);
		assert(lhs._type == rhs._type);
		glm::mat4 result = lhs.GetAsMatrix(*this) - rhs.GetAsMatrix(*this);
		_outputStack.Push({ glm::to_string(result), 0, false, false, Datum::DatumTypes::Matrix });
	}
	void ActionExpression::GreaterThanIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsInt(*this) > rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::GreaterThanFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsFloat(*this) > rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::GreaterThanString()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::String);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsString(*this) > rhs.GetAsString(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::GreaterThanEqualToIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsInt(*this) >= rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::GreaterThanEqualToFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsFloat(*this) >= rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::GreaterThanEqualToString()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::String);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsString(*this) >= rhs.GetAsString(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::LessThanIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsInt(*this) < rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::LessThanFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsFloat(*this) < rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::LessThanString()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::String);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsString(*this) < rhs.GetAsString(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::LessThanEqualToIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsInt(*this) <= rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::LessThanEqualToFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsFloat(*this) <= rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::LessThanEqualToString()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::String);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsString(*this) <= rhs.GetAsString(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::EqualToIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsInt(*this) == rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::EqualToFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsFloat(*this) == rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::EqualToString()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::String);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsString(*this) == rhs.GetAsString(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::NotEqualToIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Integer);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsInt(*this) != rhs.GetAsInt(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::NotEqualToFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::Float);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsFloat(*this) != rhs.GetAsFloat(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::NotEqualToString()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._type == Datum::DatumTypes::String);
		assert(lhs._type == rhs._type);
		bool result = lhs.GetAsString(*this) != rhs.GetAsString(*this);
		_outputStack.Push({ std::to_string(BoolToInt(result)), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::LogicalNot()
	{
		auto operand = SingleVariableOperation();

		assert(operand._type == Datum::DatumTypes::Integer);
		int result{ 0 };

		if (operand.GetAsInt(*this) != 0)
		{
			result = 1;
		}
		else
		{
			result = 0;
		}

		_outputStack.Push({ std::to_string(result), 0, false, false, Datum::DatumTypes::Integer });
	}
	void ActionExpression::AssignIntegers()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._isVariable);

		Datum* target = Search(lhs._token);
		assert(target != nullptr);
		assert(target->Type() == Datum::DatumTypes::Integer);

		*target = rhs.GetAsInt(*this);
	}
	void ActionExpression::AssignFloats()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._isVariable);

		Datum* target = Search(lhs._token);
		assert(target != nullptr);
		assert(target->Type() == Datum::DatumTypes::Float);

		*target = rhs.GetAsFloat(*this);
	}
	void ActionExpression::AssignVectors()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._isVariable);

		Datum* target = Search(lhs._token);
		assert(target != nullptr);
		assert(target->Type() == Datum::DatumTypes::Vector);

		*target = rhs.GetAsVector(*this);
	}
	void ActionExpression::AssignMatrices()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._isVariable);

		Datum* target = Search(lhs._token);
		assert(target != nullptr);
		assert(target->Type() == Datum::DatumTypes::Matrix);

		*target = rhs.GetAsMatrix(*this);
	}
	void ActionExpression::AssignStrings()
	{
		auto operands = TwoVariableOperation();
		auto& lhs = operands.first;
		auto& rhs = operands.second;
		assert(lhs._isVariable);

		Datum* target = Search(lhs._token);
		assert(target != nullptr);
		assert(target->Type() == Datum::DatumTypes::String);

		*target = rhs.GetAsString(*this);
	}
	int ActionExpression::BoolToInt(bool result)
	{
		return result ? 1 : 0;
	}
	std::pair<ExpressionParser::Token, ExpressionParser::Token> ActionExpression::TwoVariableOperation()
	{
		assert(_outputStack.Size() >= 2);
		auto rhs = _outputStack.Top();
		_outputStack.Pop();

		auto lhs = _outputStack.Top();
		_outputStack.Pop();

		return { std::move(lhs), std::move(rhs) };
	}
	ExpressionParser::Token ActionExpression::SingleVariableOperation()
	{
		assert(_outputStack.Size() >= 1);
		auto operand = _outputStack.Top();
		_outputStack.Pop();
		return std::move(operand);
	}
}
