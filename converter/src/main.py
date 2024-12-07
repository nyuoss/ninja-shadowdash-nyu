import argparse
from parser import Parser
from token import Token
import os

def main():
    parser = argparse.ArgumentParser(description='Process a file.')
    
    parser.add_argument('file_path', 
                       help='Path to the file to process')
    
    args = parser.parse_args()
    
    directory = os.path.dirname(args.file_path)
    base_name = os.path.basename(args.file_path)
    name_without_ext = os.path.splitext(base_name)[0]
    new_filename = f"{name_without_ext}.build.cc"
    new_file_path = os.path.join(directory, new_filename)
    
    ninjaparser = Parser(args.file_path)
    tokenslist = ninjaparser.gettokens()
    
    with open(new_file_path, 'w') as outfile:
        for token in tokenslist:
            outfile.write(f"{token.get_string()}\n")
            
    print(f"Successfully wrote tokens to {new_file_path}")

	

if __name__ == '__main__':
    main()