#pragma once
/*md

# lexer.hpp

Elijah Shadbolt

6 April 2020

## Summary

Lexer for the Bun programming language.
Reads UTF-8 characters from a stream and transforms them to a sequence of tokens.

## Usage

1. Implement a final class extending `Bun::Lexing::Lexer`.
2. Call member function `Lex()`.

*/

#include <string>
#include <stdexcept>

#include "interface.hpp"

namespace Bun
{
	namespace Strings
	{
		using Char = char8_t;
		using String = std::u8string;

		std::string CharFromU8(Strings::String const& input);
		Strings::String U8FromChar(std::string const& input);
	}

	class Exception : public std::runtime_error
	{
	private:
		using Super = std::runtime_error;
	public:
		~Exception() noexcept override = default;
		Exception() : Super("Bun::Exception") {}
		Exception(char const* msg) : Super(msg) {}
		Exception(std::string const& msg) : Super(msg) {}
	};

	namespace Lexing
	{
		using Bun::Strings::Char;
		using Bun::Strings::String;

		class LexerException : public Exception
		{
		private:
			using Super = Exception;
		public:
			~LexerException() noexcept override = default;
			LexerException() : Super("Bun::Lexing::LexerException") {}
			LexerException(char const* msg) : Super(msg) {}
			LexerException(std::string const& msg) : Super(msg) {}
		};

		struct Location
		{
			size_t line{};
			size_t column{};
			size_t position{};
		};

		enum class TokenType
		{
			Unknown,
			Operator, // `/`
			DecimalIntegerLiteral, // the 9's in `9999`
			HexadecimalIntegerLiteral, // the F's in `0xFFFF`
			OpenBrace,
			CloseBrace,
		};

		struct Token
		{
			Location location{};
			size_t length{};
			Strings::String content{};
			TokenType type{};
		};

		enum class Severity
		{
			Info,
			Warning,
			Error,
		};

		struct LogMessage
		{
			Token token{};
			Location location{};
			Severity severity{};
			Strings::String message{};
		};

		/*
		//	Class that represents all the callback arguments provided to the lexer.
		//	The virtual methods are sandbox methods used by the member function Lex.
		*/
		class Lexer
		{
			BUN_DECLARE_INTERFACE(Lexer);
		private:
			// true if reader stream is at end of file
			virtual bool VirtualEof() = 0;
			// reader stream current char
			virtual Strings::Char VirtualPeek() = 0;
			// advance reader stream by one char
			virtual void VirtualNext() = 0;

			// logger handles a message being logged
			virtual void VirtualLog(LogMessage log) = 0;
			// lippincott function that returns a message about the current exception being handled
			virtual Strings::String VirtualGetExceptionMessage();

			// saves a finished token
			virtual void VirtualSubmit(Token token) = 0;

		public:
			bool Eof() { return VirtualEof(); }
			Char Peek() { return VirtualPeek(); }
			void Next() { return VirtualNext(); }

			void Log(LogMessage log) { return VirtualLog(std::move(log)); }
			String GetExceptionMessage() { return VirtualGetExceptionMessage(); }

			void Submit(Token token) { return VirtualSubmit(std::move(token)); }

			void Lex(Location initialLocation = Location());
		};
	}
}
