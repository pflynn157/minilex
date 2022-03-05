#!/usr/bin/python3
import sys
import os

# Command line arguments
base_path = "./base"
output_path = "./src"
input_file = ""

# Prints the help text
def help():
	print("minilex - The simple lexical generator")
	print("")
	print("Usage: minilex FILE [base path] [output path]")
	print("")
	print("Defaults: minilex FILE ./base ./src")
	print("")

# Check command line arguments
if len(sys.argv) == 1:
	print("Error: No input file!")
	help()
	exit(1)
	
input_file = sys.argv[1]

if len(sys.argv) >= 3:
	base_path = sys.argv[2]
if len(sys.argv) >= 4:
	output_path = sys.argv[3]

# Init the needed maps
keywords = dict()
symbols = dict()
comments = list()

##
## The symbol map has a structure like this:
## {';': [(';', 'SemiColon')], '=': [('=', 'Assign')], ':': [(':', 'Colon'), (':=', 'Assign2')]}
##
## This is so we can have multiple symbols that start with the same prefix
##

# A utility function to print whitespace
def printSpace(writer, length):
	for i in range(0, length):
		writer.write(" ")

# Parses the lines
def parseLine(line):
	lastAssign = 0
	index = 0
	for c in line:
		if c == '=':
			lastAssign = index
		index += 1
	
	value = line[0:lastAssign].strip()
	name = line[lastAssign+1:].strip()
	
	cfirst = value[0]
	clast = value[len(value)-1]
	
	if cfirst == '\"' and clast == '\"':
		value = value[1:-1]
		keywords[value] = name
	elif cfirst == '\'' and clast == '\'':
		value = value[1:-1]
		key = value[0]
		if key in symbols:
			symbols[key].append((value, name))
		else:
			symbols[key] = [(value, name)]
	elif value == "@comment_line":
		comments.append(name)

# Read the file line by line
with open(input_file, "r") as reader:
	for line in reader.readlines():
		line = line.strip()
		if len(line) == 0:
			continue
		parseLine(line)
		
##		
## Begin copying
##

# First, process the header
reader = open(base_path + "/lex.hpp", "r")
writer = open(output_path + "/lex.hpp", "w")

for line in reader.readlines():
	line2 = line.strip()
	if line2 == "//##TOKEN LIST":
		for name in keywords.values():
			writer.write("    " + name + ",\n")
		for name_list in symbols.values():
			for item in name_list:
				name = item[1]
				writer.write("    " + name + ",\n")
	else:
		writer.write(line)
	
reader.close()
writer.close()

# Second, process the debug file
reader = open(base_path + "/lex_debug.cpp", "r")
writer = open(output_path + "/lex_debug.cpp", "w")

for line in reader.readlines():
	line2 = line.strip()
	if line2 == "//##TOKEN PRINT":
		for value, name in keywords.items():
			writer.write("        ")
			writer.write("case " + name + ": std::cout << \"" + value + "\" << std::endl; break;\n")
		for name_list in symbols.values():
			for item in name_list:
				writer.write("        ")
				writer.write("case " + item[1] + ": std::cout << \"" + item[0] + "\" << std::endl; break;\n")
	else:
		writer.write(line)

reader.close()
writer.close()

# Finally, process the main lex file
reader = open(base_path + "/lex.cpp", "r")
writer = open(output_path + "/lex.cpp", "w")

keyword_found_first = False

for line in reader.readlines():
	line2 = line.strip()
	
	# Returns a token for a given keyword (checks the current buffer)
	if line2 == "//##TOKEN KEYWORD":
		for value, name in keywords.items():
			writer.write("    ")
			if keyword_found_first:
				writer.write("else ")
			writer.write("if (buffer == \"" + value + "\") return " + name + ";\n")
			keyword_found_first = True
			
	# Returns whether or not a character is a valid lexical symbol
	elif line2 == "//##CHECK SYMBOL":
		for value, name in symbols.items():
			writer.write("    ")
			writer.write("    ")
			writer.write("case \'" + value[0] + "\': return true;\n")
			
	# Checks for single-line comments
	elif line2 == "//##TOKEN COMMENT_LINE":
		found_multi = False
		for symbol in comments:
			if len(symbol) == 1:
				printSpace(writer, 8)
				writer.write("if (next == \'" + symbol + "\') {\n")
				printSpace(writer, 12)
				writer.write("while (next != '\\n' && !reader.eof()) {\n")
				printSpace(writer, 16)
				writer.write("next = reader.get();\n");
				printSpace(writer, 16)
				writer.write("rawBuffer += next;\n")
				printSpace(writer, 12)
				writer.write("}\n")
				printSpace(writer, 12)
				writer.write("continue;\n")
				printSpace(writer, 8)
				writer.write("}\n")
			else:
				if found_multi:
					printSpace(writer, 8)
					writer.write("__next = \"\";\n")
				else:
					printSpace(writer, 8)
					writer.write("std::string __next = \"\";\n")
				found_multi = True
				printSpace(writer, 8)
				writer.write("__next += next;\n")
				
				for i in range(1, len(symbol)):
					printSpace(writer, 8)
					writer.write("__next += reader.get();\n")
					
				# Insert an EOF guard
				printSpace(writer, 8)
				writer.write("if (reader.eof()) {\n")
				printSpace(writer, 12)
				writer.write("token.type = Eof;\n")
				printSpace(writer, 12)
				writer.write("break;\n")
				printSpace(writer, 8)
				writer.write("}\n")
				
				# Now, for the actual check, and if we have the comment, consume it
				printSpace(writer, 8)
				writer.write("if (__next == \"" + symbol + "\") {\n")
				printSpace(writer, 12)
				writer.write("while (next != '\\n' && !reader.eof()) {\n")
				printSpace(writer, 16)
				writer.write("next = reader.get();\n");
				printSpace(writer, 16)
				writer.write("rawBuffer += next;\n")
				printSpace(writer, 12)
				writer.write("}\n")
				printSpace(writer, 12)
				writer.write("continue;\n")
				printSpace(writer, 8)
				writer.write("}\n")
					
				# Unget the characters in case we don't have a comment
				for i in range(1, len(symbol)):
					printSpace(writer, 8)
					writer.write("reader.unget();\n")
				
	# Returns a token for a given symbol
	#
	# This is complicated because some symbols may be multiple characters long, so we
	# have to check all variants
	#
	elif line2 == "//##TOKEN SYMBOL":
		for value, name_list in symbols.items():
			printSpace(writer, 8)
			if len(name_list) == 1:
				writer.write("case \'" + value + "\': return " + name_list[0][1] + ";\n")
			else:
				writer.write("case \'" + value + "\': {\n")
				printSpace(writer, 12)
				writer.write("char c2 = reader.get();\n")
				
				found_first = False
				default_name = None
				
				for item in name_list:
					if len(item[0]) == 1:
						default_name = item[1]
						continue
					
					symbol = item[0]
					name = item[1]
					
					printSpace(writer, 12)
					if found_first:
						writer.write("} else ")
					found_first = True
					writer.write("if (c2 == \'" + symbol[1] + "\') {\n");
					printSpace(writer, 16);
					writer.write("return " + name + ";\n");
					
				# Final else statement
				printSpace(writer, 12)
				writer.write("} else {\n")
				printSpace(writer, 16)
				writer.write("reader.unget();\n")
				printSpace(writer, 16)
				writer.write("return " + default_name + ";\n")
				printSpace(writer, 12)
				writer.write("}\n")
				
				# Closing brace of if statement
				printSpace(writer, 8)
				writer.write("}\n")
	else:
		writer.write(line)

reader.close()
writer.close()

