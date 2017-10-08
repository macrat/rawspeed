/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2017 Roman Lebedev

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "RawSpeed-API.h" // for RawDecoder, RawParser, Buffer, CameraMetaData
#include <cstddef>        // for size_t
#include <cstdint>        // for uint8_t
#include <memory>         // for unique_ptr

// define this function, it is only declared in rawspeed:
// for fuzzing, do not want any threading.
extern "C" int __attribute__((const)) rawspeed_get_number_of_processor_cores() {
  return 1;
}

static const rawspeed::CameraMetaData metadata{};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size);

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size) {
  // static const CameraMetaData metadata(CMAKE_SOURCE_DIR "/data/cameras.xml");

  try {
    const rawspeed::Buffer buffer(Data, Size);
    rawspeed::RawParser parser(&buffer);
    auto decoder = parser.getDecoder(/*&metadata*/);

    decoder->applyCrop = false;
    decoder->interpolateBadPixels = false;
    decoder->failOnUnknown = false;
    // decoder->checkSupport(&metadata);

    decoder->decodeRaw();
    decoder->decodeMetaData(&metadata);
  } catch (rawspeed::RawspeedException&) {
    return 0;
  }

  return 0;
}
