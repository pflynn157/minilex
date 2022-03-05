## MiniLex

Minilex is a very simple lexical analyzer generator. Minilex itself is written in Python, but it generates a completely portable analyzer in C++.

Compared to other lexical analyzer generators, Minilex is not very advanced. However, that is partially by design. The goal of this is to have a simple generator with a lower learning curve than that of Flex or Lex, and unlike Flex, it is meant to be easier to integrate with build systems.

I created this because I was tired of manually copying and pasting my lexical analyzer for each new project :) The one I originally created was working rather well, but it was tedious to do it manually, so I thought, why not just create a Python script for this?

### Usage

Short answer: minilex <input> [output directory] [base code]

The input is the only required parameter; this parameter is defines the lexical structure of keywords, sybmols, and comments. The base code is where the provided lexical base is located. The output directory is wherever you want the code.

Note that you can generate a config file (see config.py) that contains the default output and base code paths.

### Creating a Lex file

Creating a lex file is very easy. Take this for example:

```
"func" = Func
"is" = Is
"end" = End
"var" = Var
"return" = Return

';' = SemiColon
'=' = Assign
':' = Colon
':=' = Assign2

@comment_line = #
@comment_line = //
@comment_block = /* */
```

While the ordering does not matter, the example is organized to define keywords, symbols, and comments. Keywords are defined with quotation marks. Symbols are defined with single quotes. However, please note that symbols can be composed of multiple characters. For both symbols and keywords, the left value is always the textual element, and the right value corresponds to values in the TokenType enumeration.

To create comments, use "@comment_line" to define single line comments, and "@comment_block" to define multi-line comments. For multi-line comments, a space separates the beginning and ending tags. Note that the beginning and ending tags can be different lengths. However, patterns cannot be reused.

### Literals

The parser by default will scan integer, hex, string, and character literals. Integers are defined as normal numbers, hex values are defined as 0x[0-9, A-F], string literals are defined as any character between two double quotation marks, and character literals are defined as a single character between two single quotation marks. Any text that is not a keyword or pre-defined literal is returned as an identifier.

NOTE: I may change this behavior later to provide some control over the literals.

### Generated Code

Three files are generated: "lex.cpp", "lex.hpp", and "lex_debug.cpp". The header is the best way to understand how to use the lexical analyzer. It contains the definitions of the Token class, the available token types, and the scanner class. The "lex.cpp" file implements these definitions, and the "lex_debug.cpp" class implements a debug function for visualizing the lexical analyzer.

You should be able to incorporate these three files directly into your project without any issues. You will need to add a dependency to your build system to run minilex, but other than that, no special care is needed.

