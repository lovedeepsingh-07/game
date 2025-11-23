#include <common/context.hpp>

namespace layout {
    namespace components {
        bool navbar_button(Context& ctx);
        void navbar(Context& ctx);
        void login_input(Document& doc, Context& ctx, const std::string& id, const std::string& placeholder);
        bool login_button(Document& doc, Context& ctx);
    }
    namespace pages {
        void debug(Document& doc, Context& ctx);
        void login(Document& doc, Context& ctx);

        void setup(Document& doc);
    }
}
