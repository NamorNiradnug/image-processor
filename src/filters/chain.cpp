#include "chain.h"

FilterChain::FilterChain(Filters&& filters) : filters_(std::move(filters)) {
}

void FilterChain::Apply(Image& image) const {
    for (const auto& filter : filters_) {
        filter->Apply(image);
    }
}
