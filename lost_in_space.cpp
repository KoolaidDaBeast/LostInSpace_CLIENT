#include "splashkit.h"
#include "player.h"
#include "terminal_user_input.h"
#include "server_manager.h"
#include "lost_in_space.h"
#include "ai_entity.h"
#include <thread>

bitmap item_bitmap(item_type type)
{
    switch (type)
    {
    case LASER_BEAM:
        return bitmap_named("laser_green");
    case ENEMY_LASER_BEAM:
        return bitmap_named("laser_red");
    }
}

void setupGame()
{
    try
    {
        // string ip = read_string("What is the ip?");
        // int port = read_integer_range("What is the port?", 0, 50000);

        //Keep asking for ip address and port until valid connection is established
        bool svrConn = connectToServer("localhost", 25565);

        while (!svrConn)
        {
            string ip = read_string("What is the ip?");
            int port = read_integer_range("What is the port?", 0, 50000);
            svrConn = connectToServer(ip, port);
        }
    }
    catch (const std::exception &e)
    {
        write_line("Game Server >> Failed to connect to the server!");
    }
}

void updateGame(player_data &player, vector<player_data> &players, vector<entity_data> &entites, vector<item> &items)
{
    //Perform movement and update the camera
    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].hidden)
        {
            continue;
        }

        update_sprite(players[i].player_sprite);
    }

    for (int i = 0; i < items.size(); i++)
    {
        update_sprite(items[i].item);
    }

    for (int i = 0; i < entites.size(); i++)
    {
        update_sprite(entites[i].entity_sprite);
    }

    update_player(player);
}

void draw_cursor()
{
    float x = mouse_x();
    float y = mouse_y();

    draw_bitmap("mouse", x - 0.5 * bitmap_width("mouse"), y - 0.5 * bitmap_width("mouse"), option_to_screen());
}

void drawGame(player_data &player, vector<player_data> &players, vector<entity_data> &entites, vector<item> &items)
{
    //Draw game
    clear_screen(COLOR_BLACK);

    for (int i = 0; i < items.size(); i++)
    {
        if (point_point_distance(sprite_position(player.player_sprite), sprite_position(items[i].item)) > 400) continue;

        draw_sprite(items[i].item);
    }

    for (int i = 0; i < players.size(); i++)
    {
        if (players[i].hidden)
        {
            continue;
        }

        if (point_point_distance(sprite_position(player.player_sprite), sprite_position(players[i].player_sprite)) > 400) continue;

        draw_player(players[i]);
    }

    for (int i = 0; i < entites.size(); i++)
    {
        if (point_point_distance(sprite_position(player.player_sprite), sprite_position(entites[i].entity_sprite)) > 400) continue;

        draw_entity(entites[i]);
    }

    draw_cursor();

    draw_HUD(player);

    fill_rectangle(COLOR_WHITE, 400, 400, 10, 10);
}

item createItem(const point_2d from, const float rotation, const item_type type, const double speed)
{
    item itm;

    itm.type = type;

    bitmap img = item_bitmap(type);

    itm.id = "ITEM#" + to_string(getMilliTime());

    itm.item = create_sprite("ITEM#" + to_string(getMilliTime()), img);

    itm.start_x = from.x;
    itm.start_y = from.y;

    itm.speed = speed;

    itm.damage = 0;

    itm.shooter = "SERVER";

    sprite_set_x(itm.item, from.x);

    sprite_set_y(itm.item, from.y - (sprite_width(itm.item) / 2));

    sprite_set_rotation(itm.item, rotation);

    sprite_set_dx(itm.item, speed);

    return itm;
}

item createItem(const string id, const point_2d from, const float rotation, const item_type type, const double speed)
{
    item itm;

    itm.type = type;

    bitmap img = item_bitmap(type);

    itm.id = id;

    itm.item = create_sprite(id, img);

    itm.start_x = from.x;
    itm.start_y = from.y;

    itm.speed = speed;

    itm.damage = 0;

    itm.shooter = "SERVER";

    sprite_set_x(itm.item, from.x);

    sprite_set_y(itm.item, from.y - (sprite_width(itm.item) / 2));

    sprite_set_rotation(itm.item, rotation);

    sprite_set_dx(itm.item, speed);

    return itm;
}

item getItem(sprite spr, vector<item> &items)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].id == sprite_name(spr))
        {
            return items[i];
        }
    }
}

void deleteItem(vector<item> &items, const int index)
{
    if (index >= 0 && index < items.size())
    {
        free_sprite(items[index].item);
        items[index] = items[items.size() - 1];
        items.pop_back();
    }
}

void purgeItems(vector<item> &items)
{
    for (int i = 0; i < items.size(); i++)
    {
        item itm = items[i];

        point_2d from;
        from.x = itm.start_x;
        from.y = itm.start_y;

        point_2d to = center_point(itm.item);

        if (point_point_distance(from, to) > GAME_BOUNDARY)
        {
            deleteItem(items, i);
        }

        if (to.x > GAME_BOUNDARY || to.y > GAME_BOUNDARY)
        {
            deleteItem(items, i);
        }
    }
}

void checkCollisions(player_data &currentPlayer, vector<player_data> &players, vector<item> &items)
{
    purgeItems(items);

    for (int z = 0; z < players.size(); z++)
    {
        player_data player = players[z];

        if (player.hidden)
            continue;

        for (int i = 0; i < items.size(); i++)
        {
            if (!sprite_collision(player.player_sprite, items[i].item))
                continue;

            item itm = items[i];

            //Continue if the item collided with belongs to the player
            if (to_lowercase(itm.shooter) == to_lowercase(player.id))
                continue;

            //If the current client has been hit
            if (currentPlayer.id == player.id)
            {
                //Check that they are alive
                if ((currentPlayer.hp - itm.damage) <= 0)
                {
                    currentPlayer.hp = 0;

                    currentPlayer.hidden = true;

                    sendPlayerData(currentPlayer, PLAYER_DEATH, currentPlayer.kind);

                    deleteItem(items, i);

                    players[0] = currentPlayer;
                    break;
                }

                //Lower health and send data to server
                currentPlayer.hp = currentPlayer.hp - itm.damage;

                deleteItem(items, i);

                sendPlayerData(currentPlayer, UPDATE_PLAYER, currentPlayer.kind);

                players[0] = currentPlayer;
                break;
            }

            deleteItem(items, i);
        }
    }
}

void checkEntityCollisions(vector<entity_data> &entites, vector<item> &items)
{
    purgeItems(items);

    for (int z = 0; z < entites.size(); z++)
    {
        entity_data entity = entites[z];

        for (int i = 0; i < items.size(); i++)
        {
            if (!sprite_collision(entity.entity_sprite, items[i].item))
                continue;

            item itm = items[i];

            //Continue if the item collided with belongs to the player
            if (to_lowercase(itm.shooter) == to_lowercase(entity.id) || itm.type == ENEMY_LASER_BEAM)
                continue;

            //Check that they are alive
            if ((entity.hp - itm.damage) <= 0)
            {
                entity.hp = 0;

                sendEntityData(entity, ENTITY_DEATH, entity.kind);

                deleteItem(items, i);

                removeEntityData(entity.id, entites);
                break;
            }

            //Lower health and send data to server
            entity.hp = entity.hp - itm.damage;

            deleteItem(items, i);

            sendEntityData(entity, UPDATE_ENTITY, entity.kind);
        }
    }
}