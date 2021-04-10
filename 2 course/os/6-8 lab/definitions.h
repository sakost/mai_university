#pragma once

#include <lab.pb.h>
#include <cinttypes>
#include <vector>
#include <cstdlib>

#ifndef _WIN32
#include <unistd.h>
#else
#error "Windows is not supported"
#endif


using port_type = uint32_t;
const port_type default_port = 5555;

using node_id_t = int64_t;


struct ClientData{
    node_id_t client_id;
    pid_t pid;
};

enum class CreatedStatus: int64_t{
    OK,
    ALREADY_EXISTS,
    PARENT_NOT_FOUND,
    PARENT_IS_NOT_AVAILABLE,
    UNEXCPECTED_ERROR,
};

