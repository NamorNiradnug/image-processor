#include "exceptions.h"

#include "prettyout.h"
#include <sstream>

ErrorWithMessage::ErrorWithMessage(const std::string& msg) : msg_(msg){};

const char* ErrorWithMessage::what() const noexcept {
    return msg_.c_str();
}

ArgParserError::ArgParserError(const std::string& msg) : ErrorWithMessage(msg) {
}

ArgumentParseError::ArgumentParseError(const char* arg_name, const std::string& msg)
    : ErrorWithMessage((std::ostringstream() << ArgColor{arg_name} << ": " << msg).str()) {
}

FilterOptionParseError::FilterOptionParseError(const char* filter_name, const std::string& msg)
    : ErrorWithMessage((std::ostringstream() << FilterColor{filter_name} << ": " << msg).str()) {
}

ParserError::ParserError(const std::string& msg) : ErrorWithMessage(msg) {
}
