#ifndef LOAD_GPH_H
#define LOAD_GPH_H

#include <memory>
#include <string>
#include "graph.h"

namespace chirsz {
extern std::unique_ptr<Graph<std::string>> load_gph(const char* filename);
}

#endif
