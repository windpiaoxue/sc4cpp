#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include <Windows.h>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("Invalid parameters\n");
        return 1;
    }
    std::ifstream file(argv[1], std::ios::binary);
    if (!file.is_open()) {
        printf("Failed to open the file: %s\n", strerror(errno));
        return 1;
    }
    std::string sc(std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{});
    void* buffer = VirtualAlloc(
        NULL,
        sc.length(),
        MEM_RESERVE | MEM_COMMIT,
        PAGE_EXECUTE_READWRITE);
    if (buffer == NULL) {
        printf("Failed to allocate memory: error=%u\n", GetLastError());
        return 1;
    }
    memcpy(buffer, sc.c_str(), sc.length());
    ((void(*)())buffer)();
}
