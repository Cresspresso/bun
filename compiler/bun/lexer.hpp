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
#include <functional>

#include "interface.hpp"

namespace Bun
{
	using Char = char8_t;
	using String = std::u8string;

	namespace Strings
	{
		std::string CharFromU8(String const& input);
		String U8FromChar(std::string const& input);
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

	String GetDefaultExceptionMessage();

	enum class LogSeverity
	{
		Info,
		Warning,
		Error,
	};

	class LexerException : public Exception
	{
	private:
		using Super = Exception;
	public:
		~LexerException() noexcept override = default;
		LexerException() : Super("Bun::LexerException") {}
		LexerException(char const* msg) : Super(msg) {}
		LexerException(std::string const& msg) : Super(msg) {}
	};

	struct LocationInFile
	{
		size_t line{};
		size_t column{};
		size_t position{};
		size_t length{};
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
		LocationInFile location{};
		String content{};
		TokenType type{};
	};

	namespace Lexer
	{
		struct Log
		{
			LogSeverity severity{};
			String message{};
			LocationInFile location{};
			Token token{};
		};

		class ILogger
		{
			BUN_DECLARE_INTERFACE(ILogger);
		private:
			virtual void virtualLog(Log log) = 0;
			virtual void virtualCatastrophe() = 0;
			virtual String virtualGetExceptionMessage() = 0;
		public:
			void log(Log log) noexcept;
			void catastrophe() noexcept;
			String getExceptionMessage();
		};

		class DefaultLogger final : public ILogger
		{
			BUN_INTERFACE_FINAL_DEFAULT(DefaultLogger);
		public:
			DefaultLogger() = default;
		private:
			void virtualLog(Log log) final;
			void virtualCatastrophe() final;
			String virtualGetExceptionMessage() final;
		};

		class IReader
		{
			BUN_DECLARE_INTERFACE(IReader);
		private:
			virtual bool virtualEof() const = 0;
			virtual Char virtualPeek() const = 0;
			virtual void virtualNext() = 0;
		public:
			bool eof() const { return virtualEof(); }
			void next() { return virtualNext(); }
			Char peek() const;
		};

		class IWriter
		{
			BUN_DECLARE_INTERFACE(IWriter);
		private:
			virtual void virtualSubmit(Token token) = 0;
		public:
			void submit(Token token) { return virtualSubmit(std::move(token)); }
		};

		struct Args
		{
			std::reference_wrapper<ILogger> logger;
			std::reference_wrapper<IReader> reader;
			std::reference_wrapper<IWriter> writer;
			LocationInFile initialLocation = {};
		};

		void run(Args args);
	}
}
