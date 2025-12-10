#include "layout.hpp"
#include <common/theme_engine.hpp>
#include <generated/packet_.h>
#include <net.h>
#include <raylib.h>

struct Message {
    std::string username;
    std::string body;
};

std::vector<Message> messages;

// std::vector<Message> messages = {
//     { "axewbotx", "hey everyone, testing the chat app" },
//     { "nova", "looks clean so far" },
//     { "kite", "yo what's up" },
//     { "rhea", "just joined, hi all!" },
//     { "orbit", "same here, checking message flow" },
//     { "blip", "can you all see this?" },
//     { "axewbotx", "yep, coming through perfectly" },
//     { "nova", "nice, message stacking looks good too" },
//     { "rhea", "sending a longer message to test wrapping. lorem ipsum dolor
//     sit amet, consectetur adipiscing elit." }, { "kite", "lol rhea going full
//     lorem ipsum" }, { "blip", "emoji support seems fine" }, { "hazel", "hey
//     I'm late but here" }, { "hazel", "dropping another long-ish message to
//     see how bubbles scale with more text. seems okay?" }, { "axewbotx", "yeah
//     looks solid" }, { "nova", "anyone try rapid sending?" }, { "nova", "like
//     this" }, { "nova", "and this" }, { "nova", "and this one lol" }, {
//     "rhea", "haha" }, { "orbit", "group chat feels alive now" }, { "blip",
//     "we should test scrolling soon" }, { "hazel", "agree!" }, { "axewbotx",
//     "I'll add more features tomorrow" }
// };

void layout::pages::chat(Document& doc, Context& ctx) {
    // update client
    // NOTE: GetFrameTime() returns seconds like "0.016" so we multiply by 1000
    // to convert into miliseconds
    net::client::update((uint64_t)GetFrameTime() * 1000);
    // poll messages
    for (auto curr_message : net::client::poll_messages()) {
        auto msg = Message{};
        const auto* packet = packet::GetPacket(curr_message.data.data());
        auto packet_type = packet->data_type();
        switch (packet_type) {
            case (packet::Packet_Data_Message): {
                const auto* data = packet->data_as_Message();
                msg.username = std::string(data->username()->c_str());
                msg.body = std::string(data->body()->c_str());
                break;
            }
            default: {
                msg.username = "invalid_packet_data_type";
                msg.body = "invalid_packet_data_type";
            }
        }
        messages.emplace_back(msg);
    };

    CLAY(Clay_ElementDeclaration{
        .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW() },
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_TOP },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
        // ------ message list ------
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(160, 1000), .height = CLAY_SIZING_GROW() },
                        .padding = { .bottom = 24 },
                        .childGap = 16,
                        .layoutDirection = CLAY_TOP_TO_BOTTOM },
            .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() } }) {
            for (auto curr_message : messages) {
                auto client_username = net::client::get_username();
                bool outgoing_message = curr_message.username
                    == std::string(client_username.begin(), client_username.end());
                layout::components::chat_bubble(
                    doc, ctx, curr_message.username, curr_message.body, outgoing_message
                );
            }
        }
        CLAY(Clay_ElementDeclaration{
            .layout = { .sizing = { .width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT() },
                        .padding = { .top = 20 },
                        .childGap = 12,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                        .layoutDirection = CLAY_LEFT_TO_RIGHT } }) {
            layout::components::message_input(doc, ctx, "message_input", "Enter Message");
            if (layout::components::send_button(doc, ctx, "send_button", "Send")) {
                auto* message_element =
                    doc.get_element<elements::Input>("message_input");
                if (message_element != nullptr) {
                    std::string message_input =
                        common::trim_whitespace(message_element->value);
                    if (message_input.size() != 0) {
                        net::client::send_message(message_input);
                        message_element->value = "";
                    }
                }
            };
        }
    }

    // sync to server
    net::client::send_packets();
}
