
#include <fstream>
#include <sstream>
#include <cassert>

#include "lexer.hpp"
#include "parser.hpp"
//#include "coder.hpp"

#include "pipeline_tests.hpp"

namespace BunUnitTests
{
	namespace
	{
		using namespace Bun;
		using namespace Bun::Lexer;
	}

	namespace TestPipeline
	{
		class Reader final : public IReader
		{
			BUN_INTERFACE_FINAL_DEFAULT(Reader);
		public:
			using Stream = std::basic_istream<BunChar>;
			Reader(Stream& stream) : m_stream(stream) {}
			Stream& getStream() const { return m_stream.get(); }
		private:
			std::reference_wrapper<Stream> m_stream;
			// Interface Methods
			bool virtualEof() const final { return getStream().eof(); }
			BunChar virtualPeek() const final { return getStream().peek(); }
			void virtualNext() final { getStream().ignore(); }
		};

		class Writer final : public IWriter
		{
			BUN_INTERFACE_FINAL_DEFAULT(Writer);
		public:
			using Stream = std::vector<Token>;
			Writer(Stream& stream) : m_stream(stream) {}
			Stream& getStream() const { return m_stream.get(); }
		private:
			std::reference_wrapper<Stream> m_stream;
			// Interface Methods
			void virtualSubmit(Token token) final
			{
				getStream().push_back(token);
				assert(TokenType::Unknown != token.type);
			}
		};

		class Logger final : public ILogger
		{
			BUN_INTERFACE_FINAL_DEFAULT(Logger);
		public:
			Logger() = default;
		private:
			void virtualLog(Log log) final
			{
				std::cerr << Strings::charFromBun(log.message) << "\n";
				assert(LogSeverity::Error != log.severity);
				assert(LogSeverity::Warning != log.severity);
			}
			void virtualCatastrophe() final
			{
				std::cerr << "Error: Catastrophe\n";
				assert(false);
			}
			BunString virtualGetExceptionMessage() final
			{
				return getDefaultExceptionMessage();
			}
		};

		void run(std::basic_istream<char8_t>& inputStream, std::basic_istream<char>& expectedOutputStream)
		{
			assert(inputStream);
			assert(expectedOutputStream);

			auto tokens = std::vector<Token>();

			{
				auto reader = Reader(inputStream);
				auto writer = Writer(tokens);
				auto logger = Logger();
				Lexer::run({ logger, reader, writer });
			}

			auto actual = std::ostringstream();
			actual << R"__(
#include <cstdio>

int main()
{
	printf_s("hello world\n");
}
)__";

			auto const expected = std::string(std::istreambuf_iterator<char>(expectedOutputStream), {});
			assert(expected == actual.str());
		}
	}

	void testPipeline(std::basic_istream<char8_t>& inputStream, std::basic_istream<char>& expectedOutputStream)
	{
		return TestPipeline::run(inputStream, expectedOutputStream);
	}

	void testPipeline(std::string inputFilename, std::string expectedOutputFilename)
	{
		auto inputStream = std::basic_ifstream<char8_t>(inputFilename);
		auto expectedOutputStream = std::ifstream(expectedOutputFilename);
		return testPipeline(inputStream, expectedOutputStream);
	}

	void testPipeline1() { return testPipeline("pipeline1input.bun", "pipeline1expected.cpp"); }
}
