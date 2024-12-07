import argparse
from builder import Builder
from parser import Parser
import os

def main():
    parser = argparse.ArgumentParser(description='Process a file.')
    
    parser.add_argument('input_file', 
                       help='Path to the input file to process')
    parser.add_argument('output_file', 
                       help='Path to the output file')
    
    args = parser.parse_args()
    
    if not os.path.isfile(args.input_file):
        print(f"Error: The file {args.input_file} does not exist.")
        exit(1)
    
    tokenslist = Parser.tokenize(args.input_file)
    
    with open(args.output_file, 'w') as outfile:
        content = Builder.get_file_content(tokenslist)
        outfile.write(content)
            
    print(f"File {args.input_file} converted to {args.output_file}")

	

if __name__ == '__main__':
    main()