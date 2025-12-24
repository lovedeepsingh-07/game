#include "context.hpp"
#include "debug.hpp"

void Document::add_page(
    const std::string& page_id, const std::function<void(Document& doc, Context& ctx)>& page_func
) {
    if (this->m_pages.find(page_id) != this->m_pages.end()) {
        debug::warn("(Document::add_page) page with this ID already "
                    "exists, doing nothing");
        return;
    }

    this->m_pages.emplace(page_id, page_func);
}

void Document::set_curr_page(const std::string& page_id) {
    if (this->m_pages.find(page_id) == this->m_pages.end()) {
        debug::warn("(Document::set_curr_page) page with this ID does not "
                    "exist, doing nothing");
        return;
    }
    if (this->m_curr_page_id != page_id) {
        // NOTE: every time a page transition takes place, we clear the element map
        this->m_element_map.clear();
        this->m_curr_page_id = page_id;
    }
}

const std::string& Document::get_curr_page() {
    return this->m_curr_page_id;
}

void Document::render_curr_page(Document& doc, Context& ctx) {
    auto iter = this->m_pages.find(this->m_curr_page_id);
    if (iter == this->m_pages.end()) {
        debug::error("(Document::render_curr_page) page with this ID does "
                     "not exist, doing nothing");
        return;
    }
    iter->second(doc, ctx);
}

void Document::add_element(const std::string& element_id, std::unique_ptr<elements::Element> element) {
    if (m_element_map.find(element_id) != m_element_map.end()) {
        // NOTE: element with this ID already exists
        // but we don't have to warn about this because
        // this is expected behaviour
        return;
    }
    m_element_map.emplace(element_id, std::move(element));
}
