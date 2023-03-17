#include "bmploader.h"
#include "cmdline/parser.h"
#include "filters/crop.h"
#include "filters/edgedetection.h"
#include "filters/gaussianblur.h"
#include "filters/glow.h"
#include "filters/grayscale.h"
#include "filters/negative.h"
#include "filters/oldphoto.h"
#include "filters/sharpening.h"
#include "filters/waves.h"

int main(const int argc, const char* const argv[]) try {
    CmdFiltersParser<FilterOption<"-gs", "Grayscale filter", GrayscaleFilter>,
                     FilterOption<"-neg", "Negative filter", NegativeFilter>,
                     FilterOption<"-sharp", "Sharpening", SharpeningFilter>,
                     FilterOption<"-edge", "Edge Detection", EdgeDetection, Argument<"threshold", double>>,
                     FilterOption<"-crop", "Crop Image", Crop, Argument<"width", size_t>, Argument<"height", size_t>>,
                     FilterOption<"-blur", "Gaussian Blur", GaussianBlur, Argument<"sigma", double>>,
                     FilterOption<"-glow", "Simple glow effect", GlowFilter, Argument<"intensity", double>>,
                     FilterOption<"-waves", "Water waves effect", WavesFilter, Argument<"amplitude", double>,
                                  Argument<"period", double>>,
                     FilterOption<"-old", "Old photo effect", OldPhotoFilter, Argument<"border_size", size_t>,
                                  Argument<"blur_intensivity", double>>>
        parser;
    const auto [inp_file, out_file, filters] = parser.Parse(argc, argv);
    Image image = LoadImage(inp_file);
    filters.Apply(image);
    SaveImage(image, out_file);
    return 0;
} catch (const BmpFileError& error) {
    std::cerr << Colored<color::Red>{"Error: "} << error.what() << std::endl;
    std::exit(-1);
} catch (const ParserError& error) {
    std::cerr << Colored<color::Red>{"Error: "} << error.what() << std::endl;
    std::exit(-1);
}
