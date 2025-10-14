#include "../include/Tournament.h"

namespace Tournament {

    void Tournament::matchMaking() {
        for (const auto& player1 : strategies) {
            for (const auto& player2 : strategies) {
                MatchResults match (player1, player2);
            }
        }
    }

}