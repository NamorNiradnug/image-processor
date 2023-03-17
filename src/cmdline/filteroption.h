#pragma once

#include <charconv>
#include <cstring>
#include <memory>
#include <optional>
#include <span>

#include "../filter.h"
#include "exceptions.h"
#include "fixedstring.h"
#include "prettyout.h"

class FilterOptionBase {
public:
    [[nodiscard]] virtual std::unique_ptr<Filter> ParseFilter(const std::span<const char* const>& args_array) const = 0;
    virtual ~FilterOptionBase() = default;
};

template <typename T>
struct ArgParser {
    T operator()(const char* arg_str) const;
};

template <FixedString ArgName, typename ArgType, ArgParser<ArgType> Parser = ArgParser<ArgType>()>
struct Argument {
    static constexpr FixedString Name = ArgName;

    static ArgType Parse(const char* arg_str) {
        try {
            return Parser(arg_str);
        } catch (const ArgParserError& error) {
            throw ArgumentParseError(ArgName, error.what());
        }
    }
};

template <FixedString FilterName, FixedString FilterDescription, std::derived_from<Filter> FilterType,
          typename... Arguments>
class FilterOption : public FilterOptionBase {
    static_assert(FilterName[0] == '-', "FilterName must begin with '-'");

public:
    static constexpr FixedString Name = FilterName;
    static constexpr FixedString Description = FilterDescription;
    static constexpr size_t ArgsNumber = sizeof...(Arguments);

    [[nodiscard]] std::unique_ptr<Filter> ParseFilter(const std::span<const char* const>& args) const override {
        if (args.size() != ArgsNumber) {
            throw FilterOptionParseError(FilterName, "unexpected number of arguments: expected " +
                                                         std::to_string(ArgsNumber) + ", got " +
                                                         std::to_string(args.size()));
        }
        try {
            return ParseFilterImpl(args, std::index_sequence_for<Arguments...>());
        } catch (const ArgumentParseError& e) {
            throw FilterOptionParseError(FilterName, e.what());
        }
    }

    static void PrintArgsHelp(std::ostream& out) {
        char sep[] = "\0";
        ((out << sep << ArgColor{Arguments::Name}, sep[0] = ' '), ...);
    }

private:
    template <typename Arg>
    static auto ParseArg(const char* arg_str) {
        return Arg::Parse(arg_str);
    }

    template <size_t... Indexes>
    std::unique_ptr<Filter> ParseFilterImpl(const std::span<const char* const>& args_array,
                                            std::index_sequence<Indexes...> /*unused*/) const {
        // TODO(NamorNiradnug): refactor this, I think this can be done better
        using ArgsTuple = std::tuple<Arguments...>;
        return std::make_unique<FilterType>(
            ParseArg<std::remove_reference_t<decltype(std::get<Indexes>(ArgsTuple()))>>(args_array[Indexes])...);
    }
};
