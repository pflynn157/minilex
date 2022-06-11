##
## This is an example of a user-generated configuration file for
## simple-lex under a different name
##

base_path = "./base"
output_path = "./src"

# Define the keywords
keywords = [
    ("func", "Func"), ("is", "Is"), ("end", "End"),
    ("var", "Var"), ("return", "Return"),
    ("i32", "I32"), ("i64", "I64")
]

# Define the symbols
symbols = [
    (";", "SemiColon"), ("=", "Assign"), (":", "Colon"), (":=", "Assign2")
]

# Define single-line comments
single_comments = [ "#", "//" ]

# Define multi-line comments
multi_comments = [ ("*/", "*/") ]
