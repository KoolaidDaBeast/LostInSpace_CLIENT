#include "splashkit.h"
#include "player.h"
#include "server_manager.h"
#include "lost_in_space.h"
#include "ai_entity.h"
#include <chrono>
#include <cmath>

using namespace std::chrono;

void printConsole(string text)
{
    write_line("Game Server >> " + text);
}

double getMilliTime()
{
    return std::ceil(high_resolution_clock::now().time_since_epoch().count() / 1000000);
}

bool connectToServer(string ip, unsigned int port)
{
    open_connection("server_connection", ip, port, TCP);

    if (is_connection_open("server_connection"))
    {
        printConsole("Succesfully connected to server!");

        return true;
    }

    printConsole("Failed to connect to server!");

    return false;
}

void checkConnection()
{
    if (!is_connection_open("server_connection"))
    {
        reconnect("server_connection");
    }
}

void sendPlayerData(player_data &data, message_type type, ship_kind kind)
{
    json jsonData = create_json();

    json_set_string(jsonData, "display_name", data.name);
    json_set_string(jsonData, "id", data.id);
    json_set_number(jsonData, "msg_type", type);
    json_set_number(jsonData, "hp", data.hp);

    if (type == UPDATE_PLAYER)
    {
        json_set_number(jsonData, "dx", sprite_dx(data.player_sprite));
        json_set_number(jsonData, "x", sprite_position(data.player_sprite).x);
        json_set_number(jsonData, "y", sprite_position(data.player_sprite).y);
        json_set_number(jsonData, "dy", sprite_dy(data.player_sprite));
        json_set_number(jsonData, "kind", static_cast<int>(kind));
        json_set_bool(jsonData, "hidden", data.hidden);
    }

    if (is_connection_open("server_connection"))
    {
        send_message_to(json_to_string(jsonData), "server_connection");
        return;
    }

    printConsole("Failed to connect to server!");
}

void sendEntityData(entity_data &data, message_type type, ship_kind kind)
{
    json jsonData = create_json();

    json_set_string(jsonData, "display_name", data.name);
    json_set_string(jsonData, "id", data.id);
    json_set_number(jsonData, "msg_type", type);
    json_set_number(jsonData, "hp", data.hp);

    if (type == UPDATE_ENTITY)
    {
        json_set_number(jsonData, "dx", sprite_dx(data.entity_sprite));
        json_set_number(jsonData, "x", sprite_position(data.entity_sprite).x);
        json_set_number(jsonData, "y", sprite_position(data.entity_sprite).y);
        json_set_number(jsonData, "dy", sprite_dy(data.entity_sprite));
        json_set_number(jsonData, "kind", static_cast<int>(kind));
    }

    if (is_connection_open("server_connection"))
    {
        send_message_to(json_to_string(jsonData), "server_connection");
        return;
    }

    printConsole("Failed to connect to server!");
}

void sendPlayerItemData(player_data &data, message_type type, item &itm)
{
    json jsonData = create_json();

    json_set_string(jsonData, "display_name", data.name);
    json_set_string(jsonData, "id", data.id);
    json_set_number(jsonData, "msg_type", type);

    if (type == SPAWN_ITEM)
    {
        json_set_string(jsonData, "item_id", itm.id);
        json_set_number(jsonData, "item_type", itm.type);
        json_set_number(jsonData, "x", sprite_position(itm.item).x);
        json_set_number(jsonData, "y", sprite_position(itm.item).y);
        json_set_number(jsonData, "rotation", sprite_rotation(itm.item));
        json_set_number(jsonData, "speed", itm.speed);
        json_set_number(jsonData, "damage", itm.damage);
    }

    if (is_connection_open("server_connection"))
    {
        send_message_to(json_to_string(jsonData), "server_connection");
        return;
    }

    printConsole("Failed to connect to server!");
}

void sendEntityItemData(entity_data &data, message_type type, item &itm)
{
    json jsonData = create_json();

    json_set_string(jsonData, "display_name", data.name);
    json_set_string(jsonData, "id", data.id);
    json_set_number(jsonData, "msg_type", type);

    if (type == SPAWN_ITEM)
    {
        json_set_string(jsonData, "item_id", itm.id);
        json_set_number(jsonData, "item_type", itm.type);
        json_set_number(jsonData, "x", sprite_position(itm.item).x);
        json_set_number(jsonData, "y", sprite_position(itm.item).y);
        json_set_number(jsonData, "rotation", sprite_rotation(itm.item));
        json_set_number(jsonData, "speed", itm.speed);
        json_set_number(jsonData, "damage", itm.damage);
    }

    if (is_connection_open("server_connection"))
    {
        send_message_to(json_to_string(jsonData), "server_connection");
        return;
    }

    printConsole("Failed to connect to server!");
}

bool playerExist(string id, vector<player_data> &players)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (to_lowercase(players[i].id) == to_lowercase(id))
        {
            return true;
        }
    }

    return false;
}

bool entityExist(string id, vector<entity_data> &entities)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (to_lowercase(entities[i].id) == to_lowercase(id))
        {
            return true;
        }
    }

    return false;
}

player_data getPlayerData(string id, vector<player_data> &players)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (to_lowercase(players[i].id) == to_lowercase(id))
        {
            return players[i];
        }
    }
}

entity_data getEntityData(string id, vector<entity_data> &entities)
{
    for (int i = 0; i < entities.size(); i++)
    {
        if (to_lowercase(entities[i].id) == to_lowercase(id))
        {
            return entities[i];
        }
    }
}

void updatePlayerData(player_data &player, vector<player_data> &players)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (to_lowercase(players[i].id) == to_lowercase(player.id))
        {
            players[i] = player;
            break;
        }
    }
}

void updateEntityData(entity_data &entity, vector<entity_data> &entites)
{
    for (int i = 0; i < entites.size(); i++)
    {
        if (to_lowercase(entites[i].id) == to_lowercase(entity.id))
        {
            entites[i] = entity;
            break;
        }
    }
}

void removePlayerData(string id, vector<player_data> &players)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (to_lowercase(players[i].id) == to_lowercase(id))
        {
            int last_index = players.size() - 1;
            players[i] = players[last_index];
            players.pop_back();

            break;
        }
    }
}

void removeEntityData(string id, vector<entity_data> &entites)
{
    for (int i = 0; i < entites.size(); i++)
    {
        if (to_lowercase(entites[i].id) == to_lowercase(id))
        {
            int last_index = entites.size() - 1;
            entites[i] = entites[last_index];
            entites.pop_back();

            break;
        }
    }
}

void readServerResponse(vector<player_data> &players, vector<entity_data> &entites, vector<item> &items)
{
    //Check for incoming messages
    check_network_activity();

    if (!has_messages())
    {
        return;
    }

    //Receive message data
    json jsonData = create_json(message_data(read_message("server_connection")));

    string name = json_read_string(jsonData, "display_name");
    string id = json_read_string(jsonData, "id");
    message_type type = static_cast<message_type>(json_read_number_as_int(jsonData, "msg_type"));

    if (type == UPDATE_PLAYER)
    {
        float dx = json_read_number(jsonData, "dx");
        float dy = json_read_number(jsonData, "dy");
        float x = json_read_number(jsonData, "x");
        float y = json_read_number(jsonData, "y");
        int hp = json_read_number_as_int(jsonData, "hp");
        bool hidden = json_read_bool(jsonData, "hidden");
        ship_kind kind = static_cast<ship_kind>(json_read_number_as_int(jsonData, "kind"));

        player_data player;

        if (!playerExist(id, players))
        {
            player = new_player(x, y);

            sprite_set_dx(player.player_sprite, dx);

            player.name = name;

            player.id = id;

            players.push_back(player);

            printConsole("Player '" + name + "' has joined the game!");
        }
        else
        {
            player = getPlayerData(id, players);
        }

        player.hp = hp;

        player.hidden = hidden;

        //Ignore message data if is for the current player
        if (to_lowercase(players[0].id) == to_lowercase(id))
            return;

        player_switch_to_ship(player, kind);

        point_2d atPoint;
        atPoint.x = x;
        atPoint.y = y;

        player.toPoint = atPoint;

        sprite_set_dx(player.player_sprite, dx);
        sprite_set_dy(player.player_sprite, dy);

        if (!sprite_at(player.player_sprite, atPoint))
        {
            move_sprite_to(player.player_sprite, player.toPoint.x, player.toPoint.y);
        }

        updatePlayerData(player, players);
    }
    else if (type == PLAYER_DEATH)
    {
        //Ignore message data if is for the current player
        if (to_lowercase(players[0].id) == to_lowercase(id))
            return;

        printConsole("Player '" + name + "' has died!");
        removePlayerData(id, players);
    }
    else if (type == LEAVE_GAME)
    {
        //Ignore message data if is for the current player
        if (to_lowercase(players[0].id) == to_lowercase(id))
            return;

        printConsole("Player '" + name + "' has left the game!");
        removePlayerData(id, players);
    }
    else if (type == SPAWN_ITEM)
    {
        //Ignore message data if is for the current player
        if (to_lowercase(players[0].id) == to_lowercase(id))
            return;

        string item_id = json_read_string(jsonData, "item_id");
        item_type type = static_cast<item_type>(json_read_number_as_int(jsonData, "item_type"));
        float x = json_read_number(jsonData, "x");
        float y = json_read_number(jsonData, "y");
        float direction = json_read_number(jsonData, "rotation");
        double speed = json_read_number_as_double(jsonData, "speed");
        double damage = json_read_number_as_double(jsonData, "damage");

        point_2d point;

        point.x = x;
        point.y = y;

        item itm = createItem(item_id, point, direction, type, speed);

        itm.damage = damage;

        itm.shooter = id;

        items.push_back(itm);
    }

    else if (type == UPDATE_ENTITY)
    {
        float dx = json_read_number(jsonData, "dx");
        float dy = json_read_number(jsonData, "dy");
        float x = json_read_number(jsonData, "x");
        float y = json_read_number(jsonData, "y");
        int hp = json_read_number_as_int(jsonData, "hp");
        ship_kind kind = static_cast<ship_kind>(json_read_number_as_int(jsonData, "kind"));

        entity_data entity;

        if (!entityExist(id, entites))
        {
            entity = new_entity(x, y);

            sprite_set_dx(entity.entity_sprite, dx);

            entity.name = name;

            entity.id = id;

            entites.push_back(entity);

            printConsole("Entity '" + name + "' has been spawned.");
        }
        else
        {
            entity = getEntityData(id, entites);
        }

        entity.hp = hp;

        point_2d atPoint;
        atPoint.x = x;
        atPoint.y = y;

        if (!sprite_at(entity.entity_sprite, atPoint))
        {
            move_sprite_to(entity.entity_sprite, atPoint.x, atPoint.y);
        }

        sprite_set_dx(entity.entity_sprite, dx);
        sprite_set_dy(entity.entity_sprite, dy);

        updateEntityData(entity, entites);
    }
    else if (type == ENTITY_DEATH)
    {
        printConsole("Entity '" + name + "' has died!");
        removeEntityData(id, entites);
    }
}