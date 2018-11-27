#ifndef SERVER_MANAGER
#define SERVER_MANAGER

#include "splashkit.h"
#include "player.h"
#include "ai_entity.h"

using namespace std;

/**
 * The message type tells the clients what to do with the data recieved.
 * 
 * @field   UPDATE_PLAYER   Used to update position and movement
 */
enum message_type
{
    UPDATE_PLAYER,
    SPAWN_ITEM,
    PLAYER_DEATH,
    LEAVE_GAME,
    ENTITY_DEATH,
    UPDATE_ENTITY
};

/**
 * Get the systems time in milliseconds
 * 
 * @returns     The system time in milliseconds since epoch
 */
double getMilliTime();

/**
 * Connects to the server to play on 
 * 
 * @param ip    The ip address of the server
 * @param port  The port of the server
 * 
 * @returns     If the client has successfully connected to the server
 */
bool connectToServer(string ip, unsigned int port);

/**
 * Sends the player's struct data to the host server
 * 
 * @param data    The player's data
 * @param type    The message type
 */
void sendPlayerData(player_data &data, message_type type, ship_kind kind);

/**
 * Sends item data to the server
 * 
 * @param data    The player's data
 * @param type    The message type
 * @param item    The item to send
 */
void sendPlayerItemData(player_data &data, message_type type, item &itm);

/**
 * Sends item data to the server
 * 
 * @param data    The entity's data
 * @param type    The message type
 * @param item    The item to send
 */
void sendEntityItemData(entity_data &data, message_type type, item &itm);

/**
 * Removes a player from the online users inside the current client
 * 
 * @param id         The player to delete
 * @param players    The list of players currently connected to the server
 */
void removePlayerData(string id, vector<player_data> &players);

/**
 * Removes a entity from the alive entites inside the current client
 * 
 * @param id         The entity to delete
 * @param players    The list of entites currently alive
 */
void removeEntityData(string id, vector<entity_data> &entites);

/**
 * Reads the message sent from the server about the other players
 * 
 * @param players    The list of players currently connected to the server
 * @param items      The list of items inside the server
 */
void readServerResponse(vector<player_data> &players, vector<entity_data> &entites, vector<item> &items);

/**
 * Sends the entity's struct data to the host server
 * 
 * @param data    The entity's data
 * @param type    The message type
 */
void sendEntityData(entity_data &data, message_type type, ship_kind kind);

#endif

