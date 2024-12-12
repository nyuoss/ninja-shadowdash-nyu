import argparse
import os
from typing import List
from .builder import Builder
from .parser import Parser
from .token import Token

def main() -> None:
    """
    Main function to convert a ninja build file to a ShadowDash manifest file.
    """
    parser = argparse.ArgumentParser(description='Converts ninja files to ShadowDash manifest files.')
    
    # Define input and output file arguments
    parser.add_argument('input_file', 
                       help='Path to the input file to process')
    parser.add_argument('output_file', 
                       help='Path to the output file')
    
    args = parser.parse_args()
    
    # Check if the input file exists
    if not os.path.isfile(args.input_file):
        print(f"Error: The file {args.input_file} does not exist.")
        exit(1)
    
    # Tokenize the input file
    tokenslist: List[Token] = Parser.tokenize(args.input_file)
    
    # Write the converted content to the output file
    with open(args.output_file, 'w') as outfile:
        content: str = Builder.get_file_content(tokenslist)
        outfile.write(content)
            
    print(f"File {args.input_file} converted to {args.output_file}")

if __name__ == '__main__':
    main()