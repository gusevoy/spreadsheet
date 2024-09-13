#include "cell.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <optional>

using namespace std::literals;

// Реализуйте следующие методы
Cell::Cell() : impl_(new EmptyImpl()){
}

//Cell::~Cell() {}

void Cell::Set(std::string text) {
	if (text == ""s) {
		impl_ = std::make_unique<EmptyImpl>();
	}
	else if (text[0] == FORMULA_SIGN && text.size() > 1) {
		impl_ = std::make_unique<FormulaImpl>(std::move(text));
	}
	else {
		impl_ = std::make_unique<TextImpl>(std::move(text));
	}
}

void Cell::Clear() {
	impl_ = std::make_unique<EmptyImpl>();
}

Cell::Value Cell::GetValue() const {
	return impl_->GetValue();
}
std::string Cell::GetText() const {
	return impl_->GetText();
}

std::string Cell::EmptyImpl::GetText() const {
	return ""s;
}

CellInterface::Value Cell::EmptyImpl::GetValue() const {
	return ""s;
}

Cell::TextImpl::TextImpl(std::string text) : text_(std::move(text)) {
}

std::string Cell::TextImpl::GetText() const {
	return text_;
}

CellInterface::Value Cell::TextImpl::GetValue() const {
	if (text_[0] == ESCAPE_SIGN) {
		return text_.substr(1, text_.size() - 1);
	}
	else {
		return text_;
	}
}

Cell::FormulaImpl::FormulaImpl(const std::string& text) {
	formula_ = ParseFormula(text.substr(1, text.size() - 1));
}

std::string Cell::FormulaImpl::GetText() const {
	return FORMULA_SIGN + formula_->GetExpression();
}

CellInterface::Value Cell::FormulaImpl::GetValue() const {
	FormulaInterface::Value result = formula_->Evaluate();
	if (std::holds_alternative<double>(result)) {
		return std::get<double>(result);
	}
	else {
		return std::get<FormulaError>(result);
	}
}
