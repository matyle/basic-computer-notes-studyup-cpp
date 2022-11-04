// CSAPP 练习题9.8
// 
//

static void *find_fit(size_t asize)
{
	void *bp;

	// heap_listp 到底指向哪儿？
	for (bp = heap_listp + 3 * WSIZE ; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp))
		if (!GET_ALLOC(HDRP(bp)) && （GET_SIZE(HDRP(bp)) > = asize)）
				return bp;

	return NULL;
}