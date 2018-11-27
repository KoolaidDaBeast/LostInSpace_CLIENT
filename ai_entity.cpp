#include "splashkit.h"
#include "server_manager.h"
#include "lost_in_space.h"
#include "ai_entity.h"
#include "player.h"

entity_data new_entity(double x, double y)
{
    entity_data result;

    bitmap default_bitmap = ship_bitmap(SHIP_RED);

    result.entity_sprite = create_sprite(default_bitmap);

    result.kind = SHIP_RED;

    // Position at x and y coords
    sprite_set_x(result.entity_sprite, x);
    sprite_set_y(result.entity_sprite, y);

    result.hp = 25;

    result.name = "Spartan";

    result.id = result.name + "." + to_string(getMilliTime());

    result.attackTime = getMilliTime();

    return result;
}

void draw_nameplate(const entity_data &entity_to_draw)
{
    int txt_width = text_width(entity_to_draw.name, "game_font", 12);
    int txt_height = text_height(entity_to_draw.name, "game_font", 12);

    double x = center_point(entity_to_draw.entity_sprite).x - (0.5 * txt_width);
    double y = center_point(entity_to_draw.entity_sprite).y + (0.5 * sprite_height(entity_to_draw.entity_sprite) + txt_height);

    draw_text(entity_to_draw.name, COLOR_ORANGE_RED, "game_font", 12, x, y);
}

void draw_entity(entity_data &entity_to_draw)
{
    draw_nameplate(entity_to_draw);

    draw_sprite(entity_to_draw.entity_sprite);
}

void fireLaser(entity_data &entity, point_2d to, vector<item> &items)
{

    if (entity.attackTime > getMilliTime()) return;

    point_2d point = center_point(entity.entity_sprite);

    float angle = point_point_angle(point, to);

    //Create item
    item bullet = createItem(point, angle, ENEMY_LASER_BEAM, ENTITY_LASER_SPEED);

    bullet.damage = 1;

    bullet.shooter = entity.id;

    sendEntityData(entity, UPDATE_ENTITY, entity.kind);

    entity.attackTime = getMilliTime() + (1000 * ENTITY_ATTACK_COOLDOWN);

    items.push_back(bullet);
}

player_data closestPlayer(vector<player_data> &players, point_2d from)
{
    int index = -1, closestDistance = GAME_BOUNDARY;

    for (int i = 0; i < players.size(); i++)
    {
        player_data player = players[i];

        int distance = point_point_distance(from, center_point(player.player_sprite));

        if (distance < closestDistance)
        {
            index = i;
            closestDistance = distance;
        }
    }

    return players[index];
}

void attackPlayers(vector<player_data> &players, vector<entity_data> &entites, vector<item> &items)
{
    for (int z = 0; z < entites.size(); z++)
    {
        entity_data &entity = entites[z];

        if (!isInBoundary(entity.entity_sprite))
        {
            move_sprite_to(entity.entity_sprite, GAME_BOUNDARY / 3, GAME_BOUNDARY / 3);
            continue;
        }

        player_data player = closestPlayer(players, center_point(entity.entity_sprite));
        int closestPlayerDistance = point_point_distance(center_point(entity.entity_sprite), center_point(player.player_sprite));

        //If player within range follow player
        if (closestPlayerDistance <= ENTITY_PURSUE_RANGE)
        {
            vector_2d followVector = unit_vector(vector_from_to(entity.entity_sprite, player.player_sprite));
            followVector = vector_multiply(followVector, ENTITY_SPEED);

            move_sprite(entity.entity_sprite, followVector);

            fireLaser(entity, center_point(player.player_sprite), items);

            //TODO: Put up shield or shoot laser beam
        }
    }
}
