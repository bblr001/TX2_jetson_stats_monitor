#include <chrono>
#include <thread>

#include "jetson_stats.hpp"

int main(int argc, char *argv[]) {
    

    //for (int i = 0; i < 10; i++) 
    while(1)
    {
        jetson_stats::get_info();
/*
        std::cout << "RAM: " << info.memory_used << "/" << info.memory_total
                  << " MB, GPU: " << info.gpu_utilization << " %, CPU: " << info.cpu_utilization
                  << " %" << std::endl;
*/
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //jetson_stats::showdown();
    return 0;
}
