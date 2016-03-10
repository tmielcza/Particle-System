kernel
void	cube(const int num, global float4 * const restrict a)
{
	unsigned int idx = get_global_id(0);
	unsigned int k = ceil(cbrt((float)num));
	float x = (float)(idx % k) / (float)k * 0.5f;
	float y = (float)((idx / k) % k) / (float)k * 0.5f;
	float z = (float)(idx / (k * k)) / (float)k * 0.5f;
	float4 pos = (float4)(x, y, z, 1.0) - (float4)(0.25, 0.25, 0.25, 0);
	a[idx] = pos;
}

inline float	rand(float x)
{
	float	t = sin(dot((float2)(x, x * x), (float2)(12.9898f ,78.233f))) * 43758.5453f;
	return ((t - floor(t)) - 0.5f);
}

kernel
void	sphere(const int num, global float4 * const restrict a)
{
	unsigned int idx = get_global_id(0);
	float4 pos = (float4)(rand(idx), rand(idx + 2.f), rand(idx + 4.f), 0.f);
	pos = normalize(pos);
	pos = pos * (1.0f - (pow((rand(idx + 8.f) + 0.5f), 3))) * 0.25f;
	pos.w = 1.f;
	a[idx] = pos;
}

kernel
void	update(const int num, global float4 * const restrict positions,
			   global float4 * const restrict velocities, const float4 center)
{
	unsigned int idx = get_global_id(0);
	float4 vel = velocities[idx];
	float4 gravity = center - positions[idx];
	float4 acceleration = 1.0f / length(gravity) * normalize(gravity);
	positions[idx] += vel * 0.01f;
	velocities[idx] = 0.99f * vel + acceleration * 0.01f;
}

kernel
void	update_no_g(const int num, global float4 * const restrict positions,
					global float4 * const restrict velocities)
{
	unsigned int idx = get_global_id(0);
	float4 vel = velocities[idx];
	positions[idx] += vel * 0.01f;
	velocities[idx] = 0.99f * vel;
}
