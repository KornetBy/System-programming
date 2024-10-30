#include <windows.h>
#include <iostream>

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    std::cout << "Number of processors: " << sysInfo.dwNumberOfProcessors << std::endl;
    std::cout << "Processor type: " << sysInfo.dwProcessorType << std::endl;
    std::cout << "Page size: " << sysInfo.dwPageSize << std::endl;

    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(memoryInfo);
    GlobalMemoryStatusEx(&memoryInfo);

    std::cout << "Total physical memory: " << memoryInfo.ullTotalPhys << std::endl;
    std::cout << "Available physical memory: " << memoryInfo.ullAvailPhys << std::endl;

    return 0;
}
