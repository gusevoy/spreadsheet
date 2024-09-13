#include "formula.h"

#include "FormulaAST.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <sstream>

using namespace std::literals;

std::ostream& operator<<(std::ostream& output, FormulaError fe) {
    return output << "#ARITHM!";
}

namespace {
class Formula : public FormulaInterface {
public:

// Реализуйте следующие методы:

    /*
    explicit Formula(std::string expression) try : ast_(ParseFormulaAST(expression)) {
    }
    catch (const FormulaException& ex) {
        throw ex;
    }
    */

    explicit Formula(std::string expression) : ast_(ParseFormulaAST(expression)) {
    }

    Value Evaluate() const override {
        double result;
        try {
            result = ast_.Execute();
        }
        catch (const FormulaError& ex) {
            return ex;
        }
        return result;
    }

    std::string GetExpression() const override {
        std::stringstream buf;
        ast_.PrintFormula(buf);
        return buf.str();
    }

private:
    FormulaAST ast_;
};
}  // namespace

std::unique_ptr<FormulaInterface> ParseFormula(std::string expression) {
    return std::make_unique<Formula>(std::move(expression));
}