##
## This is an example of a user-generated configuration file for
## simple-lex.
##

base_path = "./base"
output_path = "./src"

# Define the keywords
keywords = [
    ("func", "Func"), ("is", "Is"), ("end", "End"),
    ("var", "Var"), ("return", "Return")
]

# Define the symbols
symbols = [
    (";", "SemiColon"), ("=", "Assign"), (":", "Colon"), (":=", "Assign2"), ("!=", "NEQ")
]

# Define single-line comments
single_comments = [ "#", "//" ]

# Define multi-line comments
multi_comments = [ ("*/", "*/") ]
