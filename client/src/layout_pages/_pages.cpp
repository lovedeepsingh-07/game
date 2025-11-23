#include "layout.hpp"

void layout::pages::setup(Document& doc) {
    doc.add_page("debug", layout::pages::debug);
    doc.add_page("login", layout::pages::login);

    doc.set_curr_page("login");
}
