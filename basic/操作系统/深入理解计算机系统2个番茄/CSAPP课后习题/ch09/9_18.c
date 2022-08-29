// CSAPP 9.18
// 
// 此题没有头绪。

static void place(void *bp, size_t asize)
{
	size_t csize = GET_SIZE(HDRP(bp));
	if ((csize - asize) >= (2 * DSIZE)) {		// 分割
		PUT(FTPR(bp), PACK(GET_SIZE(HDRP(bp)) - asize, 0));	// 设置剩下的空白块
		PUT(HDRP(bp + asize), PACK(GET_SIZE(HDRP(bp)) - asize, 0));
		PUT(HDRP(bp), PACK(asize, 1));						// 设置创建出来的块
		// PUT(FTPR(bp), PACK(asize, 1));					// 不需要脚部
	} else {			// 此时不必分割
		PUT(HDRP(bp), PACK(GET_SIZE(HDRP(bp)), 1));			// 设置创建出来的块
		// PUT(FTPR(bp), PACK(GET_SIZE(HDRP(bp)), 1));
	}
}