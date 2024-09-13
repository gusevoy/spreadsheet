#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>

struct PositionHasher {
    size_t operator()(Position pos) const;
};

class Sheet : public SheetInterface {
public:
    using SheetCells = std::unordered_map<Position, std::unique_ptr<Cell>, PositionHasher>;

    //~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

private:
    Size CalculatePrintableSize() const;
    void Print(std::ostream& output, std::function<void(std::ostream&, const CellInterface*)> func) const;
    void ValidatePosition(Position pos) const;

    SheetCells cells_;
};

