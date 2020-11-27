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

        Symbol token = symbolSequence.GetSymbol();

        IExpression* prevExpression = nullptr;

        while (token.IsCorrect()) {
            prevExpression = m_expression;

            if (token.IsNumber()) {
                m_expression = new Number {};
            } else if (token.IsUnaryOperator(symbolSequence.GetPrevSymbol())) {
                if (token.IsNegative()) {
                    m_expression = new Negative {};
                }
            } else if (token.IsBinaryOperator(symbolSequence.GetPrevSymbol())) {
                if (token.IsSubtract()) {
                    m_expression = new Subtract { prevExpression };
                } else if (token.IsAddition()) {
                    m_expression = new Addition { prevExpression };
                } else if (token.IsDivide()) {
                    m_expression = new Divide { prevExpression };
                } else if (token.IsMultiply()) {
                    m_expression = new Multiply { prevExpression };
                }
            } else if (token.IsBeginScope()) {
                m_expression = new Scope {};
            } else if (token.IsEndScope()) {
                m_expression = new Empty {};
                return;
            }

            if (!m_expression) {
                m_expression = new Empty {};
            }

            m_expression->Interpret(symbolSequence);
            symbolSequence.Shift();
            token = symbolSequence.GetSymbol();
        }
        if (!m_expression) {
            m_expression = new Empty {};
        }
    }

    double Scope::Calculate() { return m_expression->Calculate(); }

}