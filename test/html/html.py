base_path = "./base"
output_path = "./src"

# Define the keywords
keywords = [
    ("html", "Html"), ("head", "Head"), ("body", "Body"), ("title", "Title"), ("div", "Div"),
    ("h1", "H1"), ("h2", "H2"), ("h3", "H3"), ("h4", "H4"), ("h5", "H5"), ("h6", "H6")
]

# Define the symbols
symbols = [
    ("<", "TagOpen"), ("</", "CloseTagOpen"), (">", "TagClose")
]

# Define single-line comments
single_comments = []

# Define multi-line comments
multi_comments = [ ("<!--", "-->") ]
