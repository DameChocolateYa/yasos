use std::process;
use std::env;
use std::fs::File;
use std::io::Read;

use lexer::Lexer;
use lexer::Token;

mod lexer;
mod parser;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Incorrect usage. Correct usage is...");
        println!("beepc <input.bp>");
        process::exit(1);
    }
    let filename: &String = &args[1];

    let mut file: File = match File::open(filename) {
        Ok(f) => f,
        Err(e) => {
            eprintln!("Error opening file: {}", e);
            return;
        }
    };

    let mut contents: String = String::new();
    if let Err(e) = file.read_to_string(&mut contents) {
        eprintln!("Error readinf file: {}", e);
        return;
    }

    let mut lexer: Lexer = Lexer::new(contents);

    let tokens: Vec<Token> = lexer.tokenize();
    for token in tokens {
        println!("{:?}", token);
    }
}