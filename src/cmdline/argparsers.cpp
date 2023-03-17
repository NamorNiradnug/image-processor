#include "filteroption.h"

template <>
double ArgParser<double>::operator()(const char* arg_str) const {
    double result;  // NOLINT
    const auto& [end, err] = std::from_chars(arg_str, arg_str + std::strlen(arg_str), result);
    if (err != std::errc() || end != arg_str + std::strlen(arg_str)) {
        throw ArgParserError(std::string("invalid floating-point value: ") + arg_str);
    }
    return result;
}

template <>
size_t ArgParser<size_t>::operator()(const char* arg_str) const {
    long result;  // NOLINT
    const auto& [end, err] = std::from_chars(arg_str, arg_str + std::strlen(arg_str), result);
    if (err != std::errc() || end != arg_str + std::strlen(arg_str) || result < 0) {
        throw ArgParserError(std::string("invalid positive integer value: ") + arg_str);
    }
    return result;
}
