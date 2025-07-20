/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   digitalHellLogo_png;
    const int            digitalHellLogo_pngSize = 36663;

    extern const char*   HalfBoldPixel72rw3_ttf;
    const int            HalfBoldPixel72rw3_ttfSize = 27048;

    extern const char*   PixelOperatorBold_ttf;
    const int            PixelOperatorBold_ttfSize = 16984;

    extern const char*   PixelOperator_ttf;
    const int            PixelOperator_ttfSize = 17272;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 4;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
