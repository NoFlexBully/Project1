#ifndef DECOMPRESSOR_HPP
#define DECOMPRESSOR_HPP

#include "Types.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cctype>

struct NarrativeContext {
    GrammarCoordinate coord;
    Triad state;
    Triad previous_state;
    double global_time{0.0};
    bool ratchet_engaged{false};
    bool edge_severed{false};
    std::string last_intent_phrase;
};

enum class Intent {
    SEEK_MEANING,
    SEEK_EXPLANATION,
    SEEK_ACTION,
    SEEK_RELEASE,
    SEEK_STILLNESS,
    SEEK_COMPANIONSHIP
};

struct SemanticAtom {
    std::string name;
    std::string high_strain_variant;
    double valence{0.0};
    double intensity{0.5};
    double temporal_bias{0.0};
    double spatial_bias{0.0};
    double tension_bias{0.0};
};

struct Lexicon {
    std::vector<SemanticAtom> atoms;

    void Add(std::string name, 
             std::string high_strain_variant, 
             double valence, 
             double intensity, 
             double t_bias, 
             double s_bias, 
             double e_bias) 
    {
        atoms.push_back(SemanticAtom{
            std::move(name),
            std::move(high_strain_variant),
            valence,
            intensity,
            t_bias,
            s_bias,
            e_bias
        });
    }
};

inline Lexicon BuildBaseLexicon() {
    Lexicon lex;
    lex.Add("language",  "a blunt plea",    +0.3, 0.7, +0.1, +0.2, +0.1);
    lex.Add("sight",     "a burning glare", +0.4, 0.6, +0.2, +0.3, +0.4);
    lex.Add("soul",      "marrow",          +0.2, 0.5, +0.3, +0.1, +0.2);
    lex.Add("mouth",     "a clenched jaw",  +0.1, 0.5, +0.1, +0.0, +0.5);
    lex.Add("eyes",      "veiled pupils",   +0.3, 0.6, +0.2, +0.2, +0.3);
    lex.Add("stillness", "petrification",   +0.5, 0.3, -0.2, +0.4, -0.4);
    lex.Add("stride",    "recoil",          +0.2, 0.8, +0.4, +0.2, +0.3);
    return lex;
}

struct NarrativeTemplate {
    std::string id;
    std::string pattern;
};

struct SemanticBlend {
    std::vector<const SemanticAtom*> active_atoms;
    double overall_valence{0.0};
    double overall_intensity{0.0};
};

class MorphologicalDecompressor {
public:
    [[nodiscard]] static std::string Synthesize(const GrammarCoordinate& coord, const Triad& state) {
        if (coord.locked) {
            return "The anchor is struck: what was once malleable is cast in iron.\n"
                   "The boundary behind you has collapsed into sheer stone.";
        }
        if (coord.tension_factor > 2.0) {
            if (state.S < 0.2) {
                return "You are back in the open, but your thoughts remain in a vice. The quiet around you feels oppressive,\n"
                       "amplifying the unresolved pressure trapped beneath your ribs.";
            }
            return "The air is heavy, almost solid with unresolved pressure. Your chest feels tight,\n"
                   "thoughts racing in a closed loop. Every option feels like chewing glass.";
        } else if (coord.tension_factor > 1.0) {
            return "A sharp, persistent friction hums beneath the surface. You can feel the weight\n"
                   "of choices left hanging, demanding either action or release.";
        } else if (coord.tension_factor > 0.4) {
            return "There is a gentle current moving you along. Focus is steady, neither strained nor entirely at ease.";
        }
        return "A quiet stillness settles in. The horizon opens wide, uncluttered and weightless.";
    }

    [[nodiscard]] static Intent InferIntent(std::string_view input) {
        std::string lower = ToLower(input);
        if (lower.find("what is language") != std::string::npos || lower.find("meaning") != std::string::npos)
            return Intent::SEEK_MEANING;
        if (lower.find("what is it") != std::string::npos || lower.find("why") != std::string::npos)
            return Intent::SEEK_EXPLANATION;
        if (lower.find("move on") != std::string::npos || lower.find("ready") != std::string::npos || lower.find("proceed") != std::string::npos)
            return Intent::SEEK_ACTION;
        if (lower.find("tense") != std::string::npos || lower.find("breathe") != std::string::npos || lower.find("relax") != std::string::npos)
            return Intent::SEEK_RELEASE;
        if (lower.find("rest") != std::string::npos || lower.find("still") != std::string::npos)
            return Intent::SEEK_STILLNESS;

        return Intent::SEEK_COMPANIONSHIP;
    }

    [[nodiscard]] static SemanticBlend ComputeBlend(const NarrativeContext& ctx, const Lexicon& lex, Intent intent) {
        SemanticBlend blend;

        for (const auto& atom : lex.atoms) {
            double score = 0.0;
            score += atom.temporal_bias * ctx.state.T;
            score += atom.spatial_bias  * ctx.state.S;
            score += atom.tension_bias  * ctx.state.E;

            if (intent == Intent::SEEK_MEANING) {
                if (atom.name == "language") score += 0.5;
                if (atom.name == "soul")     score += 0.3;
                if (atom.name == "sight")    score += 0.3;
            }

            if (score > 0.35) {
                blend.active_atoms.push_back(&atom);
                blend.overall_valence   += atom.valence * score;
                blend.overall_intensity += atom.intensity * score;
            }
        }
        return blend;
    }

    [[nodiscard]] static NarrativeTemplate SelectTemplate(const SemanticBlend& blend, Intent intent, const NarrativeContext& ctx) {
        if (ctx.coord.locked) {
            return {"ratchet_lock", "The anchor is struck: what was once malleable is cast in iron."};
        }

        bool hasLanguage = false;
        for (const auto* atom : blend.active_atoms) {
            if (atom->name == "language") {
                hasLanguage = true;
                break;
            }
        }

        if (intent == Intent::SEEK_MEANING && hasLanguage) {
            return {
                "language_dual_metaphor",
                "{subject} is the {perceptual} of the {inner}, and a {expressive} for our {shared}."
            };
        }

        if (ctx.state.E > 1.8) {
            return {
                "high_strain_ambient",
                "Pressure locks the threshold. Every breath carries the taste of cold stone."
            };
        }

        return {
            "ambient_grounding",
            "The space holds steady. The current does not pull until you lean into it."
        };
    }

    [[nodiscard]] static std::string Realize(const NarrativeTemplate& tmpl, const SemanticBlend& blend, const NarrativeContext& ctx) {
        bool high_strain = (ctx.state.E > 1.5);

        auto pick = [&](const std::string& target_name) -> std::string {
            for (const auto* atom : blend.active_atoms) {
                if (atom->name == target_name) {
                    return high_strain ? atom->high_strain_variant : atom->name;
                }
            }
            return target_name;
        };

        std::string out = tmpl.pattern;
        ReplaceAll(out, "{subject}",    pick("language"));
        ReplaceAll(out, "{perceptual}", pick("sight"));
        ReplaceAll(out, "{inner}",      pick("soul"));
        ReplaceAll(out, "{expressive}", pick("mouth"));
        ReplaceAll(out, "{shared}",     pick("eyes"));

        return out;
    }

    [[nodiscard]] static std::string Synthesize(const NarrativeContext& ctx, const Lexicon& lex, std::string_view input) {
        Intent intent = InferIntent(input);
        SemanticBlend blend = ComputeBlend(ctx, lex, intent);
        NarrativeTemplate tmpl = SelectTemplate(blend, intent, ctx);
        return Realize(tmpl, blend, ctx);
    }

private:
    static void ReplaceAll(std::string& str, std::string_view from, std::string_view to) {
        if (from.empty()) return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    [[nodiscard]] static std::string ToLower(std::string_view input) {
        std::string result;
        result.reserve(input.size());
        for (char ch : input) {
            result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
        }
        return result;
    }
};

#endif // DECOMPRESSOR_HPP
