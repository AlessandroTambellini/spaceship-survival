#include "raylib.h"
#include <stdio.h>

#define FPS 60 // Frames Per Second
#define ASI 0.125f // Asteroid Speed Increment

#define MAX_BULLETS 512
#define MAX_ASTEROID_SPEED 16.0f
#define SPACESHIP_SPEED 8.0f
#define SPACESHIP_SCALE 0.5f
#define FLAME_SCALE 0.5f
#define BULLET_SCALE 0.5f
#define BULLET_SPEED 8.0f
#define PLAY_BTN_INLINE_PADDING 30.0f
#define PLAY_BTN_BLOCK_PADDING 15.0f

// style
#define H1_FS 30 // FS: Font-Size
#define H2_FS 25
#define P_FS 16
#define BTN_FS 30

const int screen_width = 1024;
const int screen_height = 600;

Music bkg_music;
Sound propulsion_sound;
Sound shoot_sound;
Sound asteroid_explosion_sound;
Sound spaceship_explosion_sound;

Texture2D background;
Texture2D spaceship;
Texture2D spaceship_explosion;
Texture2D flame;
Texture2D bullet;
Texture2D asteroid;
Texture2D asteroid_explosion;

Rectangle spaceship_bounds;
Vector2 asteroid_center;

Vector2 bullets[MAX_BULLETS] = {0};
float bullet_width;
float bullet_height;
int num_bullets;

bool bkg_music_on;
float bkg_scroll;

bool show_spaceship_flames;
bool is_asteroid_exploded;
double asteroid_explotion_time;
bool is_spaceship_exploded;
double spaceship_explotion_time;

Vector2 infobar_pos;
float asteroid_speed;
int asteroids_destroyed;
double last_ASI_time;
double play_start_time;
double game_duration;

/* Menù */
int title_fs;
Vector2 title_pos;

Vector2 play_btn_txt;
Rectangle play_btn_bounds;
Color play_btn_color;
Color play_btn_text_color;
bool play_btn_pressed;
float play_btn_press;

Vector2 preferences_title_pos;
int preferences_title_fs;

Vector2 bkg_sound_setting;
float bkg_sound_btn_left_margin;
float bkg_sound_btn_border;
float on_off_box_size;
Rectangle bkg_sound_btn_bounds;

/* gameover */
int gameover_title_fs;
Vector2 gameover_title_pos;
int stats_fs;
Vector2 asteroids_destroyed_txt;
Vector2 game_duration_txt;
Vector2 play_again_btn_txt;
Rectangle play_again_btn_bounds;
bool play_again_btn_hover_clr;

Vector2 menu_btn_txt;
Rectangle menu_btn_bounds;
bool menu_btn_hover_clr;

bool play;
bool gameover;

void init_variables(void);
void update_play_frame(void);
void update_gameover_frame(void);
void update_menu_frame(void);
void draw_play_frame(void);
void draw_gameover_frame(void);
void draw_menu_frame(void);
void unload_game(void);
void reset_game_variables(void);

int main(void) {

    InitWindow(screen_width, screen_height, "Spaceship Survival");

    InitAudioDevice();
    init_variables();

    SetTargetFPS(FPS); // FPS regulates also audio
    PlayMusicStream(bkg_music);

    while (!WindowShouldClose()) 
    {
        if (play) update_play_frame();
        else if (gameover) update_gameover_frame();
        else update_menu_frame();
        // update_gameover_frame();

        BeginDrawing();
            if (play) draw_play_frame();
            else if (gameover) draw_gameover_frame();
            else draw_menu_frame();
            // draw_gameover_frame();
        EndDrawing();
    }

    unload_game();
    CloseWindow();

    return 0;
}

void draw_gameover_frame(void) {
    ClearBackground(GRAY);
    DrawText("Game Over", gameover_title_pos.x, gameover_title_pos.y, gameover_title_fs, WHITE);

    // Stats about the game
    DrawText(TextFormat("Asteroids Destroyed: %d", asteroids_destroyed), asteroids_destroyed_txt.x, asteroids_destroyed_txt.y, H2_FS, WHITE);
    DrawText(TextFormat("Game Duration: %.2f", game_duration), game_duration_txt.x, game_duration_txt.y, H2_FS, WHITE);

    // play again
    DrawRectangleRec(play_again_btn_bounds, play_again_btn_hover_clr ? WHITE : LIGHTGRAY);
    DrawText("Play Again", play_again_btn_txt.x, play_again_btn_txt.y, BTN_FS, DARKGRAY);

    // go to menù
    DrawRectangleRec(menu_btn_bounds, menu_btn_hover_clr ? WHITE : LIGHTGRAY);
    DrawText("Menù", menu_btn_txt.x, menu_btn_txt.y, BTN_FS, DARKGRAY);
}

void update_gameover_frame(void) {
    play_again_btn_hover_clr = false;
    menu_btn_hover_clr = false;
    // it's a hover effect
    if (CheckCollisionPointRec(GetMousePosition(), play_again_btn_bounds)) {
        play_again_btn_hover_clr = true;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            reset_game_variables();
            play = true;
            gameover = false;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), menu_btn_bounds)) {
        menu_btn_hover_clr = true;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            reset_game_variables();
            play = false;
            gameover = false;
        }
    }
}

void reset_game_variables(void) {
    // reset all the data
    is_spaceship_exploded = false;
    asteroids_destroyed = 0;
    asteroid_speed = 1.0f;
    play_start_time = 0.0;
    game_duration = 0.0;

    spaceship_bounds.x = (screen_width - spaceship_bounds.width) / 2;
    spaceship_bounds.y = (screen_height - spaceship_bounds.height) / 3 * 2;

    asteroid_center.x = GetRandomValue(asteroid.width / 2, screen_width - asteroid.width / 2);
    asteroid_center.y = -screen_height;

    num_bullets = 0;

    bkg_scroll = 0.0f;

    StopMusicStream(bkg_music);
    PlayMusicStream(bkg_music);
}

void draw_menu_frame(void) {
    ClearBackground(GRAY);
    
    DrawText("Spaceshp Survival", title_pos.x, title_pos.y, title_fs, WHITE);

    DrawRectangle(play_btn_bounds.x + play_btn_press, play_btn_bounds.y + play_btn_press, play_btn_bounds.width, play_btn_bounds.height, DARKGRAY); // button shadow
    DrawRectangleRec(play_btn_bounds, play_btn_color);
    DrawText("Play", play_btn_txt.x, play_btn_txt.y, BTN_FS, play_btn_text_color);

    DrawText("Preferences", preferences_title_pos.x, preferences_title_pos.y, preferences_title_fs, WHITE);
    DrawText("Background Music", bkg_sound_setting.x, bkg_sound_setting.y, P_FS, WHITE);

    DrawRectangleRec(bkg_sound_btn_bounds, DARKGRAY);
    DrawRectangle(bkg_sound_btn_bounds.x + bkg_sound_btn_border, bkg_sound_btn_bounds.y + bkg_sound_btn_border, on_off_box_size, 
        on_off_box_size, bkg_music_on ? DARKGREEN : LIGHTGRAY);
    DrawRectangle(bkg_sound_btn_bounds.x + bkg_sound_btn_border + on_off_box_size, bkg_sound_btn_bounds.y + bkg_sound_btn_border, 
        on_off_box_size, on_off_box_size, bkg_music_on ? LIGHTGRAY : MAROON);
}

void draw_play_frame(void) {
    ClearBackground(BLACK);

    DrawTexture(background, 0, bkg_scroll, WHITE);
    DrawTexture(background, 0, -(background.height - bkg_scroll), WHITE);

    for (int i = 0; i < num_bullets; i++) DrawTextureEx(bullet, bullets[i], 0.0f, BULLET_SCALE, WHITE);

    if (is_spaceship_exploded) {
        // Draw spaceship explosion
        DrawTextureEx(spaceship_explosion, (Vector2){
            spaceship_bounds.x + spaceship_bounds.width / 2 - spaceship_explosion.width / 2,
            spaceship_bounds.y + spaceship_bounds.height / 2 - spaceship_explosion.height / 2
        }, 0.0f, 1.0f, WHITE);
    } else {
        DrawTextureEx(spaceship, (Vector2){spaceship_bounds.x, spaceship_bounds.y}, 0.0f, SPACESHIP_SCALE, WHITE);
        if (show_spaceship_flames) {
            DrawTextureEx(flame, (Vector2){ 
                .x = spaceship_bounds.x + spaceship_bounds.width / 2  - flame.width * FLAME_SCALE - 12, 
                .y = spaceship_bounds.y + spaceship_bounds.height },
                0.0f, FLAME_SCALE, WHITE);
            DrawTextureEx(flame, (Vector2){ 
                .x = spaceship_bounds.x + spaceship_bounds.width / 2 + 12, 
                .y = spaceship_bounds.y + spaceship_bounds.height },
                0.0f, FLAME_SCALE, WHITE);
        }
    }
    
    if (is_asteroid_exploded) {
        DrawTextureEx(asteroid_explosion, (Vector2){
            // Center it were was the asteroid
            asteroid_center.x - (asteroid_explosion.width / 2),
            asteroid_center.y - (asteroid_explosion.height / 2)
        }, 0.0f, 1.0f, WHITE);
    } else {
        DrawTextureEx(asteroid, (Vector2){asteroid_center.x - asteroid.width / 2, asteroid_center.y - asteroid.height / 2},
            0.0f, 1.0f, WHITE);
    }

    /* The info bar is drawn last to avoid being covered by other objects,
    like an asteroid crossing the screen */
    DrawText(TextFormat("\tAsteroids Destroyed: %d, Asteroid Speed: %.2f px/sec, Time Elapsed: %.2f", 
        asteroids_destroyed, asteroid_speed * FPS, game_duration), infobar_pos.x, infobar_pos.y, P_FS, RAYWHITE);
}

void update_menu_frame(void) {
    play_btn_color = LIGHTGRAY;

    if (CheckCollisionPointRec(GetMousePosition(), play_btn_bounds)) {
        play_btn_color = WHITE;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            play_btn_bounds.y += 5;
            play_btn_bounds.x += 5;
            play_btn_txt.y += 5;
            play_btn_txt.x += 5;
            play_btn_pressed = true;
            play_btn_press = 0;
        }
        if (play_btn_pressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            play = true;
        }
    }

    if (play_btn_pressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        play_btn_bounds.y -= 5;
        play_btn_bounds.x -= 5;
        play_btn_txt.x -= 5;
        play_btn_txt.y -= 5;
        play_btn_pressed = false;
        play_btn_press = 5;
    }

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){
        bkg_sound_btn_bounds.x + bkg_sound_btn_border, bkg_sound_btn_bounds.y + bkg_sound_btn_border,
        on_off_box_size, on_off_box_size
    })) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) bkg_music_on = false;
    }

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) {
        bkg_sound_btn_bounds.x + bkg_sound_btn_border + on_off_box_size, bkg_sound_btn_bounds.y + bkg_sound_btn_border,
        on_off_box_size, on_off_box_size
    })) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) bkg_music_on = true;
    }
}

void update_play_frame(void) {
    if (play_start_time == 0) play_start_time = GetTime(); // set just once
    game_duration = GetTime() - play_start_time;

    if (bkg_music_on) UpdateMusicStream(bkg_music);

    // Increase asteroid speed every 1 seconds
    if (GetTime() - last_ASI_time >= 1.0 && asteroid_speed + ASI <= MAX_ASTEROID_SPEED) {
        asteroid_speed += ASI;
        last_ASI_time = GetTime();
    }

    bkg_scroll += 0.25f;
    if (bkg_scroll >= background.height) bkg_scroll = 0;

    if (is_spaceship_exploded && GetTime() - spaceship_explotion_time > 4.0) { /* 4 seconds
    is the duration of the spaceship explosion sound. So, the explosion sprite and the explotion
    sound last the same amount of time */
        gameover = true;
        play = false;
    }

    if (!is_asteroid_exploded && !is_spaceship_exploded && 
        CheckCollisionCircleRec(asteroid_center, asteroid.width / 2, spaceship_bounds)
    ) {
        PlaySound(spaceship_explosion_sound); // boooooom! sound
        // The spaceship explodes and the asteroid continues for its path
        is_spaceship_exploded = true;
        spaceship_explotion_time = GetTime();
    }

    /*
    *
    *   Update position of the asteroid
    */
    if (!is_asteroid_exploded) {
        asteroid_center.y += asteroid_speed;
        if (asteroid_center.y > screen_height + asteroid.height / 2) {
            // Start again from top of the screen at random x-coordinate
            asteroid_center.x = GetRandomValue(asteroid.width / 2, screen_width - asteroid.width / 2);
            asteroid_center.y = -asteroid.height / 2;
        }
    }

    if (is_asteroid_exploded && GetTime() - asteroid_explotion_time > 1.0) { /* (GetTime() -  asteroid_explotion_time)
        is the time elapsed from the explosion */
        is_asteroid_exploded = false;
        // Set the new position of the asteroid
        asteroid_center.x = GetRandomValue(asteroid.width / 2, screen_width - asteroid.width / 2);
        asteroid_center.y = -asteroid.height / 2;
    }

    // Check if a bullet hits an asteroid
    for (int i = 0; i < num_bullets; i++) {
        if (!is_asteroid_exploded && CheckCollisionCircleRec((Vector2) {
            asteroid_center.x, asteroid_center.y}, asteroid.width / 2,
            (Rectangle){bullets[i].x, bullets[i].y, bullet_width, bullet_height})
        ) {
            PlaySound(asteroid_explosion_sound);
            is_asteroid_exploded = true;
            asteroid_explotion_time = GetTime();
            // The bullet is removed ==> shift all the other bullets to the left
            num_bullets -= 1;
            for (int j = i; j < num_bullets; j++) {
                bullets[j].x = bullets[j + 1].x; 
                bullets[j].y = bullets[j + 1].y; 
            }

            asteroids_destroyed += 1;
        }
    }
    
    // check if the first shoot is out of the screen
    if (num_bullets > 0 && bullets[0].y + bullet_height <= 0) {
        // shift all the other shoots to the left like on an
        // assembly line
        num_bullets -= 1;
        for (int i = 0; i < num_bullets; i++) {
            bullets[i].y = bullets[i+1].y; 
            bullets[i].x = bullets[i+1].x; 
        }
    }
    // update the position of all the other shoots        
    for (int i = 0; i < num_bullets; i++) bullets[i].y -= BULLET_SPEED;

    /*
    *   
    *   Spaceship movements
    */
    if (!is_spaceship_exploded && IsKeyDown(KEY_W)) {
        if (spaceship_bounds.y >= SPACESHIP_SPEED) {
            spaceship_bounds.y -= SPACESHIP_SPEED;
        }
    }

    if (!is_spaceship_exploded && IsKeyDown(KEY_S)) {
        if (spaceship_bounds.y + spaceship_bounds.height + SPACESHIP_SPEED <= infobar_pos.y) {
            spaceship_bounds.y += SPACESHIP_SPEED;
        }
    }

    if (!is_spaceship_exploded && IsKeyDown(KEY_A)) {
        /* Half of the spaceship can exceed the border of the window to allow shooting
        to an asteroid that is passing near the edge of the screen.
        The same is true for the right-side of the screen */
        if (spaceship_bounds.x >= -spaceship_bounds.width / 2 + SPACESHIP_SPEED) {
            spaceship_bounds.x -= SPACESHIP_SPEED;
        }
    }

    if (!is_spaceship_exploded && IsKeyDown(KEY_D)) {
        if (spaceship_bounds.x + spaceship_bounds.width / 2 + SPACESHIP_SPEED <= screen_width) {
            spaceship_bounds.x += SPACESHIP_SPEED;
        }
    }

    // Start the propulsion
    if (!is_spaceship_exploded && IsKeyPressed(KEY_W)) {
        PlaySound(propulsion_sound);
        show_spaceship_flames = true;
    }

    // Stop the propulsion
    if (IsKeyReleased(KEY_W) || is_spaceship_exploded) {
        StopSound(propulsion_sound);
        show_spaceship_flames = false;
    }

    // Shoot
    if (!is_spaceship_exploded && IsKeyPressed(KEY_SPACE)) {
        if (num_bullets >= MAX_BULLETS) {
            fprintf(stderr, "Warning: max number of shooted bullets reached!\n");
        } else {
            num_bullets += 1;
            bullets[num_bullets - 1].x = spaceship_bounds.x + 
                spaceship_bounds.width / 2 - bullet_width / 2;
            bullets[num_bullets - 1].y = spaceship_bounds.y + bullet_height; /* the bullet is 
            shooted from below the spaceship */

            PlaySound(shoot_sound);
        }
    }
}

void init_variables(void) {
    bkg_music = LoadMusicStream("resources/dead-space-music.mp3");
    bkg_music.looping = true;

    /* sound effects */
    propulsion_sound = LoadSound("resources/jet-engine-startup.mp3");
    shoot_sound = LoadSound("resources/gun-shoot.mp3");
    asteroid_explosion_sound = LoadSound("resources/boom.wav");
    spaceship_explosion_sound = LoadSound("resources/spaceship_explosion.mp3");

    /* textures - they MUST be loaded after window initialization */
    background = LoadTexture("resources/stars-background.png");
    spaceship = LoadTexture("resources/spaceship.png");
    spaceship_explosion = LoadTexture("resources/spaceship_explosion.png");
    flame = LoadTexture("resources/plasm.png");
    bullet = LoadTexture("resources/laser-3.png");
    asteroid = LoadTexture("resources/small-asteroid-resized.png");
    asteroid_explosion = LoadTexture("resources/explosion.png");

    /* components bounds/position */
    spaceship_bounds.width = spaceship.width * SPACESHIP_SCALE,
    spaceship_bounds.height = spaceship.height * SPACESHIP_SCALE,
    spaceship_bounds.x = (screen_width - spaceship_bounds.width) / 2;
    spaceship_bounds.y = (screen_height - spaceship_bounds.height) / 3 * 2;

    asteroid_center.x = GetRandomValue(asteroid.width / 2, screen_width - asteroid.width / 2);
    asteroid_center.y = -screen_height; /* At the start of the game, the asteroid will come from 
    far away to let the user "setup" mentally */

    /* infobar */
    infobar_pos.x = 0;
    infobar_pos.y = screen_height - (P_FS + 8); // 8px of bottom-padding
    asteroid_speed = 1.0f;
    asteroids_destroyed = 0;
    last_ASI_time = 0.0;
    play_start_time = 0.0;
    game_duration = 0.0;

    /* other variables */
    bullet_width = bullet.width * BULLET_SCALE;
    bullet_height = bullet.height * BULLET_SCALE;
    num_bullets = 0;

    bkg_music_on = true;
    bkg_scroll = 0;

    show_spaceship_flames = false;
    is_asteroid_exploded = false;
    asteroid_explotion_time = 0.0;
    is_spaceship_exploded = false;
    spaceship_explotion_time = 0.0;
    
    play = false;
    gameover = false;

    /*
    *
    *   Menù setup
    */
    title_fs = H1_FS;
    title_pos.x = screen_width / 2 - MeasureText("Spaceshp Survival", title_fs) / 2;
    title_pos.y = 50; // random margin from the top

    play_btn_txt.x = screen_width / 2 - MeasureText("Play", BTN_FS) / 2;
    play_btn_txt.y = title_pos.y + H1_FS + PLAY_BTN_BLOCK_PADDING + 50; // 50px is margin from the above object

    play_btn_bounds.width = MeasureText("Play", BTN_FS) + PLAY_BTN_INLINE_PADDING * 2;
    play_btn_bounds.height = BTN_FS + 2 * PLAY_BTN_BLOCK_PADDING;
    play_btn_bounds.x = screen_width / 2 - play_btn_bounds.width / 2;
    play_btn_bounds.y = play_btn_txt.y - PLAY_BTN_BLOCK_PADDING;

    play_btn_color = LIGHTGRAY;
    play_btn_text_color = DARKGRAY;

    play_btn_pressed = false;
    play_btn_press = 5.0f;

    preferences_title_fs = H2_FS;
    preferences_title_pos.x = title_pos.x;
    preferences_title_pos.y = play_btn_bounds.y + 3 * play_btn_bounds.height;

    bkg_sound_setting.x = preferences_title_pos.x;
    bkg_sound_setting.y = preferences_title_pos.y + 2 * H2_FS;

    bkg_sound_btn_left_margin = 20.0f;
    bkg_sound_btn_border = 3.0f;
    on_off_box_size = 25.0f; // 25*25px square

    bkg_sound_btn_bounds.width = on_off_box_size * 2 + bkg_sound_btn_border * 2;
    bkg_sound_btn_bounds.height = (on_off_box_size + bkg_sound_btn_border * 2);
    bkg_sound_btn_bounds.x = bkg_sound_setting.x + MeasureText("Background Music", P_FS) + bkg_sound_btn_left_margin;
    bkg_sound_btn_bounds.y = bkg_sound_setting.y - (bkg_sound_btn_bounds.height - P_FS) / 2;

    /*
    *
    *   Gameover Setup
    */
    gameover_title_fs = H1_FS;
    gameover_title_pos.x = screen_width / 2 - MeasureText("Game Over", gameover_title_fs) / 2;
    gameover_title_pos.y = title_pos.y;

    stats_fs = H2_FS;
    asteroids_destroyed_txt.x = screen_width / 2 - MeasureText("Asteroids Destroyed: ", stats_fs) / 2;
    asteroids_destroyed_txt.y = gameover_title_pos.y + H1_FS + 30;
    
    game_duration_txt.x = screen_width / 2 - MeasureText("Asteroids Destroyed: ", stats_fs) / 2;
    game_duration_txt.y = asteroids_destroyed_txt.y + H2_FS + 20;

    play_again_btn_txt.x = screen_width / 2 - MeasureText("Play Again", BTN_FS) / 2;
    play_again_btn_txt.y = game_duration_txt.y + H2_FS + PLAY_BTN_BLOCK_PADDING + 50; // 50px is margin from the above object

    play_again_btn_bounds.width = MeasureText("Play Again", BTN_FS) + PLAY_BTN_INLINE_PADDING * 2;
    play_again_btn_bounds.height = BTN_FS + PLAY_BTN_BLOCK_PADDING * 2;
    play_again_btn_bounds.x = screen_width / 2 - play_again_btn_bounds.width / 2;
    play_again_btn_bounds.y = play_again_btn_txt.y - PLAY_BTN_BLOCK_PADDING;

    play_again_btn_hover_clr = false;

    menu_btn_txt.x = screen_width / 2 - MeasureText("Menù", BTN_FS) / 2;
    menu_btn_txt.y = play_again_btn_txt.y + H1_FS + PLAY_BTN_BLOCK_PADDING + 50; // 50px is margin from the above object
    
    menu_btn_bounds.width = MeasureText("Menù", BTN_FS) + PLAY_BTN_INLINE_PADDING * 2;
    menu_btn_bounds.height = BTN_FS + PLAY_BTN_BLOCK_PADDING * 2;
    menu_btn_bounds.x = screen_width / 2 - menu_btn_bounds.width / 2;
    menu_btn_bounds.y = menu_btn_txt.y - PLAY_BTN_BLOCK_PADDING;

    menu_btn_hover_clr = false;
}

void unload_game(void) {
    UnloadMusicStream(bkg_music);
    
    UnloadSound(propulsion_sound);
    UnloadSound(asteroid_explosion_sound);
    UnloadSound(shoot_sound);
    UnloadSound(spaceship_explosion_sound);

    UnloadTexture(background);
    UnloadTexture(asteroid);
    UnloadTexture(spaceship);
    UnloadTexture(flame);
    UnloadTexture(asteroid_explosion);
    UnloadTexture(spaceship_explosion);
    UnloadTexture(bullet);
}
