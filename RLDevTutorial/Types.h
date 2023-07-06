#pragma once

#include <any>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <time.h>
#include <vector>

#include "boost/json.hpp"
#include "boost/algorithm/string.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "boost/lexical_cast.hpp" 


#include "noise/noise.h"
#include "noise/noiseutils.h"


namespace json = boost::json;

typedef std::pair<int, int> point;

typedef unsigned int uint;
typedef unsigned char byte;