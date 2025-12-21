#pragma once

/// \file
/// Image processing tools

#include "CommonDefinitions.h"


namespace Diligent
{

/// Image difference information
struct ImageDiffInfo
{
    /// The number of pixels that differ
    UInt32 NumDiffPixels DEFAULT_INITIALIZER(0);

    /// The number of pixels that differ above the threshold
    UInt32 NumDiffPixelsAboveThreshold DEFAULT_INITIALIZER(0);

    /// The maximum difference between any two pixels
    UInt32 MaxDiff DEFAULT_INITIALIZER(0);

    /// The average difference between all pixels, not counting pixels that are equal
    float AvgDiff DEFAULT_INITIALIZER(0);

    /// The root mean square difference between all pixels, not counting pixels that are equal
    float RmsDiff DEFAULT_INITIALIZER(0);
};
typedef struct ImageDiffInfo ImageDiffInfo;


/// Attributes for ComputeImageDifference function
struct ComputeImageDifferenceAttribs
{
    /// Image width
    UInt32 Width DEFAULT_INITIALIZER(0);

    /// Image height
    UInt32 Height DEFAULT_INITIALIZER(0);

    /// A pointer to the first image data
    const void* pImage1 DEFAULT_INITIALIZER(nullptr);

    /// Number of channels in the first image
    UInt32 NumChannels1 DEFAULT_INITIALIZER(0);

    /// Row stride of the first image data, in bytes
    UInt32 Stride1 DEFAULT_INITIALIZER(0);

    /// A pointer to the second image data
    const void* pImage2 DEFAULT_INITIALIZER(nullptr);

    /// Number of channels in the second image
    UInt32 NumChannels2 DEFAULT_INITIALIZER(0);

    /// Row stride of the second image data, in bytes
    UInt32 Stride2 DEFAULT_INITIALIZER(0);

    /// Difference threshold
    UInt32 Threshold DEFAULT_INITIALIZER(0);

    /// A pointer to the difference image data.
    /// If null, the difference image will not be computed.
    void* pDiffImage DEFAULT_INITIALIZER(nullptr);

    /// Row stride of the difference image data, in bytes
    UInt32 DiffStride DEFAULT_INITIALIZER(0);

    /// Number of channels in the difference image.
    /// If 0, the number of channels will be the same as in the input images.
    UInt32 NumDiffChannels DEFAULT_INITIALIZER(0);

    /// Scale factor for the difference image
    float Scale DEFAULT_INITIALIZER(1.f);
};
typedef struct ComputeImageDifferenceAttribs ComputeImageDifferenceAttribs;

/// Computes the difference between two images
///
/// \param [in]  Attribs    Image difference attributes, see Diligent::ComputeImageDifferenceAttribs.
///
/// \return     The image difference information, see Diligent::ImageDiffInfo.
///
/// The difference between two pixels is calculated as the maximum of the
/// absolute differences of all channels. The average difference is the
/// average of all differences, not counting pixels that are equal.
/// The root mean square difference is calculated as the square root of
/// the average of the squares of all differences, not counting pixels that
/// are equal.
void DILIGENT_GLOBAL_FUNCTION(ComputeImageDifference)(const ComputeImageDifferenceAttribs& Attribs, ImageDiffInfo& ImageDiff);

} // namespace Diligent
