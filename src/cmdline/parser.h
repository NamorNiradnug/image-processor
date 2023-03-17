#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include "../filters/chain.h"
#include "exceptions.h"
#include "filteroption.h"

struct ParsedCmdOptions {
    const char* in_file_path;
    const char* out_file_path;
    FilterChain filters;
};

template <std::derived_from<FilterOptionBase>... Options>
class CmdFiltersParser {
public:
    ParsedCmdOptions Parse(int argc, const char* const argv[], std::ostream& out = std::cout) const {
        if (argc == 1) {
            PrintHelp(argv[0], out);
            std::exit(0);
        }
        if (argc < 2) {
            throw ParserError("missing input file path");
        }
        if (argc < 3) {
            throw ParserError("missing output file path");
        }

        FilterChain::Filters filters;
        for (size_t argi = 3; argi < argc;) {
            const char* filter_name = argv[argi++];
            if (!REGISTRED_FILTERS.contains(filter_name)) {
                throw ParserError("unknown filter: " + std::string(FilterColor{filter_name}));
            }
            const auto& filter_option = REGISTRED_FILTERS.at(filter_name);
            size_t filter_args_count = 0;
            while (argi + filter_args_count < argc && argv[argi + filter_args_count][0] != '-') {
                ++filter_args_count;
            }
            try {
                filters.push_back(filter_option->ParseFilter({argv + argi, filter_args_count}));
            } catch (const FilterOptionParseError& error) {
                throw ParserError(error.what());
            }
            argi += filter_args_count;
        }

        return {argv[1], argv[2], FilterChain(std::move(filters))};
    }

private:
    using FilterOptionsMap = std::map<std::string_view, std::unique_ptr<FilterOptionBase>>;
    inline static const FilterOptionsMap REGISTRED_FILTERS =
        // cannot use list initializer syntax here because cannot copy std::unique_ptr
        []() {
            FilterOptionsMap result;
            (result.emplace(Options::Name, new Options()), ...);
            return result;
        }();

    static void PrintHelp(const char* cmd_name, std::ostream& out) {
        using SectionColor = Colored<color::Yellow>;

        out << Colored<color::Green>{"Image Processor"} << std::endl
            << "A simple image manipulation utility.\n"
            << std::endl
            << SectionColor{"Usage:"} << std::endl
            << "    " << cmd_name << ' ' << FileColor{"input_file"} << ' ' << FileColor{"output_file"} << " ["
            << FilterColor{"FILTERS"} << " [" << ArgColor{"ARGS"} << "]]\n"
            << std::endl;

        out << SectionColor{"Filters:"} << std::endl;
        (PrintOptionHelp<Options>(out), ...);
    }

    template <std::derived_from<FilterOptionBase> Option>
    static void PrintOptionHelp(std::ostream& out) {
        static constexpr size_t OptionWithArgsMaxLength = 25;
        static constexpr size_t InvisibleCharsSize =
            (sizeof(color::Reset) + sizeof(color::White) - 2) * (1 + Option::ArgsNumber);
        std::stringstream help_stream;
        help_stream << "    " << FilterColor{Option::Name} << ' ';
        Option::PrintArgsHelp(help_stream);
        out << std::left << std::setw(OptionWithArgsMaxLength + InvisibleCharsSize) << help_stream.view();
        if (help_stream.view().size() >= OptionWithArgsMaxLength + InvisibleCharsSize) {
            out << '\n' << std::string(OptionWithArgsMaxLength, ' ') << Option::Description;
        } else {
            out << Option::Description;
        }
        out << std::endl;
    }
};
