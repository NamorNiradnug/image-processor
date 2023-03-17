#pragma once

#include <exception>
#include <string>

class ErrorWithMessage : public std::exception {
public:
    explicit ErrorWithMessage(const std::string& msg);
    [[nodiscard]] const char* what() const noexcept override;

private:
    const std::string msg_;
};

class ArgParserError : public ErrorWithMessage {
public:
    explicit ArgParserError(const std::string& msg);
};

class ArgumentParseError : public ErrorWithMessage {
public:
    ArgumentParseError(const char* arg_name, const std::string& msg);
};

class FilterOptionParseError : public ErrorWithMessage {
public:
    FilterOptionParseError(const char* filter_name, const std::string& msg);
};

class ParserError : public ErrorWithMessage {
public:
    explicit ParserError(const std::string& msg);
};
