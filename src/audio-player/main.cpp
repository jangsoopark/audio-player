/** @file paex_read_write_wire.c
    @ingroup examples_src
    @brief Tests full duplex blocking I/O by passing input straight to output.
    @author Bjorn Roche. XO Audio LLC for Z-Systems Engineering.
    @author based on code by: Phil Burk  http://www.softsynth.com
    @author based on code by: Ross Bencina rossb@audiomulch.com
*/
/*
 * $Id: patest_read_record.c 757 2004-02-13 07:48:10Z rossbencina $
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however,
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also
 * requested that these non-binding requests be included along with the
 * license above.
 */


#include <portaudio.h>
#include <algorithm>

#include <iostream>

#include "format.h"

#ifdef __cplusplus
extern "C"
{
#include <libavcodec/avcodec.h>
}
#endif


/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE       (44100)
#define FRAMES_PER_BUFFER   (44100)
#define NUM_SECONDS          (100)
/* #define DITHER_FLAG     (paDitherOff)  */
#define DITHER_FLAG           (0)



/*******************************************************************/
int main(void)
{
    PaStreamParameters input_parameters;
    PaStreamParameters output_parameters;
    
    const PaDeviceInfo* input_device_info = nullptr;
    const PaDeviceInfo* output_device_info = nullptr;

    PaStream* stream = nullptr;

    PaError err;

    err = Pa_Initialize();
    if (err != paNoError)
        return -1;

    // Input Device
    input_parameters.device = Pa_GetDefaultInputDevice();
    input_device_info = Pa_GetDeviceInfo(input_parameters.device);

    std::cout << "Input device # " << input_parameters.device << std::endl;
    std::cout << "\t Name:  " << input_device_info->name << std::endl
            << "\t LL: " << input_device_info->defaultLowInputLatency << std::endl
            << "\t HL: " << input_device_info->defaultHighInputLatency << std::endl;

    // Output Device
    output_parameters.device = Pa_GetDefaultOutputDevice();
    output_device_info = Pa_GetDeviceInfo(output_parameters.device);

    std::cout << "Output device # " << output_parameters.device << std::endl;
    std::cout << "\t Name:  " << output_device_info->name << std::endl
            << "\t LL: " << output_device_info->defaultLowOutputLatency << std::endl
            << "\t HL: " << output_device_info->defaultHighOutputLatency << std::endl;       

    int num_channels = std::min(input_device_info->maxInputChannels, output_device_info->maxOutputChannels);
    std::cout << "Num channels: " << num_channels << std::endl;

    // Input Parameter
    input_parameters.channelCount = num_channels;
    input_parameters.sampleFormat = PA_SAMPLE_TYPE;
    input_parameters.suggestedLatency = input_device_info->defaultHighInputLatency;
    input_parameters.hostApiSpecificStreamInfo = nullptr;

    // Output Parameter
    output_parameters.channelCount = num_channels;
    output_parameters.sampleFormat = PA_SAMPLE_TYPE;
    output_parameters.suggestedLatency = output_device_info->defaultHighInputLatency;
    output_parameters.hostApiSpecificStreamInfo = nullptr;

    // ????????????????????????????????????????????????????
    err = Pa_OpenStream(
        &stream,
        &input_parameters,
        &output_parameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,
        nullptr,
        nullptr
    );
    if (err != paNoError)
        return -1;

    int num_bytes = FRAMES_PER_BUFFER * num_channels * SAMPLE_SIZE;
    char* sample_block = new char [num_bytes];
    if (!sample_block)
        return -1;

    memset(sample_block, (int)SAMPLE_SILENCE, num_bytes);
    err = Pa_StartStream(stream);
    if (err != paNoError)
        return -1;

    std::cout << "fdzz" << std::endl;

    // while (true)
    for(int i=0; i<(NUM_SECONDS*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i )
    {
        err = Pa_WriteStream(stream, sample_block, FRAMES_PER_BUFFER);
        if (err)
            break;
        err = Pa_ReadStream(stream, sample_block, FRAMES_PER_BUFFER);
        if (err)
            break;
    }
    err = Pa_StopStream(stream);
    if (err != paNoError)
        return -1;

    delete [] sample_block;
    
    Pa_Terminate();

    return 0;
}
