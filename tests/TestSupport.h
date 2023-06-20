#pragma once
#include <iostream>
#include <fstream>
#include <exception>
#include <filesystem>
#include <memory>

namespace math3d::test {

    class TestSupport {
    public:
        static bool areFilesEqual(std::filesystem::path const& file1,
                                  std::filesystem::path const& file2) {

            std::ifstream ifs1(file1.string(), std::ios::binary | std::ios::ate);
            std::ifstream ifs2(file2.string(), std::ios::binary | std::ios::ate);
            if (!ifs1 || !ifs2) {
                throw std::runtime_error("Unable to open input files");
            }

            auto size1 = ifs1.tellg();
            auto size2 = ifs2.tellg();
            if (size1 != size2) {
                std::cerr << "File sizes don't match" << std::endl;
                return false;
            }

            ifs1.seekg(0, std::ios::beg);
            ifs2.seekg(0,std::ios::beg);

            auto buffer1 = std::unique_ptr<char[]>(new char[size1]);
            auto buffer2 = std::unique_ptr<char[]>(new char[size2]);

            ifs1.read(buffer1.get(), size1);
            ifs2.read(buffer2.get(), size1);

            for (size_t i = 0; i < size1; ++i) {
                if (buffer1[i] != buffer2[i]) {
                    std::cerr << "File contents are different" << std::endl;
                    return false;
                }
            }

            return true;
        }
    };

}