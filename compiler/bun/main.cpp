
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

#include "lexer.hpp"

void UnitTest1()
{
	using namespace Bun;
	using namespace Bun::Lexing;

	/*
	//	This is how to use the lexer.
	//	Provide the sandbox methods for the Lex function to use.
	*/
	class Lexer final : public Bun::Lexing::Lexer
	{
	public:
		~Lexer() noexcept final = default;
		Lexer(
			std::basic_istream<Char>& reader,
			std::vector<LogMessage>& logs,
			std::vector<Token>& tokens
			)
			: m_reader(reader)
			, m_logs(logs)
			, m_tokens(tokens)
		{
		}

	private:
		// Fields
		std::reference_wrapper<std::basic_istream<Char>> m_reader;
		std::reference_wrapper<std::vector<LogMessage>> m_logs;
		std::reference_wrapper<std::vector<Token>> m_tokens;

	private:
		// Interface Methods
		bool VirtualEof() final { return m_reader.get().eof(); }
		Char VirtualPeek() final { return m_reader.get().peek(); }
		void VirtualNext() final { m_reader.get().get(); }

		void VirtualLog(LogMessage log) final
		{
			assert(Severity::Error != log.severity);
			assert(Severity::Warning != log.severity);
			m_logs.get().push_back(std::move(log));
		}

		void VirtualSubmit(Token token) final
		{
			assert(TokenType::Unknown != token.type);
			m_tokens.get().push_back(std::move(token));
		}
	};

	auto reader = std::basic_istringstream<Char>(u8R"___(
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

	auto logs = std::vector<LogMessage>();
	auto tokens = std::vector<Token>();
	
	Lexer(
		reader,
		logs,
		tokens
		).Lex();

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

int main()
{
	UnitTest1();
}
