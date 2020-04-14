
#include <cassert>

#include "lexer_unit_tests.hpp"

namespace
{
	template<class F>
	void Run(F f)
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
	Run(UnitTestLexer1);
}
