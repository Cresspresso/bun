
#include <cassert>

#include "lexer_unit_tests.hpp"

namespace
{
	template<class F>
	void run(F f)
	{
		try
		{
			f();
		}
		catch (...)
		{
			assert(false);
		}
	}
}

int main()
{
	using namespace BunUnitTests;
	run(unitTestLexer1);
}
