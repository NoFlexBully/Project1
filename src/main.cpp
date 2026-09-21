#include <iostream>
#include <iomanip>
#include <string>
#include "Types.hpp"
#include "Rational.hpp"
#include "GraphManifold.hpp"
#include "Rotator.hpp"
#include "Resonator.hpp"
#include "Sieve.hpp"
#include "Ratchet.hpp"
#include "Decompressor.hpp"
#include "LexicalParser.hpp"

int main() {
    std::cout << "================================================================================\n";
    std::cout << "COGNITIVE STATE ENGINE // MOBILE MANIFOLD ACTIVE\n";
    std::cout << "================================================================================\n\n";

    GraphManifold manifold;
    manifold.AddNode(0, "Stillness / Open Observation", Rational(1, 1));
    manifold.AddNode(1, "Deliberation / The Crucible", Rational(3, 2));
    manifold.AddNode(2, "Committed Action / The Crossing", Rational(1, 2));

    manifold.Connect(0, 1, Rational(1, 2));
    manifold.Connect(1, 2, Rational(3, 4));
    manifold.Connect(2, 0, Rational(1, 1));

    uint32_t current_locus = 0;
    TriadicRotator rotator;
    Triad current_state{.T = 1.0, .S = 0.5, .E = 0.4};
    Triad previous_state = current_state;
    Lexicon lexicon = BuildBaseLexicon();

    std::string line;
    while (true) {
        const auto* node = manifold.GetNode(current_locus);
        std::cout << "\n--------------------------------------------------------------------------------\n";
        std::cout << "SPACE: " << (node ? node->label : "Unknown") 
                  << (manifold.IsCommitted(current_locus) ? " [COMMITTED]" : "")
                  << "  |  T: " << std::fixed << std::setprecision(2) << current_state.T 
                  << "  S: " << current_state.S 
                  << "  E: " << current_state.E << "\n\n";

        GrammarCoordinate current_coord{
            .register_label = "Ambient",
            .tension_factor = current_state.E,
            .active_slot = 0,
            .locked = manifold.IsCommitted(current_locus)
        };
        std::cout << MorphologicalDecompressor::Synthesize(current_coord, current_state) << "\n\n";

        NarrativeContext ctx{
            .coord = current_coord,
            .state = current_state,
            .previous_state = previous_state,
            .global_time = current_state.T,
            .ratchet_engaged = manifold.IsCommitted(current_locus),
            .edge_severed = false,
            .last_intent_phrase = line
        };

        std::cout << "What do you say or do? ('exit' to stop)\n>> ";
        if (!std::getline(std::cin, line) || line == "exit") {
            break;
        }

        Intent intent = MorphologicalDecompressor::InferIntent(line);
        if (intent == Intent::SEEK_MEANING || intent == Intent::SEEK_EXPLANATION) {
            std::string voice = MorphologicalDecompressor::Synthesize(ctx, lexicon, line);
            std::cout << "\n\"" << voice << "\"\n";
            continue;
        }

        ParsedCommand cmd = LexicalParser::Parse(line);

        switch (cmd.intent) {
            case OperatorIntent::DISSIPATE: {
                double kappa = DissipationSieve::DISSIPATION_RATE * cmd.intensity;
                previous_state = current_state;
                current_state.E = std::max(0.0, current_state.E - (kappa * std::tanh(current_state.E + 0.1)));
                current_state.T += 0.05;
                std::cout << "\nYou exhale slowly. The inner pressure recedes.\n";
                break;
            }

            case OperatorIntent::ADVANCE: {
                auto edges = manifold.GetEdges(current_locus);
                if (!edges || edges->empty()) {
                    std::cout << "\nYou push forward, but the threshold is severed. You are trapped in this state.\n";
                    break;
                }

                auto min_edge = std::min_element(edges->begin(), edges->end(),
                    [](const GraphEdge& a, const GraphEdge& b) {
                        return a.admittance_cost < b.admittance_cost;
                    });

                Rational cost = min_edge->admittance_cost;
                uint32_t target = min_edge->target_node_id;

                previous_state = current_state;
                current_state.T += 0.20 * cmd.intensity;
                current_state.S = static_cast<double>(target) * 0.5;
                current_state.E += cost.ToDouble() * cmd.intensity;

                Triad rotated = rotator.Rotate(current_state);
                Triad sieved = DissipationSieve::Filter(rotated, previous_state);
                auto res = PhaseResonator::Evaluate(sieved);
                auto commit = EpistemicRatchet::Evaluate(sieved, res.locked);

                if (target == 2 && (commit.committed || manifold.IsCommitted(2))) {
                    manifold.MarkCommitted(2);
                    if (manifold.SeverEdge(2, 0)) {
                        std::cout << "\n[!] The bridge behind you collapses into the void. There is no return.\n";
                    }
                }

                current_locus = target;
                current_state = sieved;
                std::cout << "\nYou step across the threshold into the next space.\n";
                break;
            }

            case OperatorIntent::REFLECT: {
                std::cout << "\nYou pause and take measure of the surrounding walls...\n";
                auto edges = manifold.GetEdges(current_locus);
                if (!edges || edges->empty()) {
                    std::cout << "  No passages remain open. The locus is terminal.\n";
                } else {
                    for (const auto& e : *edges) {
                        const auto* target_node = manifold.GetNode(e.target_node_id);
                        std::cout << "  -> A passage opens toward: \"" 
                                  << (target_node ? target_node->label : "Unknown")
                                  << "\" (Cost: " << e.admittance_cost.ToString() << ")\n";
                    }
                }
                break;
            }

            case OperatorIntent::COMMIT: {
                auto res = PhaseResonator::Evaluate(current_state);
                auto commit = EpistemicRatchet::Evaluate(current_state, res.locked);
                if (current_locus == 2 || commit.committed) {
                    manifold.MarkCommitted(current_locus);
                    manifold.SeverEdge(2, 0);
                    std::cout << "\nYou lock the latch. The topology hardens permanently.\n";
                } else {
                    std::cout << "\nThe ground does not yield. Your internal resonance is out of phase.\n";
                }
                break;
            }

            case OperatorIntent::SEVER: {
                auto edges = manifold.GetEdges(current_locus);
                if (edges && !edges->empty()) {
                    uint32_t target_to_cut = edges->back().target_node_id;
                    manifold.SeverEdge(current_locus, target_to_cut);
                    std::cout << "\nYou shatter the passage forward. That path is gone forever.\n";
                    current_state.E += 0.50;
                } else {
                    std::cout << "\nThere are no outbound paths left to sever.\n";
                }
                break;
            }

            case OperatorIntent::INHIBIT: {
                std::cout << "\nYou choke back the impulse. Kinetic drive turns inward as raw heat.\n";
                current_state.E += 0.25 * cmd.intensity;
                current_state.T += 0.05;
                break;
            }

            case OperatorIntent::FORGE: {
                std::cout << "\nWith desperate resolve, you carve a new threshold through the stone...\n";
                uint32_t target = (current_locus == 1) ? 2 : 0;
                manifold.ForgeEdge(current_locus, target, Rational(5, 4));
                current_state.E += 1.0 * cmd.intensity;
                current_state.T += 0.30;
                std::cout << "A ragged passage opens before you at severe physical cost.\n";
                break;
            }

            case OperatorIntent::UNKNOWN:
            default:
                std::cout << "\nYour words echo off the stones, ungrounded.\n";
                break;
        }
    }

    std::cout << "\nThe engine falls silent.\n";
    return 0;
}
