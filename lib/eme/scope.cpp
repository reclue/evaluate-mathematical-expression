#include "scope.h"

#include "empty.h"
#include "number.h"
#include "negative.h"
#include "subtract.h"
#include "addition.h"
#include "divide.h"
#include "multiply.h"


namespace reclue {

    Scope::Scope() : m_expression {} {}
    Scope::~Scope() { delete m_expression; }

    void Scope::Interpret(SymbolSequence& symbolSequence) {
        if (symbolSequence.GetSymbol().IsBeginScope()) {
            symbolSequence.Shift();
        }

        Symbol symbol = symbolSequence.GetSymbol();

        IExpression* prevExpression = nullptr;

        while (!symbol.IsDeadEnd()) {
            prevExpression = m_expression;

            if (symbol.IsNumber()) {
                m_expression = new Number {};
            } else if (symbol.IsUnaryOperator(symbolSequence.GetPrevSymbol())) {
                if (symbol.IsNegative()) {
                    m_expression = new Negative {};
                }
            } else if (symbol.IsBinaryOperator(symbolSequence.GetPrevSymbol())) {
                ABinaryOperator* binaryOperator { nullptr };

                if (symbol.IsSubtract()) {
                    binaryOperator = new Subtract {};
                } else if (symbol.IsAddition()) {
                    binaryOperator = new Addition {};
                } else if (symbol.IsDivide()) {
                    binaryOperator = new Divide {};
                } else if (symbol.IsMultiply()) {
                    binaryOperator = new Multiply {};
                }

                if (binaryOperator) {
                    binaryOperator->SetFirst(prevExpression);
                    m_expression = binaryOperator;
                }
            } else if (symbol.IsBeginScope()) {
                m_expression = new Scope {};
            } else if (symbol.IsEndScope()) {
                m_expression = new Empty {};
                return;
            }

            if (!m_expression) {
                m_expression = new Empty {};
            }

            m_expression->Interpret(symbolSequence);
            symbolSequence.Shift();
            symbol = symbolSequence.GetSymbol();
        }

        if (!m_expression) {
            m_expression = new Empty {};
        }
    }

    double Scope::Calculate() { return m_expression->Calculate(); }

}
