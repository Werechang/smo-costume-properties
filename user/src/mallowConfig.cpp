#include <mallow/mallow.hpp>
#include <nn/fs.h>
#include <nn/fs/fs_directories.h>
#include <nn/fs/fs_types.h>

namespace mallow::config {
    const char* path = "sd:/atmosphere/contents/0100000000010000/mallow.json";
    const char* pathEmu = "sd:/mallow.json";
    const char* defaultConfig = R"(
{
    "logger": {
        "enable": false,
        "reconnect": false,
        "ip": "192.168.1.110",
        "port": 3080
    }
}
    )";

    Allocator* getAllocator() {
        static DefaultAllocator allocator = {};
        return &allocator;
    }
    bool isEmu(){
        nn::fs::DirectoryEntryType type;
        nn::fs::GetEntryType(&type, "sd:/atmosphere/package3");
        return type != nn::fs::DirectoryEntryType_File;
    }
}
