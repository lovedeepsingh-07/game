pub fn debug(input: &str) {
    let flag_str = "\x1b[90m[NET_DEBUG]\x1b[0m";
    println!("{} {}", flag_str, input);
}
pub fn info(input: &str) {
    let flag_str = "\x1b[94m[NET_INFO]\x1b[0m";
    println!("{} {}", flag_str, input);
}
pub fn warn(input: &str) {
    let flag_str = "\x1b[93m[NET_WARN]\x1b[0m";
    println!("{} {}", flag_str, input);
}
pub fn error(input: &str) {
    let flag_str = "\x1b[91m[NET_ERROR]\x1b[0m";
    println!("{} {}", flag_str, input);
}
