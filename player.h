#ifndef LOST_IN_SPACE_PLAYER
#define LOST_IN_SPACE_PLAYER

#include "splashkit.h"
#include <vector>

using namespace std;

#define PLAYER_SPEED 3
#define PLAYER_ROTATE_SPEED 3
#define SCREEN_BORDER 100

#define LASER_BEAM_SPEED 9

#define PLAYER_ATTACK_COOLDOWN 0.25

/**
 * Different options for the kind of ship.
 * Adjusts the image used.
 */
enum ship_kind
{
    SHIP_BLUE,
    SHIP_GREEN,
    SHIP_RED
}; 

/**
 * Different options for the items
 * Adjusts the image used.
 */
enum item_type
{
    LASER_BEAM,
    ENEMY_LASER_BEAM
};

/**
 * The items keeps track about all information regarding that item
 * 
 * @field   id              The id - Used to get the sprite item data
 * @field   type            The item type
 * @field   item            The item's sprite - used to track position and movement
 * @field   speed           The intial velocity of the item
 * @field   start_x         The x coord of where the item was spawned in - Used to remove item when out of view
 * @field   start_y         The y coord of where the item was spawned in - Used to remove item when out of view
 * @field   shooter         The entity who spawned in the item
 */
struct item 
{
    string id;
    item_type type;
    sprite item;
    float speed;
    double start_x;
    double start_y;
    double damage;
    string shooter;
};

/**
 * The inventory_data keeps track of a players items
 * 
 * @field   items            The items inside the player's inventory
 */
struct inventory_data 
{
    vector<item> items;
};

/**
 * The player data keeps track of all of the information related to the player.
 * 
 * @field   player_sprite   The player's sprite - used to track position and movement
 * @field   hp              The current hp for the player
 * @field   kind            Current kind of player ship
 * @field   name            Current ship's displayname
 * @field   id              Current ship id for the server
 * @field   hidden          Current ship visibility
 * @field   inventory       Current players inventory
 */
struct player_data
{
    sprite      player_sprite;
    int         hp;
    ship_kind   kind;
    string name;
    string id;
    bool hidden;
    bool shield_active;
    float shield_rotation;
    inventory_data inventory;
    double attackTime;
    point_2d toPoint;
};


bitmap ship_bitmap(ship_kind kind);

/**
 * Creates a new player in the x and y coords
 * 
 * @returns     The new player data
 */
player_data new_player();

/**
 * Creates a new player in the centre of the screen with the default ship.
 * 
 * @param x   The x coordinate
 * @param y   The y coordinate
 * @returns     The new player data
 */
player_data new_player(double x, double y);

/**
 * Draws the player to the screen. 
 * 
 * @param player_to_draw    The player to draw to the screen
 */
void draw_player(player_data &player_to_draw);

/**
 * Actions a step update of the player - moving them and adjusting the camera.
 * 
 * @param player_to_update      The player being updated
 */
void update_player(player_data &player_to_update);

/**
 * Read user input and update the player based on this interaction.
 * 
 * @param player    The player to update
 * @param items     The list of items inside the server
 */
void handle_input(player_data &player, vector<item> &items);

/**
 * Switches the player data's ship ship_kind
 * 
 * @param player    The player to update
 * @param target    The ship_kind to change into
 */
void player_switch_to_ship(player_data &player, ship_kind target);

/**
 * Draws the HUD which displays the information about the player
 * 
 * @param player    The player to draw the hud for
 */
void draw_HUD(const player_data &player);

/**
 * Check if the sprite is within the game play area
 * 
 * @param player    The sprite to check
 * @returns         If the sprite is inside the boundary
 */
bool isInBoundary(sprite &spr);

#endif 