#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

size_t PositionHasher::operator()(Position pos) const {
    return static_cast<size_t>(pos.row * Position::MAX_COLS + pos.col);
}

//Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text) {
    ValidatePosition(pos);
    auto cell = std::make_unique<Cell>();
    cell->Set(text);
    cells_[pos].swap(cell);
}

const CellInterface* Sheet::GetCell(Position pos) const {
    ValidatePosition(pos);    auto it = cells_.find(pos);
    if (it == cells_.end()) {
        return nullptr;
    }
    return (*it).second.get();
}

CellInterface* Sheet::GetCell(Position pos) {
    ValidatePosition(pos);    auto it = cells_.find(pos);
    if (it == cells_.end()) {
        return nullptr;
    }
    return (*it).second.get();
}

void Sheet::ClearCell(Position pos) {
    ValidatePosition(pos);
    cells_.erase(pos);
}

Size Sheet::GetPrintableSize() const {
    return CalculatePrintableSize();
}

void Sheet::PrintValues(std::ostream& output) const {
    Print(output, [](std::ostream& out, const CellInterface* cell) {
            if (cell) {
                std::visit(
                    [&](const auto& x) {
                        out << x;
                    },
                    cell->GetValue());
            }
        });
}

void Sheet::PrintTexts(std::ostream& output) const {
    Print(output, [](std::ostream& out, const CellInterface* cell) {
        if (cell) {
            out << cell->GetText();
        }
        });
}


Size Sheet::CalculatePrintableSize() const {
    int max_row = -1;
    int max_col = -1;
    for (const auto& pair : cells_) {
        max_row = std::max(max_row, pair.first.row);
        max_col = std::max(max_col, pair.first.col);
    }
    return { max_row + 1, max_col + 1};
}

void Sheet::Print(std::ostream& output, std::function<void(std::ostream&, const CellInterface*)> func) const {
    Size size = GetPrintableSize();
    for (int row = 0; row < size.rows; ++row) {
        for (int col = 0; col < size.cols; ++col) {
            if (col != 0) {
                output << '\t';
            }
            func(output, GetCell({ row, col }));
        }
        output << '\n';
    }
}

void Sheet::ValidatePosition(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position"s);
    }
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}