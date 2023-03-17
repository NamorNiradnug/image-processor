#include "edgedetection.h"

#include "bypixel.h"
#include "grayscale.h"
#include "matrixfilter.h"

EdgeDetection::EdgeDetection(double threshold)
    : FilterChain(GrayscaleFilter(), Matrix3x3Filter<EdgeDetectionMatrix>(),
                  ByPixelFilter([threshold](const RGB& rgb) { return rgb.r > threshold ? RGB{1.0} : RGB{0.0}; })) {
}
