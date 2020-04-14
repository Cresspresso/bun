
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

#include "lexer.hpp"

#include "lexer_tests.hpp"

namespace BunUnitTests
{
	void testLexer1()
	{
		using namespace Bun;
		using namespace Bun::Lexer;

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
				assert(TokenType::Unknown != token.type);
				getStream().push_back(std::move(token));
			}
		};

		class Logger final : public ILogger
		{
			BUN_INTERFACE_FINAL_DEFAULT(Logger);
		public:
			using Stream = std::vector<Log>;
			Logger(Stream& stream) : m_stream(stream) {}
			Stream& getStream() const { return m_stream.get(); }
		private:
			std::reference_wrapper<Stream> m_stream;
			// Interface Methods
			void virtualLog(Log log) final
			{
				assert(LogSeverity::Error != log.severity);
				assert(LogSeverity::Warning != log.severity);
				getStream().push_back(std::move(log));
			}
			void virtualCatastrophe() final
			{
				assert(false);
			}
			BunString virtualGetExceptionMessage() final
			{
				return getDefaultExceptionMessage();
			}
		};

		auto sourceStream = std::basic_istringstream<BunChar>(u8R"___(
512
03
	621
		0
1
2 0x0 0xFFF
0xa39 // comment
0X34
	/*
mult
line
comment
*/ 32
	/*outer
a /*inner*/ b
end*/
91

/////
4

/
3)___");

		auto logs = std::vector<Lexer::Log>();
		auto tokens = std::vector<Token>();

		{
			auto logger = Logger(logs);
			auto writer = Writer(tokens);
			auto reader = Reader(sourceStream);
			Lexer::run({
				logger,
				reader,
				writer,
				});
		}

		assert(logs.empty());

		auto it = tokens.cbegin();
		auto const end = tokens.cend();
		assert(end != it);
		auto const next = [&] {
			++it;
			assert(end != it);
		};

		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"512" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"03" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"621" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"0" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"1" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"2" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert(u8"0" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert(u8"FFF" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert(u8"a39" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert(u8"34" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"32" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"91" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"4" == it->content);

		next();
		assert(TokenType::Operator == it->type);
		assert(u8"/" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert(u8"3" == it->content);

		++it;
		assert(end == it);

#if 0

		printf_s("\nTokens:\n");
		for (auto const& token : tokens)
		{
			std::string const type = [&] {
				switch (token.type)
				{
				default:
				case TokenType::Unknown: return "Unknown";
				case TokenType::Operator: return "Operator";
				case TokenType::DecimalIntegerLiteral: return "DecimalIntegerLiteral";
				case TokenType::HexadecimalIntegerLiteral: return "HexadecimalIntegerLiteral";
				case TokenType::CloseBrace: return "CloseBrace";
				case TokenType::OpenBrace: return "OpenBrace";
				}
			}();

			std::string content = StringConversion::CharFromU8(token.content);
			if (content.find('\n') != content.npos)
			{
				content = '`' + content + '`';
			}

			printf_s("\t%s: %s\n",
				type.c_str(),
				content.c_str());
		}

		printf_s("\nLogs:\n");
		for (auto const& log : logs)
		{
			printf_s("\tlocation:%d:%d  :  %s\n",
				(int)log.location.line,
				(int)log.location.column,
				StringConversion::CharFromU8(log.message).c_str());
		}

#endif
	}
}
