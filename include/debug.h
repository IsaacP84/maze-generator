#pragma once
#include <cassert>
#include <iostream>
#ifdef DEBUG
#  define LOG(x) std::cout << #x << ": " << x << std::endl;
#  define ASSERT(x) assert(x)
#else
#  define LOG(x)
#  define ASSERT(x) 
#endif
