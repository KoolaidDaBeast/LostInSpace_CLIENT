#ifndef LOST_IN_SPACE_ENTITY
#define LOST_IN_SPACE_ENTITY

#include "splashkit.h"
#include "player.h"
#include <vector>

using namespace std;

#define ENTITY_SPEED 0.75

#define ENTITY_LASER_SPEED 10

#define ENTITY_ATTACK_COOLDOWN 2.5

#define ENTITY_PURSUE_RANGE 500

/**
 * The entity data keeps track of all of the information related to the entity.
 * 
 * @field   entity_sprite   The player's sprite - used to track position and movement
 * @field   hp              The current hp for the player
 * @field   kind            Current kind of player ship
 * @field   name            Current ship's displayname
 * @field   id              Current ship id for the server
 * @field   hidden          Current ship visibility
 * @field   inventory       Current players inventory
 */
struct entity_data
{
    sprite      entity_sprite;
    int         hp;
    ship_kind   kind;
    string name;
    string id;
    string target;
    double attackTime;
    point_2d to;
};

entity_data new_entity(double x, double y);

void draw_entity(entity_data &entity_to_draw);

void attackPlayers(vector<player_data> &players, vector<entity_data> &entites, vector<item> &items);

#endif 