#ifndef LOST_IN_SPACE_GAME
#define LOST_IN_SPACE_GAME

#include "splashkit.h"
#include "player.h"
#include "ai_entity.h"

using namespace std;

#define GAME_BOUNDARY 5000

/**
 * Returns the bitmap for a selected item
 * 
 * @param type        The item type
 * 
 * @returns           The item bitmap
 */
bitmap item_bitmap(item_type type);

/**
 * Updates all the players
 * 
 * @param player    The current player
 * @param players   The players that are connected to the server
 * @param items     The items currently in the server
 */
void updateGame(player_data &player, vector<player_data> &players, vector<entity_data> &entites, vector<item> &items);

/**
 * Draws all the players
 * 
 * @param player    The current player
 * @param players   The players that are connected to the server
 * @param items     The items currently in the server
 */
void drawGame(player_data &player, vector<player_data> &players, vector<entity_data> &entites, vector<item> &items);

/**
 * Connects the client to the speficed server
 * 
 */
void setupGame();

/**
 * Creates a item at the specifed location
 * 
 * @param from        The location to spawn the item in
 * @param rotation    The direction the item should face
 * @param type        The item you want to spawn
 * @param speed       The item velocity
 * 
 * @returns           The item data about the sprite
 */
item createItem(const point_2d from, const float rotation, const item_type type, const double speed);

/**
 * Creates a item at the specifed location
 * 
 * @param id          The id the item should use - For setting global items for all players.
 * @param from        The location to spawn the item in
 * @param rotation    The direction the item should face
 * @param type        The item you want to spawn
 * @param speed       The item velocity
 * 
 * @returns           The item data about the sprite
 */
item createItem(const string id, const point_2d from, const float rotation, const item_type type, const double speed);

/**
 * Checks the collisions with other sprites
 * 
 * @param players   The list of online players
 * @param items     The items currently in the server
 */
void checkCollisions(player_data &currentPlayer, vector<player_data> &players, vector<item> &items);

void checkEntityCollisions(vector<entity_data> &entites, vector<item> &items);

#endif 