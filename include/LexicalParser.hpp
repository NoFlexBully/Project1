#ifndef LEXICAL_PARSER_HPP
#define LEXICAL_PARSER_HPP

#include <string>
#include <string_view>
#include <algorithm>
#include <vector>
#include <cctype>

enum class OperatorIntent {
    DISSIPATE,  // Grounding, relaxation, releasing tension
    ADVANCE,    // Flowing forward along existing paths
    REFLECT,    // Taking inventory, orienting oneself
    COMMIT,     // Locking in, accepting consequences
    SEVER,      // Burning bridges, closing options
    INHIBIT,    // Holding back, resisting the urge to act
    FORGE,      // Willpower breakthrough: creating a path out of nowhere
    UNKNOWN
};

struct ParsedCommand {
    OperatorIntent intent{OperatorIntent::UNKNOWN};
    double intensity{1.0};
    std::string raw_input;
};

class LexicalParser {
public:
    [[nodiscard]] static ParsedCommand Parse(std::string_view line) {
        std::string cleaned = Normalize(line);
        ParsedCommand cmd;
        cmd.raw_input = line;

        if (cleaned.find("really") != std::string::npos || 
            cleaned.find("deeply") != std::string::npos ||
            cleaned.find("hard") != std::string::npos ||
            cleaned.find("now") != std::string::npos ||
            cleaned.find("immediately") != std::string::npos ||
            cleaned.find("whatever it takes") != std::string::npos) {
            cmd.intensity = 1.5;
        } else if (cleaned.find("slowly") != std::string::npos ||
                   cleaned.find("gently") != std::string::npos ||
                   cleaned.find("a bit") != std::string::npos ||
                   cleaned.find("quietly") != std::string::npos) {
            cmd.intensity = 0.5;
        }

        static const std::vector<std::pair<std::vector<std::string_view>, OperatorIntent>> rules = {
            {{"calm down", "breathe", "rest", "relax", "cool down", "settle", "chill", "ease", "let go", "take a breath"}, OperatorIntent::DISSIPATE},
            {{"move on", "step forward", "continue", "next", "proceed", "go", "advance", "keep going", "step", "take path", "walk"}, OperatorIntent::ADVANCE},
            {{"stuck", "where am i", "inspect", "options", "look around", "look", "reflect", "status", "help", "what can i do", "observe"}, OperatorIntent::REFLECT},
            {{"im ready", "i am ready", "commit", "lock it in", "choose", "decide", "ratchet", "resolve", "done", "accept this"}, OperatorIntent::COMMIT},
            {{"burn", "sever", "cut", "close this door", "never back", "destroy path", "shut it"}, OperatorIntent::SEVER},
            {{"hold back", "suppress", "wait", "freeze", "stop", "resist", "do not do it", "dont do it"}, OperatorIntent::INHIBIT},
            {{"find a way", "break out", "make a path", "push through", "forge", "create a way", "escape", "force it"}, OperatorIntent::FORGE}
        };

        for (const auto& [phrases, mapped_intent] : rules) {
            for (const auto& phrase : phrases) {
                if (cleaned.find(phrase) != std::string::npos) {
                    cmd.intent = mapped_intent;
                    return cmd;
                }
            }
        }

        return cmd;
    }

private:
    [[nodiscard]] static std::string Normalize(std::string_view str) {
        std::string result;
        result.reserve(str.size());
        for (char ch : str) {
            if (std::isalnum(static_cast<unsigned char>(ch)) || std::isspace(static_cast<unsigned char>(ch))) {
                result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
            }
        }
        return result;
    }
};

#endif // LEXICAL_PARSER_HPP
