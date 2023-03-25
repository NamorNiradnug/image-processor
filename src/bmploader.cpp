#include "bmploader.h"

#include <array>
#include <fstream>
#include <numeric>

#include "cmdline/prettyout.h"

BmpFileError::BmpFileError(const char* file, const std::string& msg)
    : ErrorWithMessage((std::ostringstream() << FileColor{file} << ": " << msg).str()) {
}

namespace bmp {
static constexpr size_t FileHeaderSize = 14;
static constexpr const char DIBHeaderName[] = "BITMAPINFOHEADER";
static constexpr size_t DIBHeaderSize = 40;
static constexpr uint16_t BitsPerPixel = 24;

enum Compression : uint32_t {
    BI_RGB = 0,
    BI_RLE8 = 1,
    BI_RLE4 = 2,
    BI_BITFIELDS = 3,
    BI_JPEG = 4,
    BI_PNG = 5,
    BI_ALPHABITFIELDS = 6,
    BI_CMYK = 11,
    BI_CMYKRLE8 = 12,
    BI_CMYKRLE4 = 13,
};
}  // namespace bmp

template <typename T>
static T Read(std::ifstream& fstream) {
    T result;
    fstream.read(reinterpret_cast<char*>(&result), sizeof(T));
    return result;
}

template <typename T>
static void Write(std::ofstream& fstream, const T& value) {
    fstream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

#pragma pack(push, 1)
struct BmpHeader {
    char id[2] = {'B', 'M'};
    uint32_t file_size;
    char unused[4] = "\0\0\0";
    uint32_t data_offset;
};

struct DIBHeader {
    uint32_t header_size;
    struct Size {
        int32_t width;
        int32_t height;
    } image_size;
    uint16_t number_of_planes;
    uint16_t bits_per_pixel;
    bmp::Compression compression;
    uint32_t bitmap_size;
    struct PrintResolution {
        uint32_t horizontal = 0;
        uint32_t vertical = 0;
    } print_resolution;
    uint32_t colors_in_palette = 0;
    uint32_t important_colors = 0;
};
#pragma pack(pop)

static_assert(sizeof(DIBHeader) == bmp::DIBHeaderSize);
static_assert(std::endian::native == std::endian::little);

class InvalidHeader : public ErrorWithMessage {
public:
    explicit InvalidHeader(const std::string& msg) : ErrorWithMessage(msg){};
};

static void ValidateBMPHeader(const BmpHeader& bmp_header) {
    if (bmp_header.id[0] != 'B' || bmp_header.id[1] != 'M') {
        throw InvalidHeader("invalid BMP file ID: " + std::string(bmp_header.id, 2));
    }
}

static void ValidateDIBHeader(const DIBHeader& dib_header) {
    if (dib_header.header_size != bmp::DIBHeaderSize) {
        throw InvalidHeader("unsupported DIB header size " + std::to_string(dib_header.header_size) + "; only " +
                            bmp::DIBHeaderName + " DIB header format (" + std::to_string(bmp::DIBHeaderSize) +
                            " bytes) is supported");
    }

    if (dib_header.image_size.width < 0) {
        throw InvalidHeader("BMP with negative pixmap width");
    }
    if (dib_header.number_of_planes != 1) {
        throw InvalidHeader("unexpected number of color planes: expected 1, got " +
                            std::to_string(dib_header.number_of_planes));
    }

    if (dib_header.bits_per_pixel != bmp::BitsPerPixel) {
        throw InvalidHeader("unsupported BPP (bits per pixel): got " + std::to_string(dib_header.bits_per_pixel) +
                            ", but only " + std::to_string(bmp::BitsPerPixel) + " is supported.");
    }

    if (dib_header.compression != bmp::Compression::BI_RGB) {
        throw InvalidHeader("unsupported compression method, only BI_RGB (uncompressed) is supported.");
    }
}

Image LoadImage(const char* path) {
    std::ifstream image_stream(path, std::ios_base::binary);
    if (!image_stream.is_open()) {
        throw BmpFileError(path, "cannot open file");
    }

    const auto bmp_header = Read<BmpHeader>(image_stream);
    const auto dib_header = Read<DIBHeader>(image_stream);
    try {
        ValidateBMPHeader(bmp_header);
        ValidateDIBHeader(dib_header);
    } catch (const InvalidHeader& error) {
        throw BmpFileError(path, error.what());
    }

    const auto& [image_width, image_height] = dib_header.image_size;

    Image image{static_cast<size_t>(image_width), static_cast<size_t>(std::abs(image_height))};
    for (size_t y = 0; y < std::abs(image_height); ++y) {
        for (size_t x = 0; x < image_width; ++x) {
            const auto blue = Read<uint8_t>(image_stream);
            const auto green = Read<uint8_t>(image_stream);
            const auto red = Read<uint8_t>(image_stream);
            image.At(x, image_height > 0 ? image_height - y - 1 : y) = {red, green, blue};
        }
        image_stream.ignore(static_cast<std::streamsize>((image_width * 3 + 3) / 4 * 4 - image_width * 3));
    }
    return image;
}

void SaveImage(const Image& image, const char* path) {
    std::ofstream image_stream(path, std::ios_base::binary);
    if (!image_stream.is_open()) {
        throw BmpFileError(path, "cannot open file to save image");
    }

    // --- BMP Header ---
    const uint32_t row_size = ((bmp::BitsPerPixel * image.Width() + 31) / 32) * 4;
    const uint32_t raw_data_size = row_size * image.Height();
    const uint32_t file_size = bmp::FileHeaderSize + bmp::DIBHeaderSize + raw_data_size;

    Write(image_stream, BmpHeader{.file_size = file_size, .data_offset = bmp::FileHeaderSize + bmp::DIBHeaderSize});

    // --- DIB Header ---
    Write(image_stream, DIBHeader{
                            .header_size = bmp::DIBHeaderSize,
                            .image_size = {static_cast<int32_t>(image.Width()), static_cast<int32_t>(image.Height())},
                            .number_of_planes = 1,
                            .bits_per_pixel = bmp::BitsPerPixel,
                            .compression = bmp::Compression::BI_RGB,
                            .bitmap_size = raw_data_size,
                        });

    // --- Bitmap Data ---
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            const auto& [r, g, b] = image.Get(x, image.Height() - y - 1);
            Write(image_stream, static_cast<uint8_t>(b * std::numeric_limits<uint8_t>::max()));
            Write(image_stream, static_cast<uint8_t>(g * std::numeric_limits<uint8_t>::max()));
            Write(image_stream, static_cast<uint8_t>(r * std::numeric_limits<uint8_t>::max()));
        }
        image_stream.write("\0\0\0", static_cast<std::streamsize>(row_size - 3 * image.Width()));
        image_stream.flush();
    }
    image_stream.close();
}
