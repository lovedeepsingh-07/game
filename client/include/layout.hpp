#include <common/context.hpp>

namespace layout {
    namespace components {
        bool navbar_button(Document& doc, Context& ctx, const std::string& id, const std::string& button_text);
        void navbar(Document& doc, Context& ctx);
        void login_input(Document& doc, Context& ctx, const std::string& id, const std::string& placeholder);
        bool login_button(Document& doc, Context& ctx, const std::string& id, const std::string& button_text);
        void message_input(Document& doc, Context& ctx, const std::string& id, const std::string& placeholder);
        bool send_button(Document& doc, Context& ctx, const std::string& id, const std::string& button_text);
        void chat_bubble(
            Document& doc, Context& ctx, const std::string& username, const std::string& body, bool outgoing_message
        );
    }
    namespace pages {
        void debug(Document& doc, Context& ctx);
        void login(Document& doc, Context& ctx);
        void chat(Document& doc, Context& ctx);

        void setup(Document& doc);
    }
}
