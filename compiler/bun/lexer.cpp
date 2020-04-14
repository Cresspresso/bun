
#include <cstdio>
#include <string_view>
#include <memory>
#include <cassert>

#include "lexer.hpp"

namespace Bun
{
	namespace Strings
	{
		std::string CharFromU8(String const& input)
		{
			std::string output;
			for (Char const c : input)
			{
				output.push_back((char)c);
			}
			return output;
		}

		String U8FromChar(std::string const& input)
		{
			String output;
			for (char const c : input)
			{
				output.push_back((Char)c);
			}
			return output;
		}
	}

	namespace Lexing
	{
		String Lexer::VirtualGetExceptionMessage()
		{
			String message = u8"internal exception: ";

			try
			{
				throw;
			}
			catch (Exception const& e)
			{
				message += u8"Lexing exception: ";
				message += Strings::U8FromChar(e.what());
			}
			catch (std::exception const& e)
			{
				message += u8"unknown exception: ";
				message += Strings::U8FromChar(e.what());
			}
			catch (...)
			{
				message += u8"unknown exception of unknown type";
			}

			return message;
		}

		namespace
		{
			struct IsCharacterTools
			{
				static inline bool AnyOf(Char const c, std::u8string_view const s) { return s.npos != s.find(c); }
				static inline bool InClosedInterval(Char const c, Char const low, Char const hi) { return c >= low && c <= hi; }
			};

			struct IsCharacter : public IsCharacterTools
			{
				using Super = IsCharacterTools;
				using Super::InClosedInterval;
				using Super::AnyOf;

				static inline bool Lowercase(Char const c) { return InClosedInterval(c, u8'a', u8'z'); }
				static inline bool Uppercase(Char const c) { return InClosedInterval(c, u8'A', u8'Z'); }

				static inline bool Decimal(Char const c) { return InClosedInterval(c, u8'0', u8'9'); }
				static inline bool Zero(Char const c) { return u8'0' == c; }
				static inline bool HighHex(Char const c) { return InClosedInterval(c, u8'a', u8'f') || InClosedInterval(c, u8'A', u8'F'); }
				static inline bool Hex(Char const c) { return Decimal(c) || HighHex(c); }
				static inline bool SpecifyHex(Char const c) { return AnyOf(c, u8"xX"); }
				static inline bool NumberWhitespace(Char const c) { return u8'_' == c; }
				static inline bool NumberFractionSeparator(Char const c) { return u8'.' == c; }

				static inline bool WhitespaceExcludingNewline(Char const c) { return AnyOf(c, u8" \t\v"); }
				static inline bool Newline(Char const c) { return AnyOf(c, u8"\n\r;"); }
				static inline bool WhitespaceIncludingNewline(Char const c) { return WhitespaceExcludingNewline(c) || Newline(c); }

				static inline bool OpenBrace(Char const c) { return u8'{' == c; }
				static inline bool CloseBrace(Char const c) { return u8'}' == c; }

				static inline bool BranchCommentOrDivide(Char const c) { return u8'/' == c; }
				static inline bool SpecifyMultiLineComment(Char const c) { return u8'*' == c; }
				static inline bool SpecifySingleLineComment(Char const c) { return u8'/' == c; }
			};

			class LexerImpl
			{
			public:
				~LexerImpl() noexcept = default;
				LexerImpl(LexerImpl&&) noexcept = delete;
				LexerImpl& operator=(LexerImpl&&) noexcept = delete;
				LexerImpl(LexerImpl const&) = delete;
				LexerImpl& operator=(LexerImpl const&) = delete;
				LexerImpl(
					Lexer& functionality,
					Location initialLocation
					)
					: m_functionality(functionality)
					, m_currentLocation(initialLocation)
					, m_currentToken{
						.location = initialLocation
				}
				{
				}

			private:
				std::reference_wrapper<Lexer> m_functionality;
				Location m_currentLocation;
				Token m_currentToken;
				bool m_passedNewline = false;

			private:
				// METHODS

				Lexer& GetFunctionality() { return m_functionality.get(); }

				void Log(LogMessage log)
				{
					try
					{
						GetFunctionality().Log(std::move(log));
					}
					catch (...)
					{
						fprintf_s(stderr, "Lexer failed to log a message.\n");

						try
						{
							GetFunctionality().Log(LogMessage{
								{},
								{},
								Severity::Error,
								u8"catastrophy"
								});
						}
						catch (...)
						{
							// discard
						}
					}
				}

				void LogError(String message)
				{
					Log(LogMessage{
					m_currentToken,
					m_currentLocation,
					Severity::Error,
					std::move(message)
						});
				}

				String GetExceptionMessage()
				{
					return GetFunctionality().GetExceptionMessage();
				}



				void StartToken()
				{
					m_currentToken.location = m_currentLocation;
					m_currentToken.length = 0;
					m_currentToken.content.clear();
					m_currentToken.type = TokenType::Unknown;
				}

				void SubmitToken(TokenType type)
				{
					m_currentToken.type = type;
					try
					{
						GetFunctionality().Submit(m_currentToken);
					}
					catch (...)
					{
						LogError(u8"SubmitToken failed: " + GetExceptionMessage());
					}
					StartToken();
				}



				void StartNewlineTerminatedSection()
				{
					m_passedNewline = false;
				}

				bool PassedNewline() const
				{
					return m_passedNewline;
				}

				void SetPassedNewline()
				{
					m_passedNewline = true;
				}



				bool Eof()
				{
					return GetFunctionality().Eof();
				}

				Char Peek()
				{
					if (Eof())
					{
						throw LexerException("unexpected end of file");
					}
					return GetFunctionality().Peek();
				}

				void Discard()
				{
					Char const c = Peek();
					GetFunctionality().Next();

					++m_currentLocation.position;
					if (IsCharacter::Newline(c))
					{
						m_currentLocation.column = 0;
						++m_currentLocation.line;
						SetPassedNewline();
					}
					else
					{
						++m_currentLocation.column;
					}
				}

				void Eat()
				{
					m_currentToken.content += Peek();
					++m_currentToken.length;
					Discard();
				}

				void DiscardWhitespaceExcludingNewline()
				{
					while (!Eof() && IsCharacter::WhitespaceExcludingNewline(Peek()))
					{
						Discard();
					}
				}

				void DiscardNewline()
				{
					while (IsCharacter::Newline(Peek()))
					{
						Discard();
					}
				}

				void DiscardWhitespaceIncludingNewlines()
				{
					while (!Eof() && IsCharacter::WhitespaceIncludingNewline(Peek()))
					{
						Discard();
					}
				}

				void DiscardSingleLineComment()
				{
					while (!Eof() && !IsCharacter::Newline(Peek()))
					{
						Discard();
					}
					if (!Eof())
					{
						assert(IsCharacter::Newline(Peek()));
						DiscardWhitespaceIncludingNewlines();
					}
				}

				void DiscardMultiLineComment()
				{
					assert(IsCharacter::SpecifyMultiLineComment(Peek()));
					Discard();

					// discard until end of multi line comment block
					while (true)
					{
						while (!IsCharacter::SpecifyMultiLineComment(Peek())
							&& !IsCharacter::BranchCommentOrDivide(Peek()))
						{
							Discard();
						}

						if (IsCharacter::SpecifyMultiLineComment(Peek()))
						{
							// could be the end of the block
							Discard();
							if (IsCharacter::BranchCommentOrDivide(Peek()))
							{
								// it is the end of the block
								Discard();
								break;
							}
						}
						else
						{
							assert(IsCharacter::BranchCommentOrDivide(Peek()));
							// could be a nested multi line comment
							Eat();
							if (IsCharacter::SpecifyMultiLineComment(Peek()))
							{
								// it is a nested multi line comment
								DiscardMultiLineComment();
							}
						}
						// continue discarding for this multi line comment
					}
				}

				void BranchCommentOrDivide()
				{
					assert(IsCharacter::BranchCommentOrDivide(Peek()));
					StartToken();
					Eat();
					if (IsCharacter::SpecifySingleLineComment(Peek()))
					{
						DiscardSingleLineComment();
					}
					else if (IsCharacter::SpecifyMultiLineComment(Peek()))
					{
						DiscardMultiLineComment();
					}
					else
					{
						SubmitToken(TokenType::Operator);
					}
				}

				void EatRestOfDecimal()
				{
					while (!Eof() && IsCharacter::Decimal(Peek()))
					{
						Eat();
					}
				}

				void BranchNumber()
				{
					assert(IsCharacter::Zero(Peek()));
					StartToken();
					Eat();
					if (Eof())
					{
						SubmitToken(TokenType::DecimalIntegerLiteral);
					}
					else
					{
						if (IsCharacter::SpecifyHex(Peek()))
						{
							Eat();
							if (Eof() || !IsCharacter::Hex(Peek()))
							{
								LogError(u8"Expected hexadecimal integer literal after `0x`");
							}
							else
							{
								StartToken();
								while (!Eof() && IsCharacter::Hex(Peek()))
								{
									Eat();
								}
								SubmitToken(TokenType::HexadecimalIntegerLiteral);
							}
						}
						else
						{
							EatRestOfDecimal();
							SubmitToken(TokenType::DecimalIntegerLiteral);
						}
					}
				}

				void BranchRoot()
				{
					assert(!Eof());
					if (IsCharacter::Zero(Peek()))
					{
						BranchNumber();
						DiscardWhitespaceIncludingNewlines();
					}
					else if (IsCharacter::Decimal(Peek()))
					{
						StartToken();
						EatRestOfDecimal();
						SubmitToken(TokenType::DecimalIntegerLiteral);
						DiscardWhitespaceIncludingNewlines();
					}
					else if (IsCharacter::BranchCommentOrDivide(Peek()))
					{
						BranchCommentOrDivide();
					}
					else
					{
						StartToken();
						while (!Eof() && !IsCharacter::WhitespaceIncludingNewline(Peek()))
						{
							Eat();
						}
						LogError(u8"unrecognized token");
						SubmitToken(TokenType::Unknown);
					}
					DiscardWhitespaceIncludingNewlines();
				}

			public:
				void Lex()
				{
					try
					{
						DiscardWhitespaceIncludingNewlines();
						while (!Eof())
						{
							try
							{
								BranchRoot();
							}
							catch (...)
							{
								LogError(u8"BranchRoot failed: " + GetExceptionMessage());
							}
						}
					}
					catch (...)
					{
						LogError(u8"Lex failed: " + GetExceptionMessage());
					}
				}
			};
		}

		void Lexer::Lex(Location initialLocation)
		{
			try
			{
				LexerImpl(*this, std::move(initialLocation)).Lex();
			}
			catch (...)
			{
				try
				{
					Log(LogMessage{
						.token = {},
						.location = {},
						.severity = Severity::Error,
						.message = GetExceptionMessage()
						});
				}
				catch (...)
				{
					fprintf_s(stderr, "Lexer failed to log a message.\n");
				}
			}
		}
	}
}
