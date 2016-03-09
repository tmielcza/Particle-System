kernel
void	add(const int num, global float4 * const restrict a)
{
	unsigned int idx = get_global_id(0);
	unsigned int k = ceil(cbrt((float)num));
	float x = (float)(idx % k) / (float)k * 0.5f;
	float y = (float)((idx / k) % k) / (float)k * 0.5f;
	float z = (float)(idx / (k * k)) / (float)k * 0.5f;
	float4 pos = (float4)(x, y, z, 1.0) - (float4)(0.25, 0.25, 0.25, 0);
	a[idx] = pos;}

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
