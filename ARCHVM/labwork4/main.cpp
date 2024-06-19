#include <omp.h>
#include <iostream>
#include "hit.h"
#include <string>
#include <cstring>
#include <fstream>
#include <limits>

float myRand(uint32_t& seed, float min, float max) {
    uint32_t hi, lo;
    lo = 16807 * (seed & 0xFFFF);
    hi = 16807 * (seed >> 16);

    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;

    if (lo > 0x7FFFFFFF) {
        lo -= 0x7FFFFFFF;
    }

    return min + (max - min) * (static_cast<float>(seed = lo) / static_cast<float>(0x7FFFFFFF));
}

float getVolumeOmp(int number_threads, int number_iters) {
    const float* range = get_axis_range();
    uint64_t all_hit = 0;
    omp_set_num_threads(number_threads);
    double start = omp_get_wtime();
#pragma omp parallel
    {
    uint32_t next = omp_get_thread_num() * omp_get_wtime() + omp_get_wtime();
    uint64_t local_hit = 0;
    float x, y, z;

#pragma omp for schedule(static)
    for (long long i = 0; i < number_iters; ++i) {
        x = myRand(next, range[0], range[1]);
        y = myRand(next, range[2], range[3]);
        z = myRand(next, range[4], range[5]);
        if (hit_test(x, y, z)) {
            ++local_hit;
        }
    }
#pragma omp critical
    all_hit += local_hit;
    }

    double finish = omp_get_wtime();
    
    printf("Time (%i thread(s)): %g ms\n", number_threads,  (finish - start) * 1000);

    double volume = ((range[1] - range[0]) * (range[3] - range[2]) * (range[5] - range[4]) * all_hit) / number_iters;
    return volume;
}

float getVolume(int number_iters) {
    const float* range = get_axis_range();

    int hit = 0;
    
    uint32_t next = omp_get_thread_num() * omp_get_wtime() + omp_get_wtime();

    double start = omp_get_wtime();
    float x, y, z;

    for (long long i = 0; i < number_iters; ++i) {
        x = myRand(next, range[0], range[1]);
        y = myRand(next, range[2], range[3]);
        z = myRand(next, range[4], range[5]);
        if (hit_test(x, y, z)) {
            ++hit;
        }
    }

    double finish = omp_get_wtime();

    printf("Time (%i thread(s)): %g ms\n", 0,  (finish - start)*1000);

    double volume = ((range[1] - range[0]) * (range[3] - range[2]) * (range[5] - range[4]) * hit) / number_iters;
    return volume;
}

struct parseArg {
    std::string input_file;
    std::string output_file;
    int num_threads = 0;
};

void parse(int argc, char** argv, parseArg& parse_arg) {
    for (int i = 1; i < argc; ++i) {
        if (!std::strcmp(argv[i], "--input")) {
            parse_arg.input_file = argv[++i];
        }
        else if (!std::strcmp(argv[i], "--output")) {
            parse_arg.output_file = argv[++i];
        }
        else if (!std::strcmp(argv[i], "--no-omp")) {
            parse_arg.num_threads = 0;
        }
        else if (!std::strcmp(argv[i], "--omp-threads")) {
            if (!std::strcmp(argv[i+1], "default")) {
                parse_arg.num_threads = omp_get_max_threads();
            }
            else {
                parse_arg.num_threads = std::stoi(argv[++i]);
            }
        }
    }
}

int getNumberIters(std::string& input_file) {
    int tmp;

    std::ifstream file(input_file);
    file >> tmp;
    file.close();

    return tmp;
}

void putVolume(std::string& output_file, double volume) {
    std::ofstream file(output_file);

    file << volume << '\n';

    file.close();
}

int main(int argc, char** argv) {
    parseArg parse_arg;
    parse(argc, argv, parse_arg);

    int number_iters = getNumberIters(parse_arg.input_file);
    double volume = parse_arg.num_threads == 0 ? getVolume(number_iters) : getVolumeOmp(parse_arg.num_threads, number_iters);
    putVolume(parse_arg.output_file, volume);
    return 0;
}
