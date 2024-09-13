#pragma once

#include "common.h"
#include "formula.h"

class Cell : public CellInterface {
public:
    Cell();
    ~Cell() = default;

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;

private:

    class Impl {
    public:
        virtual std::string GetText() const = 0;
        virtual Value GetValue() const = 0;
        virtual ~Impl() = default;
    };

    class EmptyImpl : public Impl {
    public:
        std::string GetText() const override;
        Value GetValue() const override;
    };

    class TextImpl : public Impl {
    public:
        TextImpl(std::string text);
        std::string GetText() const override;
        Value GetValue() const override;
    private:
        std::string text_;
    };

    class FormulaImpl : public Impl {
    public:
        FormulaImpl(const std::string& text);
        std::string GetText() const override;
        Value GetValue() const override;
    private:
        std::unique_ptr<FormulaInterface> formula_;
    };

    std::unique_ptr<Impl> impl_;

};