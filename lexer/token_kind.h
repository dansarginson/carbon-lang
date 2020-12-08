// Part of the Carbon Language project, under the Apache License v2.0 with LLVM
// Exceptions. See /LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef LEXER_TOKEN_KIND_H_
#define LEXER_TOKEN_KIND_H_

#include <stdint.h>

#include <iterator>

#include "llvm/ADT/StringRef.h"

namespace Carbon {

class TokenKind {
  enum class KindEnum : int8_t {
#define CARBON_TOKEN(TokenName) TokenName,
#include "lexer/token_registry.def"
  };

 public:
#define CARBON_TOKEN(TokenName) \
  static constexpr auto TokenName()->TokenKind { return KindEnum::TokenName; }
#include "lexer/token_registry.def"

  // The default constructor is deleted as objects of this type should always be
  // constructed using the above factory functions for each unique kind.
  TokenKind() = delete;

  auto operator==(const TokenKind& rhs) const -> bool {
    return kind_value == rhs.kind_value;
  }
  auto operator!=(const TokenKind& rhs) const -> bool {
    return kind_value != rhs.kind_value;
  }

  // Get a friendly name for the token for logging or debugging.
  auto Name() const -> llvm::StringRef;

  // Test whether this kind of token is a simple symbol sequence (punctuation,
  // not letters) that appears directly in the source text and can be
  // unambiguously lexed with `starts_with` logic. While these may appear
  // inside of other tokens, outside of the contents of other tokens they
  // don't require any specific characters before or after to distinguish them
  // in the source. Returns false otherwise.
  auto IsSymbol() const -> bool;

  // Test whether this kind of token is a grouping symbol (part of an opening
  // and closing pair that must always be matched in the token stream).
  auto IsGroupingSymbol() const -> bool;

  // Test whether this kind of token is an opening symbol for a group.
  auto IsOpeningSymbol() const -> bool;

  // Returns the associated closing symbol for an opening symbol.
  //
  // The token kind must be an opening symbol.
  auto GetClosingSymbol() const -> TokenKind;

  // Test whether this kind of token is an closing symbol for a group.
  auto IsClosingSymbol() const -> bool;

  // Returns the associated opening symbol for a closing symbol.
  //
  // The token kind must be an closing symbol.
  auto GetOpeningSymbol() const -> TokenKind;

  // Test whether this kind of token is a keyword.
  auto IsKeyword() const -> bool;

  // If this token kind has a fixed spelling when in source code, returns it.
  // Otherwise returns an empty string.
  auto GetFixedSpelling() const -> llvm::StringRef;

  // Enable implicit conversion to an int, including in a `constexpr` context,
  // to enable usage in `switch` and `case`.
  constexpr operator int() const { return static_cast<int>(kind_value); }

 private:
  constexpr TokenKind(KindEnum kind_value) : kind_value(kind_value) {}

  KindEnum kind_value;
};

}  // namespace Carbon

#endif  // LEXER_TOKEN_KIND_H_