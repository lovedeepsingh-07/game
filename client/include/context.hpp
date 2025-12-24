#pragma once

#include "elements.hpp"
#include "memory_arena.hpp"
#include "theme_engine.hpp"
#include <functional>
#include <memory>

class Context {
  public:
    MemoryArena frame_arena{ MemoryArena(10240) };
    MemoryArena message_arena{ MemoryArena(20480) };
    ThemeEngine theme_e;

    Context() = default;
    ~Context() = default;

    // disable copying and moving
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;

    void setup() {
        this->theme_e.setup();
    }
};

class Document {
  private:
    std::unordered_map<std::string, std::function<void(Document& doc, Context& ctx)>> m_pages;
    std::string m_curr_page_id;
    std::unordered_map<std::string, std::unique_ptr<elements::Element>> m_element_map;

  public:
    Document() = default;
    ~Document() = default;

    // disable copying and moving
    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;
    Document(Document&&) = delete;
    Document& operator=(Document&&) = delete;

    void add_page(const std::string& page_id, const std::function<void(Document& doc, Context& ctx)>& page_func);
    void set_curr_page(const std::string& page_id);
    const std::string& get_curr_page();
    void render_curr_page(Document& doc, Context& ctx);

    template <typename Element_Type>
    Element_Type* get_element(const std::string& element_id) {
        auto iter = m_element_map.find(element_id);
        if (iter == m_element_map.end()) {
            return nullptr;
        }
        return dynamic_cast<Element_Type*>(iter->second.get());
    }
    void add_element(const std::string& element_id, std::unique_ptr<elements::Element> element);
};
