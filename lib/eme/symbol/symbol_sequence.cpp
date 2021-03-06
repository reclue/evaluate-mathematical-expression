#include "symbol_sequence.h"


namespace reclue {

    bool SymbolSequence::HasNext() const {
        return !m_sequence.empty() && m_position < m_sequence.size();
    }

    bool SymbolSequence::SkipIncorrectSymbols() {
        for (;; ++m_position) {
            if (!HasNext()) {
                m_currentSymbol = Symbol {};
                return false;
            }

            m_currentSymbol = Symbol { m_sequence[m_position] };

            if (m_currentSymbol.IsCorrect()) return true;
        }
    }

    SymbolSequence::SymbolSequence(const std::string_view& expression) :
            m_sequence { expression }, m_position { 0 },
            m_currentSymbol { HasNext() ? m_sequence[m_position] : Symbol::DEADEND } {
        SkipIncorrectSymbols();
    }

    Symbol SymbolSequence::GetSymbol() const { return m_currentSymbol; }

    bool SymbolSequence::Shift() {
        if (HasNext()) {
            ++m_position;
            return SkipIncorrectSymbols();
        }

        return false;
    }

}
