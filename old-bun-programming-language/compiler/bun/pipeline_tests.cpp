
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
			Reader(Stream& stream) : m_stream(stream) { virtualNext(); }
			Stream& getStream() const { return m_stream.get(); }
		private:
			std::reference_wrapper<Stream> m_stream;
			bool m_eof;
			BunChar m_peek;
			// Interface Methods
			bool virtualEof() const final { return m_eof; }
			BunChar virtualPeek() const final { return m_peek; }
			void virtualNext() final { m_eof = bool(getStream() >> m_peek); }
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
			}
		};

		class Logger final : public ILogger
		{
			BUN_INTERFACE_FINAL_DEFAULT(Logger);
		public:
			Logger(std::string filename) : m_filename(filename) {}
		private:
			std::string m_filename;

			void virtualLog(Log log) final
			{
				std::cerr << m_filename << ":"
					<< log.location.line << ":"
					<< log.location.column << "  "
					<< Strings::charFromBun(log.message) << ": `"
					<< Strings::charFromBun(log.token.content) << "`\n";
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

		void run(std::string testname, std::basic_istream<BunChar>& inputStream, std::basic_istream<char>& expectedOutputStream)
		{
			assert(inputStream);
			assert(expectedOutputStream);

			auto tokens = std::vector<Token>();

			{
				auto reader = Reader(inputStream);
				auto writer = Writer(tokens);
				auto logger = Logger(testname);
				Lexer::run({ logger, reader, writer });
			}

			std::string actual = R"__(
#include <cstdio>

int main()
{
	printf_s("hello world\n");
}
)__";
			using It = std::istreambuf_iterator<char>;
			assert((std::equal(It(expectedOutputStream), It(), actual.begin(), actual.end())));
		}
	}

	void testPipeline(std::string testname, std::basic_istream<BunChar>& inputStream, std::basic_istream<char>& expectedOutputStream)
	{
		return TestPipeline::run(testname, inputStream, expectedOutputStream);
	}

	void testPipeline(std::string inputFilename, std::string expectedOutputFilename)
	{
		auto inputStream = std::basic_ifstream<BunChar>(inputFilename);
		auto expectedOutputStream = std::ifstream(expectedOutputFilename);
		return testPipeline(inputFilename, inputStream, expectedOutputStream);
	}

	void testPipeline1() { return testPipeline("pipeline1input.bun", "pipeline1expected.cpp"); }
}
