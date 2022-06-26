#pragma once
#include "move.hpp"

namespace sm
{

	struct MinMaxResult
	{

		Move move;
		float evaluation;
		int depth;
		int nodes;

	};

}
