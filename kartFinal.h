#include "vec2.h"

struct kart{
  struct vec2 direction;
  struct vec2 position;
  struct vec2 velocity;
  struct vec2 acceleration;
};

struct kart initialize();
struct kart set_direction(struct kart cart, double x, double y);
struct kart set_position(struct kart cart, double x, double y);
struct kart set_acceleration(struct kart cart, double x, double y);
struct kart move(struct kart current_kart, double acc, int lr, double timePassed);
struct kart update_direction(struct kart current_kart, int lr);
struct kart update_position(struct kart current_kart, double timePassed);
struct kart update_velocity(struct kart current_kart, double timePassed);
// struct kart update_acceleration(struct kart current_kart, int xdir, int ydir);//negative number for down, left, position for up, right, 0 for neither
// int add_acceleration(double currentA, double addition, int dir);
struct vec2 vec2_addition(struct vec2 A, struct vec2 B);
struct vec2 scalar_mult(struct vec2 A, double multiplier);
