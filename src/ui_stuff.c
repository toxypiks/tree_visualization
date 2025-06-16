#include "ui_stuff.h"
#include <stdio.h>
#include <assert.h>
#include <rlgl.h>
#include <stdlib.h>

#include "stb_ds.h"

#include "doubly_linked_list.h"

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

UiRect ui_rect(float x, float y, float w, float h) {
  UiRect r = {0};
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  return r;
}

UiRect rect_combine(UiRect rect0, UiRect rect1, LayoutOrient orient)
{
    if (orient == LO_VERT) {
        return (UiRect) {
            .x = MIN(rect0.x, rect1.x),
            .y = MIN(rect0.y, rect1.y),
            .w = rect0.w,
            .h = rect0.h + rect1.h,
        };
    } else { // LO_HORZ
        return (UiRect) {
            .x = MIN(rect0.x, rect1.x),
            .y = MIN(rect0.y, rect1.y),
            .w = rect0.w + rect1.w,
            .h = rect0.h,
        };
    }
}

UiRect layout_slot_loc(Layout *l, const char *file_path, int line)
{
  if (l->i >= l->count) {
    fprintf(stderr, "%s:%d: ERROR: Layout overflow\n", file_path, line);
    exit(1);
  }

  UiRect r = {0};

  switch (l->orient) {
  case LO_HORZ:
    r.w = l->rect.w / l->count;
    r.h = l->rect.h;
    r.x = l->rect.x + l->i * r.w;
    r.y = l->rect.y;

    if (l->i == 0) { // First
      r.w -= l->gap / 2;
    } else if (l->i >= l->count - 1) { // Last
      r.x += l->gap / 2;
      r.w -= l->gap / 2;
    } else { // Middle
      r.x += l->gap / 2;
      r.w -= l->gap;
    }

    break;

  case LO_VERT:
    r.w = l->rect.w;
    r.h = l->rect.h / l->count;
    r.x = l->rect.x;
    r.y = l->rect.y + l->i * r.h;

    if (l->i == 0) { // First
      r.h -= l->gap / 2;
    } else if (l->i >= l->count - 1) { // Last
      r.y += l->gap / 2;
      r.h -= l->gap / 2;
    } else { // Middle
      r.y += l->gap / 2;
      r.h -= l->gap;
    }

    break;

  default:
    assert(0 && "Unreachable");
  }

  l->i += 1;

  return r;
}

void layout_stack_push(LayoutStack *ls, LayoutOrient orient, UiRect rect, size_t count, float gap)
{
  Layout l = {0};
  l.orient = orient;
  l.rect = rect;
  l.count = count;
  l.gap = gap;
  da_append(ls, l);
}

void layout_stack_delete(LayoutStack *ls){
    ls->capacity = 0;
    ls->count = 0;
    free(ls->items);
}

static Color colors[] = {
    PINK,
    YELLOW,
    RED,
    BLUE,
    MAROON,
    GREEN,
    LIME,
    SKYBLUE,
    GOLD,
    PURPLE,
    VIOLET,
    BEIGE,
    BROWN,
};

void widget(UiRect r, Color c) { DrawRectangle(r.x, r.y, r.w, r.h, c); }

void tree_widget(UiRect r, TreeState *tree_state_print, Color background)
{
    float x = r.x;
    float y = r.y;
    float w = r.w;
    float h = r.h;

    for (size_t i = 0; i < arrlen(tree_state_print->edge_coords); ++i) {
        DrawLine((int)(tree_state_print->edge_coords[i].start_x*w + x),
                 (int)(tree_state_print->edge_coords[i].start_y*h + y),
                 (int)(tree_state_print->edge_coords[i].end_x*w + x),
                 (int)(tree_state_print->edge_coords[i].end_y*h + y), PINK);
    }
    for (size_t i = 0; i < hmlen(tree_state_print->tree_map); ++i) {
        int balance = get_balance(tree_state_print->tree_map[i].key, tree_state_print->tree_depth_map);
        int circle_x = (int)(tree_state_print->tree_map[i].value.x*w + x);
        int circle_y = (int)(tree_state_print->tree_map[i].value.y*h +  y);
        float radius = 0.4f*tree_state_print->max_radius*w*0.5f;
        if (tree_state_print->tree_insert_state) {
            // TODO better check for equal pointer
            if (tree_state_print->tree_map[i].key->data == tree_state_print->tree_insert_state->tmp->data) {
                DrawCircle(circle_x, circle_y, 1.2*radius, ORANGE);
                DrawCircle(circle_x, circle_y, radius, background);
            }
        }
        DrawCircle(circle_x, circle_y, radius, GREEN);
        DrawCircle(circle_x, circle_y, 0.9f*radius, background);

        char text[8];
        sprintf(text, "%d", tree_state_print->tree_map[i].key->data);
        int len_pos = strlen(text)*0.2 + 0.3f;
        float font_size = 0.8f * radius;
        DrawText(text, circle_x - radius*len_pos, circle_y - radius*0.3f, font_size, GREEN);

        char balance_text[8];
        sprintf(balance_text, "%d", balance);
        int balance_len_pos = strlen(balance_text)*0.2 + 0.3f;
        float balance_font_size = 0.7f * radius;
        DrawText(balance_text, circle_x - radius*balance_len_pos + 1.2*radius, circle_y - radius*0.3f, balance_font_size, ORANGE);
    }
}

void tree_list_widget(UiRect r, TreeList *tree_list, Color background)
{
    float x = r.x;
    float y = r.y;
    float w = r.w;
    float h = r.h;

    TreeLE *current = get_current(tree_list);
    TreeState* tree_state_print[4] = {0};
    tree_state_print[2] = &(current->tree_state);
    if (current->prev) {
        tree_state_print[1] = &(current->prev->tree_state);
        if (current->prev->prev) {
            tree_state_print[0] = &(current->prev->prev->tree_state);
        }
    }
    if (current->next) {
        tree_state_print[3] = &(current->next->tree_state);
    }

    LayoutStack ls = {0};
    layout_stack_push(&ls, LO_HORZ, ui_rect(x, y, w, h), 4, 0);

    for (size_t i = 0; i < 4 ;++i) {
        UiRect ui_rect = layout_stack_slot(&ls);
        if(tree_state_print[i]) {
            tree_widget(ui_rect, tree_state_print[i], background);
        } else { // no tree
            widget(ui_rect, background);
        }
        Rectangle rect = {.x = ui_rect.x, .y = ui_rect.y, .width = ui_rect.w, .height = ui_rect.h };
        Color color = WHITE;
        float thickness = 1.0f;
        if (i == 2) {
            color = GREEN;
            thickness = 4.0f;
        }
        DrawRectangleLinesEx(rect, thickness, color);
    }
}

UiStuff* create_ui_stuff(size_t screen_width, size_t screen_height){
  UiStuff* ui_stuff = (UiStuff*)malloc(sizeof(UiStuff));
  // init part
  ui_stuff->screen = LoadRenderTexture(screen_width, screen_height);
  return ui_stuff;
}

void ui_stuff_clear(UiStuff* ui_stuff) {
  free(ui_stuff);
  return;
}
