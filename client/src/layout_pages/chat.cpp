#include "layout.hpp"
#include <common/error.hpp>
#include <common/theme_engine.hpp>
#include <generated/packet_.h>
#include <net.h>
#include <raylib.h>
#include <rust/cxx.h>

std::vector<std::vector<uint8_t>> packets;

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
    try {
        net::client::update((uint64_t)GetFrameTime() * 1000);
    } catch (rust::Error e) {
        common::error(fmt::format(
            "failed to update the client, {}", error::Error::from_rust(e).to_string()
        ));
        std::exit(1);
    }
    // poll packets
    try {
        for (auto curr_packet : net::client::poll_packets()) {
            packets.emplace_back(
                std::vector(curr_packet.data.begin(), curr_packet.data.end())
            );
        };
    } catch (rust::Error e) {
        common::error(
            fmt::format("failed to poll packets, {}", error::Error::from_rust(e).to_string())
        );
        std::exit(1);
    }

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
            for (auto curr_packet : packets) {
                const auto* packet = packet::GetPacket(curr_packet.data());
                auto packet_type = packet->data_type();
                switch (packet_type) {
                    case (packet::Packet_Data_Message): {
                        const auto* message = packet->data_as_Message();
                        std::string message_username =
                            std::string(message->username()->c_str());
                        std::string message_body = std::string(message->body()->c_str());

                        try {
                            auto client_username = net::client::get_username();
                            bool outgoing_message = message_username
                                == std::string(client_username.begin(),
                                               client_username.end());

                            layout::components::chat_bubble(doc, ctx, message_username, message_body, outgoing_message);
                        } catch (rust::Error e) {
                            common::error(fmt::format(
                                "failed to get the client username, {}",
                                error::Error::from_rust(e).to_string()
                            ));
                            std::exit(1);
                        }
                        break;
                    }
                    case (packet::Packet_Data_JoinEvent): {
                        const auto* join_event = packet->data_as_JoinEvent();
                        std::string username =
                            std::string(join_event->username()->c_str());

                        try {
                            auto client_username = net::client::get_username();
                            if (username
                                != std::string(
                                    client_username.begin(), client_username.end()
                                )) {
                                layout::components::chat_event(doc, ctx, fmt::format("{} joined the server", username));
                            }
                        } catch (rust::Error e) {
                            common::error(fmt::format(
                                "failed to get the client username, {}",
                                error::Error::from_rust(e).to_string()
                            ));
                            std::exit(1);
                        }
                        break;
                    }
                    case (packet::Packet_Data_LeaveEvent): {
                        const auto* leave_event = packet->data_as_LeaveEvent();
                        std::string username =
                            std::string(leave_event->username()->c_str());

                        try {
                            auto client_username = net::client::get_username();
                            if (username
                                != std::string(
                                    client_username.begin(), client_username.end()
                                )) {
                                layout::components::chat_event(doc, ctx, fmt::format("{} left the server", username));
                            }
                        } catch (rust::Error e) {
                            common::error(fmt::format(
                                "failed to get the client username, {}",
                                error::Error::from_rust(e).to_string()
                            ));
                            std::exit(1);
                        }
                        break;
                    }
                    default: {
                    }
                }
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
                        try {
                            net::client::send_message(message_input);
                            message_element->value = "";
                        } catch (rust::Error e) {
                            common::error(fmt::format(
                                "failed to send message, {}",
                                error::Error::from_rust(e).to_string()
                            ));
                            std::exit(1);
                        }
                    }
                }
            };
        }
    }

    // sync to server
    try {
        net::client::send_packets();
    } catch (rust::Error e) {
        common::error(fmt::format(
            "failed to send packets to the server, {}",
            error::Error::from_rust(e).to_string()
        ));
        std::exit(1);
    }
}
