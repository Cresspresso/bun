
#include <cassert>

#include "lexer_tests.hpp"
#include "pipeline_tests.hpp"

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
	run(testLexer1);
	run(testPipeline1);
}
