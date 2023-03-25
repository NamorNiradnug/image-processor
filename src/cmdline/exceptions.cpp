#include "exceptions.h"

#include <sstream>

#include "prettyout.h"

ErrorWithMessage::ErrorWithMessage(const std::string& msg) : msg_(msg){};

const char* ErrorWithMessage::what() const noexcept {
    return msg_.c_str();
}

ArgumentParseError::ArgumentParseError(const char* arg_name, const std::string& msg)
    : ErrorWithMessage((std::ostringstream() << ArgColor{arg_name} << ": " << msg).str()) {
}

FilterOptionParseError::FilterOptionParseError(const char* filter_name, const std::string& msg)
    : ErrorWithMessage((std::ostringstream() << FilterColor{filter_name} << ": " << msg).str()) {
}
