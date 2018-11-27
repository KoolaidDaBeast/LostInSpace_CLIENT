#include "splashkit.h"
#include "server_manager.h"
#include "lost_in_space.h"
#include "player.h"

bitmap ship_bitmap(ship_kind kind)
{
    switch (kind)
    {
    case SHIP_BLUE:
        return bitmap_named("ship_blue");
    case SHIP_GREEN:
        return bitmap_named("ship_green");
    default:
        return bitmap_named("ship_red");
    }
}

player_data new_player()
{
    player_data result;

    bitmap default_bitmap = ship_bitmap(SHIP_BLUE);

    // Create the sprite with 3 layers - we can turn on and off based
    // on the ship kind selected
    result.player_sprite = create_sprite(default_bitmap);

    sprite_add_layer(result.player_sprite, ship_bitmap(SHIP_GREEN), "SHIP_GREEN");
    sprite_add_layer(result.player_sprite, ship_bitmap(SHIP_RED), "SHIP_RED");

    // Default to layer 0 = SHIP_BLUE so hide others
    sprite_hide_layer(result.player_sprite, 1);
    sprite_hide_layer(result.player_sprite, 2);

    result.kind = SHIP_BLUE;

    // Position in the centre of the initial screen
    sprite_set_x(result.player_sprite, (screen_width() - sprite_width(result.player_sprite)) / 2);
    sprite_set_y(result.player_sprite, (screen_height() - sprite_height(result.player_sprite)) / 2);

    //Alter anchor point
    // point_2d anchorPoint;

    // anchorPoint.y = sprite_height(result.player_sprite) / 2;
    // anchorPoint.x = sprite_width(result.player_sprite) / 2;

    // sprite_set_anchor_point(result.player_sprite, anchorPoint);

    result.hidden = false;
    result.hp = 100;
    result.attackTime = getMilliTime();

    return result;
}

player_data new_player(double x, double y)
{
    player_data result;

    bitmap default_bitmap = ship_bitmap(SHIP_BLUE);

    // Create the sprite with 3 layers - we can turn on and off based
    // on the ship kind selected
    result.player_sprite = create_sprite(default_bitmap);

    sprite_add_layer(result.player_sprite, ship_bitmap(SHIP_GREEN), "SHIP_GREEN");
    sprite_add_layer(result.player_sprite, ship_bitmap(SHIP_RED), "SHIP_RED");

    // Default to layer 0 = SHIP_BLUE so hide others
    sprite_hide_layer(result.player_sprite, 1);
    sprite_hide_layer(result.player_sprite, 2);

    result.kind = SHIP_BLUE;

    //Rotate ship

    // Position in the centre of the initial screen
    sprite_set_x(result.player_sprite, x);
    sprite_set_y(result.player_sprite, y);

    //Alter anchor point
    // point_2d anchorPoint;

    // anchorPoint.y = sprite_height(result.player_sprite) / 2;
    // anchorPoint.x = sprite_width(result.player_sprite) / 2;

    // sprite_set_anchor_point(result.player_sprite, anchorPoint);

    result.hidden = false;
    result.hp = 100;

    return result;
}

void draw_nameplate(const player_data &player_to_draw)
{
    int txt_width = text_width(player_to_draw.name, "game_font", 12);
    int txt_height = text_height(player_to_draw.name, "game_font", 12);

    double x = center_point(player_to_draw.player_sprite).x - (0.5 * txt_width);
    double y = center_point(player_to_draw.player_sprite).y + (0.5 * sprite_height(player_to_draw.player_sprite) + txt_height);

    draw_text(player_to_draw.name, COLOR_WHITE, "game_font", 12, x, y);
}

void draw_player(player_data &player_to_draw)
{
    if (player_to_draw.hidden)
    {
        return;
    }

    draw_nameplate(player_to_draw);

    // float shield_x = sprite_position(player_to_draw.player_sprite).x - sprite_width(player_to_draw.player_sprite) / 2;
    // float shield_y = sprite_position(player_to_draw.player_sprite).y + sprite_height(player_to_draw.player_sprite) / 2;

    // if (player_to_draw.shield_active)
    // {
    //     draw_bitmap("shield", shield_x, shield_y, option_to_screen());
    // }

    draw_sprite(player_to_draw.player_sprite);
}

void update_player(player_data &player_to_update)
{
    // Test edge of screen boundaries to adjust the camera
    double left_edge = camera_x() + SCREEN_BORDER;
    double right_edge = left_edge + screen_width() - 2 * SCREEN_BORDER;
    double top_edge = camera_y() + SCREEN_BORDER;
    double bottom_edge = top_edge + screen_height() - 2 * SCREEN_BORDER;

    // Get the center of the player
    point_2d sprite_center = center_point(player_to_update.player_sprite);

    // Test if the player is outside the area and move the camera
    // the player will appear to stay still and everything else
    // will appear to move :)

    // Test top/bottom of screen
    if (sprite_center.y < top_edge)
    {
        move_camera_by(0, sprite_center.y - top_edge);
    }
    else if (sprite_center.y > bottom_edge)
    {
        move_camera_by(0, sprite_center.y - bottom_edge);
    }

    // Test left/right of screen
    if (sprite_center.x < left_edge)
    {
        move_camera_by(sprite_center.x - left_edge, 0);
    }
    else if (sprite_center.x > right_edge)
    {
        move_camera_by(sprite_center.x - right_edge, 0);
    }
}

void player_switch_to_ship(player_data &player, ship_kind target)
{
    if (player.kind != target)
    {
        sprite_show_layer(player.player_sprite, static_cast<int>(target));

        sprite_hide_layer(player.player_sprite, static_cast<int>(player.kind));

        player.kind = target;
    }
}

bool isInBoundary(sprite &spr)
{
    int x = sprite_position(spr).x + sprite_width(spr);
    int y = sprite_position(spr).y + sprite_height(spr);

    if (x < 0)
        x *= -1;

    if (y < 0)
        y *= -1;

    if (x < GAME_BOUNDARY && y < GAME_BOUNDARY)
        return true;

    return false;
}

void fireLaser(player_data &player, vector<item> &items)
{
    if (player.attackTime > getMilliTime()) return;

    point_2d point = center_point(player.player_sprite);
    point_2d mousePoint = to_world(mouse_position());

    float angle = point_point_angle(point, mousePoint);

    //Create item
    item bullet1 = createItem(point, angle, LASER_BEAM, LASER_BEAM_SPEED);

    bullet1.damage = 3;

    bullet1.shooter = player.id;

    player.attackTime = getMilliTime() + (1000 * PLAYER_ATTACK_COOLDOWN);

    sendPlayerItemData(player, SPAWN_ITEM, bullet1);

    items.push_back(bullet1);
}

void handle_input(player_data &player, vector<item> &items)
{
    //If player reaches GAME_BOUNDARY teleport them back to the middle
    if (!isInBoundary(player.player_sprite))
    {
        move_sprite_to(player.player_sprite, 400, 400);
        return;
    }

    // Handle movement - rotating left/right and moving forward/back
    float rotation = sprite_rotation(player.player_sprite);

    // Allow rotation with left/right keys
    if (key_typed(W_KEY))
    {
        //sprite_set_dx(player.player_sprite, dx - PLAYER_SPEED);
        if (sprite_dy(player.player_sprite) == 0)
        {
            sprite_set_dy(player.player_sprite, -PLAYER_SPEED);
            sprite_set_dx(player.player_sprite, 0);
        }
        else if (sprite_dy(player.player_sprite) > 0)
        {
            sprite_set_dy(player.player_sprite, -0);
            sprite_set_dx(player.player_sprite, 0);
        }
        else
        {
            sprite_set_dy(player.player_sprite, -PLAYER_SPEED);
            sprite_set_dx(player.player_sprite, 0);
        }
        sendPlayerData(player, UPDATE_PLAYER, player.kind);
    }

    if (key_typed(S_KEY))
    {
        //sprite_set_dx(player.player_sprite, dx + PLAYER_SPEED);
        if (sprite_dy(player.player_sprite) == 0)
        {
            sprite_set_dy(player.player_sprite, PLAYER_SPEED);
            sprite_set_dx(player.player_sprite, 0);
        }
        else if (sprite_dy(player.player_sprite) < 0)
        {
            sprite_set_dy(player.player_sprite, 0);
            sprite_set_dx(player.player_sprite, 0);
        }

        else
        {
            sprite_set_dy(player.player_sprite, PLAYER_SPEED);
            sprite_set_dx(player.player_sprite, 0);
        }
        sendPlayerData(player, UPDATE_PLAYER, player.kind);
    }

    else if (key_down(A_KEY))
    {
        //sprite_set_rotation(player.player_sprite, rotation - PLAYER_ROTATE_SPEED);
        sprite_set_dx(player.player_sprite, -PLAYER_SPEED);
        sprite_set_dy(player.player_sprite, 0);
        sendPlayerData(player, UPDATE_PLAYER, player.kind);
    }

    else if (key_down(D_KEY))
    {
        //sprite_set_rotation(player.player_sprite, rotation + PLAYER_ROTATE_SPEED);
        sprite_set_dx(player.player_sprite, PLAYER_SPEED);
        sprite_set_dy(player.player_sprite, 0);
        sendPlayerData(player, UPDATE_PLAYER, player.kind);
    }

    if (mouse_clicked(LEFT_BUTTON))
    {
        fireLaser(player, items);
    }

    if (mouse_clicked(RIGHT_BUTTON))
    {
        write_line(player.name + " shield activated!");
        player.shield_active = true;
    }
}

string convert_point_to_string(point_2d point)
{
    //Turns float value into whole number
    int x = point.x;
    int y = point.y;

    string result =
        "X: " + to_string(x) + " " +
        "Y: " + to_string(y);

    return result;
}

int distance_to_point(point_2d from, point_2d to)
{
    int distance = point_point_distance(from, to);

    return distance;
}

void draw_HUD(const player_data &player)
{
    int hpXOffset = text_width("HP: ", "game_font", 12);

    //Draw Display Name, Location
    draw_text("Display Name: " + player.name, COLOR_WHITE, "game_font", 12, 0, 5, option_to_screen());
    draw_text("Location: " + convert_point_to_string(center_point(player.player_sprite)), COLOR_WHITE, "game_font", 12, 0, 20, option_to_screen());

    // //Draw Health Bar
    // const int HP_WIDTH = 135;
    // const int HP_HEIGHT = 10;
    // int currentHP = HP_WIDTH * (((double)player.hp) / 100);

    // draw_text("HP: ", COLOR_WHITE, "game_font", 12, 0, 35, option_to_screen());

    // draw_rectangle(COLOR_BLACK, hpXOffset + 2, 35, HP_WIDTH, HP_HEIGHT, option_to_screen());
    // fill_rectangle(COLOR_GREEN, hpXOffset + 2, 35, currentHP, HP_HEIGHT, option_to_screen());

    //Draw HP bar Image
    draw_bitmap("empty_hp_bar", screen_width() - bitmap_width("empty_hp_bar"), screen_height() - bitmap_height("empty_hp_bar"), option_to_screen());

    drawing_options option = option_part_bmp(0, 0, (((double)player.hp) / 100) * bitmap_width("full_hp_bar"), bitmap_height("full_hp_bar"), option_to_screen());

    draw_bitmap("full_hp_bar", screen_width() - bitmap_width("full_hp_bar"), screen_height() - bitmap_height("full_hp_bar"), option);
}
