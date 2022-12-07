#ifndef FORGE_VECTOR_H
#define FORGE_VECTOR_H

typedef struct Vector2i
{
	i32 x, y;
}Vector2i;

typedef struct Vector3i
{
	i32 x, y, z;
}Vector3i;

typedef struct Vector4i
{
	i32 x, y, z, w;
}Vector4i;

typedef struct Vector2f
{
	f32 x, y;
}Vector2f;

typedef struct Vector3f
{
	f32 x, y, z;
}Vector3f;

typedef struct Vector4f
{
	f32 x, y, z, w;
}Vector4f;

FR_API inline Vector2i v2i(i32 x, i32 y);
FR_API inline Vector3i v3i(i32 x, i32 y, i32 z);
FR_API inline Vector4i v4i(i32 x, i32 y, i32 z, i32 w);

FR_API inline Vector2f v2f(f32 x, f32 y);
FR_API inline Vector3f v3f(f32 x, f32 y, f32 z);
FR_API inline Vector4f v4f(f32 x, f32 y, f32 z, f32 w);

#endif //FORGE_VECTOR_H