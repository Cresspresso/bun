
#include <cstdio>
#include <string_view>
#include <memory>
#include <cassert>
#include <set>

#include "lexer.hpp"

namespace Bun
{
	namespace Strings
	{
		std::string charFromBun(BunString const& input)
		{
			std::string output;
			for (BunChar const c : input)
			{
				output.push_back((char)c);
			}
			return output;
		}

		BunString bunFromChar(std::string const& input)
		{
			BunString output;
			for (char const c : input)
			{
				output.push_back((BunChar)c);
			}
			return output;
		}
	}

	BunString getDefaultExceptionMessage()
	{
		BunString message = "internal exception: ";

		try
		{
			throw;
		}
		catch (Exception const& e)
		{
			message += "Lexing exception: ";
			message += Strings::bunFromChar(e.what());
		}
		catch (std::exception const& e)
		{
			message += "unknown exception: ";
			message += Strings::bunFromChar(e.what());
		}
		catch (...)
		{
			message += "unknown exception of unknown type";
		}

		return message;
	}

	namespace Lexer
	{
		namespace
		{
			constexpr char const* DefaultLogger_rootString = "Bun::Lexer::DefaultLogger";
		}

		void ILogger::log(Log log) noexcept
		{
			try
			{
				virtualLog(std::move(log));
			}
			catch (...)
			{
				catastrophe();
			}
		}

		void ILogger::catastrophe() noexcept
		{
			try
			{
				virtualCatastrophe();
			}
			catch (...)
			{
				// discard
			}
		}

		BunString ILogger::getExceptionMessage()
		{
			return virtualGetExceptionMessage();
		}

		void DefaultLogger::virtualLog(Log log)
		{
			std::string const severity = [&]() -> char const* {
				switch (log.severity)
				{
				default:
				case LogSeverity::Error: return "Error";
				case LogSeverity::Warning: return "Warning";
				case LogSeverity::Info: return "i";
				}
			}();

			std::string const message = Strings::charFromBun(log.message);

			fprintf_s(stdout, "%s: %s: %s.\n",
				DefaultLogger_rootString,
				severity.c_str(),
				message.c_str());
		}

		void DefaultLogger::virtualCatastrophe()
		{
			fprintf_s(stderr, "%s: Error: Failed to log a message.\n",
				DefaultLogger_rootString);
		}

		BunString DefaultLogger::virtualGetExceptionMessage()
		{
			return Bun::getDefaultExceptionMessage();
		}

		BunChar IReader::peek() const
		{
			assert(!eof());
			if (eof())
			{
				throw LexerException("unexpected end of file");
			}
			return virtualPeek();
		}

		namespace
		{
			struct IsCharacterTools
			{
				static inline bool AnyOf(BunChar const c, std::string_view const s) { return s.npos != s.find(c); }
				static inline bool InClosedInterval(BunChar const c, BunChar const low, BunChar const hi) { return c >= low && c <= hi; }
			};

			struct IsCharacter : public IsCharacterTools
			{
				using Super = IsCharacterTools;
				using Super::InClosedInterval;
				using Super::AnyOf;

				static inline bool LowercaseAscii(BunChar const c) { return InClosedInterval(c, 'a', 'z'); }
				static inline bool UppercaseAscii(BunChar const c) { return InClosedInterval(c, 'A', 'Z'); }

				static inline bool Decimal(BunChar const c) { return InClosedInterval(c, '0', '9'); }
				static inline bool Zero(BunChar const c) { return '0' == c; }
				static inline bool HighHex(BunChar const c) { return InClosedInterval(c, 'a', 'f') || InClosedInterval(c, 'A', 'F'); }
				static inline bool Hex(BunChar const c) { return Decimal(c) || HighHex(c); }
				static inline bool SpecifyHex(BunChar const c) { return AnyOf(c, "xX"); }
				static inline bool NumberWhitespace(BunChar const c) { return '_' == c; }
				static inline bool NumberFractionSeparator(BunChar const c) { return '.' == c; }

				static inline bool WhitespaceExcludingNewline(BunChar const c) { return AnyOf(c, " \t\v"); }
				static inline bool Newline(BunChar const c) { return AnyOf(c, "\n\r;"); }
				static inline bool WhitespaceIncludingNewline(BunChar const c) { return WhitespaceExcludingNewline(c) || Newline(c); }

				static inline bool OpenBrace(BunChar const c) { return '{' == c; }
				static inline bool CloseBrace(BunChar const c) { return '}' == c; }

				static inline bool BranchCommentOrDivide(BunChar const c) { return '/' == c; }
				static inline bool SpecifyMultiLineComment(BunChar const c) { return '*' == c; }
				static inline bool SpecifySingleLineComment(BunChar const c) { return '/' == c; }

				static inline bool identifierWhitespace(BunChar const c) { return '_' == c; }
				static inline bool startIdentifier(BunChar const c) { return LowercaseAscii(c) || UppercaseAscii(c) || identifierWhitespace(c); }
				static inline bool identifierTail(BunChar const c) { return startIdentifier(c) || Decimal(c); }

				static inline bool stringLiteral(BunChar const c) { return c == '"'; }
			};

			class LexerImpl
			{
			public:
				~LexerImpl() noexcept = default;
				LexerImpl(LexerImpl&&) noexcept = delete;
				LexerImpl& operator=(LexerImpl&&) noexcept = delete;
				LexerImpl(LexerImpl const&) = delete;
				LexerImpl& operator=(LexerImpl const&) = delete;

				LexerImpl(Lexer::Args& functionality)
					: m_functionality(functionality)
					, m_currentLocation(std::move(functionality.initialLocation))
					, m_currentToken{
						.location = m_currentLocation
					}
				{
					nextEof();
				}

			private:
				std::reference_wrapper<Lexer::Args> m_functionality;
				LocationInFile m_currentLocation;
				Token m_currentToken;
				bool m_passedNewline = false;
				bool m_eof{};
				BunChar m_peek{};

			private:
				// METHODS

				Lexer::Args& getFunctionality() { return m_functionality.get(); }
				ILogger& getLogger() { return getFunctionality().logger.get(); }
				IReader& getReader() { return getFunctionality().reader.get(); }
				IWriter& getWriter() { return getFunctionality().writer.get(); }

				BunString getExceptionMessage() { return getLogger().getExceptionMessage(); }
				void catastrophe() noexcept { return getLogger().catastrophe(); }
				void log(Log log) noexcept { getLogger().log(std::move(log)); }
				void logError(BunString message)
				{
					log({
						.severity = LogSeverity::Error,
						.message = std::move(message),
						.location = m_currentLocation,
						.token = m_currentToken,
						});
				}
				void logException(BunString messageRoot) noexcept
				{
					try
					{
						logError(messageRoot + " failed: " + getExceptionMessage());
					}
					catch (...)
					{
						getLogger().catastrophe();
					}
				}

				void startToken()
				{
					m_currentLocation.length = 0;
					m_currentToken = {
						.location = m_currentLocation,
						.content = {},
						.type = {},
					};
				}

				void submitToken(TokenType type)
				{
					m_currentToken.type = type;
					try
					{
						getWriter().submit(m_currentToken);
					}
					catch (...)
					{
						logException("submitToken");
					}
					startToken();
				}



				void startNewlineTerminatedSection()
				{
					m_passedNewline = false;
				}

				bool passedNewline() const
				{
					return m_passedNewline;
				}

				void setPassedNewline()
				{
					m_passedNewline = true;
				}



				bool eof() const { return m_eof; }
				BunChar peek() const
				{
					assert(!eof());
					if (eof())
					{
						throw LexerException("unexpected end of file");
					}
					return m_peek;
				}

				void nextEof()
				{
					m_eof = getReader().eof();
					m_peek = m_eof ? '\0' : getReader().peek();
				}

				void discard()
				{
					BunChar const c = peek();
					getReader().next();
					nextEof();

					++m_currentLocation.position;
					if (IsCharacter::Newline(c))
					{
						m_currentLocation.column = 0;
						++m_currentLocation.line;
						setPassedNewline();
					}
					else
					{
						++m_currentLocation.column;
					}
				}

				void eat()
				{
					m_currentToken.content += peek();
					++m_currentLocation.length;
					discard();
				}

				void discardWhitespaceExcludingNewline()
				{
					while (!eof() && IsCharacter::WhitespaceExcludingNewline(peek()))
					{
						discard();
					}
				}

				void discardNewline()
				{
					while (IsCharacter::Newline(peek()))
					{
						discard();
					}
				}

				void discardWhitespaceIncludingNewlines()
				{
					while (!eof() && IsCharacter::WhitespaceIncludingNewline(peek()))
					{
						discard();
					}
				}

				void discardSingleLineComment()
				{
					while (!eof() && !IsCharacter::Newline(peek()))
					{
						discard();
					}
					if (!eof())
					{
						assert(IsCharacter::Newline(peek()));
						discardWhitespaceIncludingNewlines();
					}
				}

				void discardMultiLineComment()
				{
					assert(IsCharacter::SpecifyMultiLineComment(peek()));
					discard();

					// discard until end of multi line comment block
					while (true)
					{
						while (!IsCharacter::SpecifyMultiLineComment(peek())
							&& !IsCharacter::BranchCommentOrDivide(peek()))
						{
							discard();
						}

						if (IsCharacter::SpecifyMultiLineComment(peek()))
						{
							// could be the end of the block
							discard();
							if (IsCharacter::BranchCommentOrDivide(peek()))
							{
								// it is the end of the block
								discard();
								break;
							}
						}
						else
						{
							assert(IsCharacter::BranchCommentOrDivide(peek()));
							// could be a nested multi line comment
							eat();
							if (IsCharacter::SpecifyMultiLineComment(peek()))
							{
								// it is a nested multi line comment
								discardMultiLineComment();
							}
						}
						// continue discarding for this multi line comment
					}
				}

				void branchCommentOrDivide()
				{
					assert(IsCharacter::BranchCommentOrDivide(peek()));
					startToken();
					eat();
					if (IsCharacter::SpecifySingleLineComment(peek()))
					{
						discardSingleLineComment();
					}
					else if (IsCharacter::SpecifyMultiLineComment(peek()))
					{
						discardMultiLineComment();
					}
					else
					{
						submitToken(TokenType::Operator);
						discardWhitespaceIncludingNewlines();
					}
				}

				void eatRestOfDecimal()
				{
					while (!eof() && IsCharacter::Decimal(peek()))
					{
						eat();
					}
				}

				void branchNumber()
				{
					assert(IsCharacter::Zero(peek()));
					startToken();
					eat();
					if (eof())
					{
						submitToken(TokenType::DecimalIntegerLiteral);
					}
					else
					{
						if (IsCharacter::SpecifyHex(peek()))
						{
							eat();
							if (eof() || !IsCharacter::Hex(peek()))
							{
								logError("Expected hexadecimal integer literal after `0x`");
							}
							else
							{
								startToken();
								while (!eof() && IsCharacter::Hex(peek()))
								{
									eat();
								}
								submitToken(TokenType::HexadecimalIntegerLiteral);
							}
						}
						else
						{
							eatRestOfDecimal();
							submitToken(TokenType::DecimalIntegerLiteral);
						}
					}
				}

				TokenType determineKeywordOrIdentifier() const
				{
					static auto const keywords = std::set<BunString>
					{
						"main_program_entry_point",
						"print",
					};
					if (keywords.find(m_currentToken.content) != keywords.end())
					{
						return TokenType::Keyword;
					}
					else
					{
						return TokenType::Identifier;
					}
				}

				void identifier()
				{
					assert(IsCharacter::startIdentifier(peek()));
					startToken();
					eat();
					if (eof())
					{
						submitToken(determineKeywordOrIdentifier());
					}
					else
					{
						while (!eof() && IsCharacter::identifierTail(peek()))
						{
							eat();
						}
						submitToken(determineKeywordOrIdentifier());
					}
					discardWhitespaceIncludingNewlines();
				}

				void stringLiteral()
				{
					assert(IsCharacter::stringLiteral(peek()));
					eat();
					startToken();
					while (!IsCharacter::stringLiteral(peek()))
					{
						eat();
					}
					submitToken(TokenType::StringLiteral);
					eat();
					discardWhitespaceIncludingNewlines();
				}

				void branchRoot()
				{
					assert(!eof());
					if (IsCharacter::Zero(peek()))
					{
						branchNumber();
						discardWhitespaceIncludingNewlines();
					}
					else if (IsCharacter::Decimal(peek()))
					{
						startToken();
						eatRestOfDecimal();
						submitToken(TokenType::DecimalIntegerLiteral);
						discardWhitespaceIncludingNewlines();
					}
					else if (IsCharacter::BranchCommentOrDivide(peek()))
					{
						branchCommentOrDivide();
					}
					else if (IsCharacter::startIdentifier(peek()))
					{
						identifier();
					}
					else if (IsCharacter::AnyOf(peek(), "{}[]():"))
					{
						eat();
						submitToken(TokenType::Punctuation);
						discardWhitespaceIncludingNewlines();
					}
					else if (peek() == ';')
					{
						eat();
						submitToken(TokenType::StatementTerminator);
					}
					else if (IsCharacter::stringLiteral(peek()))
					{
						stringLiteral();
					}
					else
					{
						startToken();
						while (!eof() && !IsCharacter::WhitespaceIncludingNewline(peek()))
						{
							eat();
						}
						logError("unrecognized token");
						submitToken(TokenType::Unknown);
					}
					discardWhitespaceIncludingNewlines();
				}

			public:
				void run()
				{
					try
					{
						discardWhitespaceIncludingNewlines();
						while (!eof())
						{
							try
							{
								branchRoot();
							}
							catch (...)
							{
								logException("branchRoot");
							}
						}
					}
					catch (...)
					{
						logException("run");
					}
				}
			};
		}

		void run(Args args)
		{
			try
			{
				LexerImpl(args).run();
			}
			catch (...)
			{
				ILogger& logger = args.logger.get();
				try
				{
					logger.log({
						.severity = LogSeverity::Error,
						.message = logger.getExceptionMessage(),
						.location = {},
						.token = {},
						});
				}
				catch (...)
				{
					logger.catastrophe();
				}
			}
		}
	}
}
