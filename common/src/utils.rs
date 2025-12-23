pub fn str_to_32_bytes(input: &str) -> [u8; 32] {
    *blake3::hash(input.as_bytes()).as_bytes()
}
