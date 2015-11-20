#pragma once
#include <string>
#include <vector>
#include "FBMathLib/Transformation.h"
namespace fastbird{
	typedef std::pair<std::string, Transformation> AUXILIARY;
	typedef std::vector< AUXILIARY > AUXILIARIES;
}