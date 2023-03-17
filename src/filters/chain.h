#pragma once

#include <concepts>
#include <memory>

#include "../filter.h"

/*!
 * Filters composition. Applies filters from the first to the last.
 */
class FilterChain : public Filter {
public:
    using Filters = std::vector<std::unique_ptr<Filter>>;

    template <std::derived_from<Filter>... Args>
    explicit FilterChain(Args&&... filters) {
        filters_.reserve(sizeof...(filters));
        (filters_.push_back(std::make_unique<Args>(filters)), ...);
    }

    explicit FilterChain(Filters&& filters);

    void Apply(Image& image) const override;

private:
    Filters filters_;
};
