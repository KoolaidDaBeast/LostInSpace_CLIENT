#include "splashkit.h"
#include "player.h"
#include "ai_entity.h"
#include "terminal_user_input.h"
#include "server_manager.h"
#include "lost_in_space.h"
#include <thread>

void load_resources()
{
    load_resource_bundle("game_bundle", "lost_in_space.txt");

    load_font("game_font", "VCR_OSD_MONO_1.001.ttf");
}

int main()
{
    vector<player_data> players;
    vector<entity_data> entites;
    vector<item> items;

    setupGame();

    //Get Server Info
    string displayName = read_string("What should your display name be?");

    open_window("Lost In Space++", 650, 650);

    load_resources();

    player_data player = new_player();
    player.name = displayName;
    player.id = displayName + "." + to_string(getMilliTime());
    players.push_back(player);

    //Send player data to server
    sendPlayerData(player, UPDATE_PLAYER, player.kind);

    //Hide mouse
    hide_mouse();

    entites.push_back(new_entity(400, 400));

    do
    {
        process_events();

        if (!player.hidden)
        {
            handle_input(player, items);

            updateGame(player, players, entites, items);

            drawGame(player, players, entites, items); 

            //Read server data from other players
            readServerResponse(players, entites, items);

            attackPlayers(players, entites, items);

            checkCollisions(player, players, items);

            checkEntityCollisions(entites, items);
        }
        else
        {
            player_data specPlayer;

            for (int i = 0; i < players.size(); i++)
            {
                if (players[i].hidden) continue;
                
                specPlayer = players[i];
            }

            updateGame(specPlayer, players, entites, items);

            drawGame(specPlayer, players, entites, items);

            //Read server data from other players
            readServerResponse(players, entites, items);

            checkCollisions(specPlayer, players, items);
        }

        refresh_screen(60);

    } while (!quit_requested());

    sendPlayerData(player, LEAVE_GAME, player.kind);

    return 0;
}