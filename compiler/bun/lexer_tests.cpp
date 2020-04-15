
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
			Reader(BunString string) : m_string(std::move(string))
			{
				m_eof = m_string.empty();
			}
		private:
			BunString m_string;
			size_t m_position = 0;
			bool m_eof = false;

			// Interface Methods
			bool virtualEof() const final { return m_eof; }
			BunChar virtualPeek() const final { return m_string[m_position]; }
			void virtualNext() final
			{
				if (!m_eof)
				{
					++m_position;
					if (m_string.length() == m_position)
					{
						m_eof = true;
					}
				}
			}
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

		auto logs = std::vector<Lexer::Log>();
		auto tokens = std::vector<Token>();

		{
			auto reader = Reader(R"___(
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

			auto logger = Logger(logs);
			auto writer = Writer(tokens);

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
		assert("512" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("03" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("621" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("0" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("1" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("2" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert("0" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert("FFF" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert("a39" == it->content);

		next();
		assert(TokenType::HexadecimalIntegerLiteral == it->type);
		assert("34" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("32" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("91" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("4" == it->content);

		next();
		assert(TokenType::Operator == it->type);
		assert("/" == it->content);

		next();
		assert(TokenType::DecimalIntegerLiteral == it->type);
		assert("3" == it->content);

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
