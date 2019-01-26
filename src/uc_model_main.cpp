// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"

#include <algorithm>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>

#include <uc/lip/lip.h>
#include <uc/gx/lip/model.h>
#include <uc/lip/tools_time_utils.h>
#include <uc/img/img.h>
#include <uc/os/windows/com_initializer.h>
#include <uc/lzham/lzham_compressor.h>
#include <uc/import/texture/texture.h>

#include "uc_model_command_line.h"

namespace uc
{
    namespace import
    {
        void convert_texture( const std::string& input_file_name, const std::string& output_file_name, const std::string& texture_format )
        {
            auto storage = texture::string_to_storage_format(texture_format);
            auto view = import::texture::string_to_view_format(texture_format);

            if (storage == lip::storage_format::unknown)
            {
                uc::lip::texture2d m = texture::create_texture_2d(input_file_name);
                uc::lip::serialize_object(&m, output_file_name, uc::lzham::compress_buffer);
            }
            else
            {
                uc::lip::texture2d m = texture::create_texture_2d(input_file_name, storage, view);
                uc::lip::serialize_object(&m, output_file_name, uc::lzham::compress_buffer);
            }
        }
    }
}

inline std::string get_environment()
{
    return "x64";
}


int32_t main(int32_t argc, const char* argv[])
{
    using namespace uc::import;
    using namespace uc::model;

    std::string input_model_error = "uc_dev_texture_r.exe";

    try
    {
        uc::os::windows::com_initializer           com;
        texture::compressonator::initializer       compressonator;

        //
        // ShutdownBCLibrary - Shutdown the BC6H or BC7 library
        //
        
        auto&& om   = build_option_map(argc, argv);
        auto&& vm   = std::get<0>(om);
        auto&& desc = std::get<1>(om);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 0;
        }

        auto input_model            = get_input_texture(vm);
        auto output_model           = get_output_texture(vm);
        auto texture_format         = get_texture_format(vm);

        std::cout << "building texture (" << get_environment() << ") " << std::endl;

        std::cout << "Building texture:" << input_model << std::endl;
        convert_texture(input_model, output_model, texture_format);
    }

    catch (const std::exception& e)
    {
        std::cerr << input_model_error << '(' << 0 << ',' << 0 << ')' << ":error 12345: missing " << e.what() << "\r\n";
        return -1;
    }

    return 0;
}

