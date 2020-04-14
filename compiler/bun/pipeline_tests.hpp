#pragma once

#include <string>
#include <iostream>

namespace BunUnitTests
{
	void testPipeline(std::basic_istream<char8_t>& inputStream, std::basic_istream<char>& expectedOutputStream);
	void testPipeline(std::string inputFilename, std::string expectedOutputFilename);

	void testPipeline1();
}
