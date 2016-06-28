int32_t circ_index(int32_t index, int32_t size)
{
	if (size <= 0)
		return 0;

	while (index < 0)
		index += size;

	while (index >= size)
		index -= size;

	return index;
}

// example call: memset_circular(buffer, 0, sizeof(double), 120, buffer_start, buffer_size);
void *memset_circular(void *s, int c, int32_t esize, int32_t num, int32_t pos, int32_t limit)
{
	uint8_t *sb = s;

	pos = circ_index(pos, limit);

	if (pos+num <= limit)
		memset(&sb[pos*esize], c, num * esize);
	else
	{
		memset(&sb[pos*esize], c, (limit-pos) * esize);
		memset(s, c, (num-(limit-pos)) * esize);
	}

	return s;
}

// example call: memcpy_circular(dest, src, sizeof(double), 120, buffer_start, buffer_size);
void *memcpy_circular(void *dest, void *src, int32_t esize, int32_t num, int32_t pos, int32_t limit)
{
	uint8_t *db = dest, *sb = src;

	pos = circ_index(pos, limit);

	if (pos+num <= limit)
		memcpy(db, &sb[pos*esize], num * esize);
	else
	{
		memcpy(db, &sb[pos*esize], (limit-pos) * esize);
		memcpy(&db[(limit-pos) * esize], sb, (num-(limit-pos)) * esize);
	}

	return dest;
}
