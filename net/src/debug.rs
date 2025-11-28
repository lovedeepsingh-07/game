use colored::Colorize;

pub fn debug(input: &str){
    println!("{} {}", "[NET_DEBUG]".bright_white(), input);
}
pub fn info(input: &str){
    println!("{} {}", "[NET_INFO]".bright_cyan(), input);
}
pub fn warn(input: &str){
    println!("{} {}", "[NET_WARN]".bright_yellow(), input);
}
pub fn error(input: &str){
    println!("{} {}", "[NET_ERROR]".bright_red(), input);
}
