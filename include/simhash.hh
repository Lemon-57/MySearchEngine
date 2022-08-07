#ifndef simhash_hh_
#define simhash_hh_

#include "../include/simhash/Simhasher.hpp"

//this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN 

using namespace simhash;

class SimHash{
public:
    SimHash();
    ~SimHash();
    uint64_t getSim(const string doc1);

private:
    Simhasher _simhasher;
};

#endif 