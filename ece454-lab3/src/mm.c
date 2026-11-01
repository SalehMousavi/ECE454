/*
 * ECE454 Lab 3 - Malloc
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h> // remove if for some reason this causes performance issues

#include "mm.h"
#include "memlib.h"
 
 /*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
	 /* Team name */
	 "SM",
	 /* First member's first name and last name */
	 "Saleh:Mousavi",
	 /* First member's student number */
	 "1008848945",
	 /* Second member's first name and last name (do not modify if working alone) */
	 "",
	 /* Second member's student number (do not modify if working alone) */
	 ""
};

/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
*************************************************************************/
#define WSIZE       sizeof(void *)            /* word size (bytes) */
#define DSIZE       (2 * WSIZE)            /* doubleword size (bytes) */
#define CHUNKSIZE   (1<<0) / DSIZE      /* initial heap size (double word) */

#define MAX(x,y) ((x) > (y)?(x) :(y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(uintptr_t *)(p))
#define PUT(p,val)      (*(uintptr_t *)(p) = (val))
 
 /* Size and tag bits from an already-loaded header or footer word */
 #define UNPACK_SIZE(word) ((word) & ~(DSIZE - 1))
 #define UNPACK_TAGS(word) ((word) & (DSIZE - 1))

/* Read the size and allocated fields from address p */
 #define GET_SIZE(p)     (UNPACK_SIZE(GET(p)))
 #define GET_TAGS(p)     (UNPACK_TAGS(GET(p)))
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp))
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - WSIZE)

#define NEXT_BLK(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) (GET((char *)(bp + WSIZE)))
#define PREV_BLKP(bp) (GET((char *)(bp + 2*WSIZE)))

void* heap_listp = NULL;

// sizes (in double words counting header footer pointer to next and prev): 2, 3, 4, 5-8, 9-16, 17-32, 33-64, 65-128, 129-256, 257-512
enum bins {	 
	bin_2 = 0,
	bin_3, 
	bin_4,
	bin_8,
	bin_16,
	bin_32, 
	bin_64, 
	bin_128,
	bin_256,
	bin_512,
	bin_large,
	bin_total
};
 
enum bin_maxes {
	bin_2_max = 2,
	bin_3_max = 3,
	bin_4_max = 4,
	bin_8_max = 8,
	bin_16_max = 16,
	bin_32_max = 32,
	bin_64_max = 64, 
	bin_128_max = 128, 
	bin_256_max = 256,
	bin_512_max = 512
};
 
 enum error_codes {
	 invalid_bin = 1,
	 cant_find_fit,
 };
 
 
 struct mem_tracker {
	 char * bin_pointers[bin_total];
 };
 
 static struct mem_tracker g_mem_tracker;
 static uint64_t available_dw;
 
 /**
  * @brief gets index of the head pointer for the appropriate free list
  * @param asize size of block in double words
  * @return int index of bin for the current size, -1 if illegal 
  */
 static inline int get_bin_idx(uintptr_t asize) {
	 int bin_idx;
	 if(asize < bin_2_max) {
		 bin_idx = -1;
	 }
	 else if(asize == bin_2_max) {
		 bin_idx = bin_2;
	 }
	 else if (asize == bin_3_max) {
		 bin_idx = bin_3;
	 }
	 else if (asize == bin_4_max) {
		 bin_idx = bin_4;
	 }
	 else if (asize <= bin_8_max) {
		 bin_idx = bin_8;
	 }
	 else if (asize <= bin_16_max) {
		 bin_idx = bin_16;
	 }
	 else if (asize <= bin_32_max) {
		 bin_idx = bin_32;
	 }
	 else if (asize <= bin_64_max) {
		 bin_idx = bin_64;
	 }
	 else if (asize <= bin_128_max) {
		 bin_idx = bin_128;
	 }
	 else if (asize <= bin_256_max) {
		 bin_idx = bin_256;
	 }
	 else if (asize <= bin_512_max) {
		 bin_idx = bin_512;
	 }
	 else {
		 bin_idx = bin_large;
	 }
	 return bin_idx;
 }
 
 /**
  * @brief converts size request in bytes to double words acccounting for size needed for headers
  */
 static inline size_t adjusted_size(size_t size) {
	 size_t adjusted_size = (size+ WSIZE + DSIZE - 1)/DSIZE;
     adjusted_size = (adjusted_size < 2)? 2 : adjusted_size;
	 return adjusted_size;
 }
 
 /**********************************************************
  * mm_init
  * Initialize the heap, including "allocation" of the
  * prologue and epilogue
  **********************************************************/
 int mm_init(void)
 {
	 uintptr_t init_heap_size = 2*WSIZE;
 
	 if ((heap_listp = mem_sbrk(init_heap_size)) == (void *)-1)
		 return -1;
 
	 PUT(heap_listp, PACK(0, 1));                         // sentry to indicate allocated and size zero
	 PUT(heap_listp + WSIZE, PACK(0, 3));    // sentry to indicate allocated and size zero
 
	 heap_listp += WSIZE; // skip the header sentry
 
	 for(uint bin_idx = 0; bin_idx < bin_total; bin_idx++) {
		 g_mem_tracker.bin_pointers[bin_idx] = 0;
	 }
 
	// set 512 head to start of current heap
	//  g_mem_tracker.bin_pointers[bin_512] = heap_listp;
	//  // set size and allocated bit for current and previous block
	//  PUT(heap_listp, PACK(bin_512_max*DSIZE, 2)); // bit 1 indicates prev block is allocated
	//  // set next and prev pointers
	//  PUT(heap_listp + WSIZE, 0);
	//  PUT(heap_listp + 2*WSIZE, 0);
	//  PUT(heap_listp + bin_512_max*DSIZE - WSIZE, PACK(bin_512_max*DSIZE, 2)); // free blocks keep a footer
	 available_dw = 0;
 
	 return 0;
 }
 
 /**********************************************************
  * coalesce
  * Covers the 4 cases discussed in the text:
  * - both neighbours are allocated
  * - the next block is available for coalescing
  * - the previous block is available for coalescing
  * - both neighbours are available for coalescing
  **********************************************************/
 static inline void remove_from_free(void *bp) 
 {
	 size_t size = GET_SIZE(bp);
	 int bin_idx = get_bin_idx(size/DSIZE);
	 if(bin_idx == -1) {
		 exit(invalid_bin);
	 }
 
	 char * next_bp = (char *) GET(bp + WSIZE);
	 char * prev_bp = (char *) GET(bp + 2*WSIZE);
	 if(prev_bp != NULL)
		 PUT(prev_bp+WSIZE, (uintptr_t) next_bp);
	 if(next_bp != NULL)
		 PUT(next_bp+2*WSIZE, (uintptr_t)prev_bp);
	 if(g_mem_tracker.bin_pointers[bin_idx] == bp) {
		 g_mem_tracker.bin_pointers[bin_idx] = next_bp;
	 }
 }
 
 
 /**
  * @brief collesces a block pointer with adjacent blocks, typically used when bp is about to be freed
  * @param bp, pointer to block pointer being coalesced
  * @param size_mode, bool set when only trying to get a coalesced size estimate
  * @param forward_block, size of the forward block used when in size mode
  * @param coalesced_size, intptr used as an output for estimated coalesced size.
  * @param forward_block, output for the forward free block's size in size mode; NULL otherwise.
  */
 void *coalesce(void *bp, bool size_mode, size_t* coalesced_size, size_t* forward_block, bool move)
 {
     uintptr_t hdr = GET(bp);
     size_t size = UNPACK_SIZE(hdr);
     size_t prev_alloc = UNPACK_TAGS(hdr) & 0b10;
	 char* next_bp = (char*)(bp+size);
	 size_t next_alloc = GET_ALLOC(next_bp);
 
	 if (prev_alloc && next_alloc) {       /* Case 1 */
		 if(!size_mode) {
			 uintptr_t next_header = GET(next_bp);
			 next_header &= ~0b10; // clear the next block's allocated bit
			 PUT(next_bp, next_header);
		 }
		 else {
             *forward_block = 0;
			 *coalesced_size = size;
		 }
		 return bp;
	 }
 
	 else if (prev_alloc && !next_alloc) { /* Case 2 */
		 // previous block is not free
		 // next block is free
		 size_t next_size = GET_SIZE(next_bp);
		 if(!size_mode) {
			 remove_from_free(bp+size);
			 PUT(bp, PACK(size+next_size, 2));
			 PUT(bp + size+next_size-WSIZE, PACK(size+next_size, 2));
		 }
		 else {
             *forward_block = next_size;
			 *coalesced_size = size + next_size;
		 }
		 return (bp);
	 }
 
	 else if (!prev_alloc && next_alloc) { /* Case 3 */
		 // previous block is free
		 // next block is not free
		 uintptr_t prev_ftr = GET(bp-WSIZE);
         uintptr_t prev_size = UNPACK_SIZE(prev_ftr);
         uintptr_t prev_tag = UNPACK_TAGS(prev_ftr);
		 if(!size_mode) {
			 remove_from_free(bp-prev_size);
			 PUT(bp-prev_size, PACK(size+prev_size, prev_tag));
			 if(!move)
				 PUT(bp+size-WSIZE, PACK(size+prev_size, prev_tag));
			 
			 // clear the next block's allocated bit
			 uintptr_t next_header = GET(next_bp);
			 next_header &= ~0b10; 
			 PUT(next_bp, next_header);    
		 }
		 else {
             *forward_block = 0;
			 *coalesced_size = size + prev_size;
		 }
		 return (bp-prev_size);
	 }
 
	 else {            /* Case 4 */
		 uintptr_t prev_ftr = GET(bp-WSIZE);
         uintptr_t prev_size = UNPACK_SIZE(prev_ftr);
         uintptr_t prev_tag = UNPACK_TAGS(prev_ftr);
         uintptr_t next_hdr = GET(next_bp);
         size_t next_size = UNPACK_SIZE(next_hdr);
		 uintptr_t total_size = prev_size + size + next_size;
		 
		 if(!size_mode) {
			 remove_from_free(bp+size);
			 remove_from_free(bp-prev_size);
			 PUT(bp-prev_size, PACK(total_size, prev_tag));
			 PUT(bp+size+next_size-WSIZE, PACK(total_size, prev_tag));
		 }
		 else {
             *forward_block = next_size;
			 *coalesced_size = total_size;
		 }
		 return (bp-prev_size);
	 }
 }
 
 /**********************************************************
  * extend_heap
  * Extend the heap by "words" words, maintaining alignment
  * requirements of course. Free the former epilogue block
  * and reallocate its new header
  **********************************************************/
 
 void *extend_heap(size_t words)
 {
	char *bp;
	size_t size;
 
	/* Allocate an even number of words to maintain alignments */
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ( (bp = mem_sbrk(size)) == (void *)-1 )
		return NULL;
 
	/* Initialize free block header/footer and the epilogue header */
	// read old epilogue
	bp = bp - WSIZE;
	uintptr_t ep_tag = GET_TAGS(bp) & ~0b1; // clear allocated bit
	PUT(bp, PACK(size, ep_tag));                // free block header
	PUT(bp+size-WSIZE, PACK(size, ep_tag));                // free block footer
	PUT(bp+size, PACK(0, 1));        // new epilogue header
	available_dw += size/DSIZE;
 
	/* Coalesce if the previous block was free */
	return coalesce(bp, false, NULL, NULL, false);
 }
 
 void insert_in_free(char * block) 
 {
	 uintptr_t size = GET_SIZE(block);
	 // get appropriate bin insert it in
	 uintptr_t bin_idx = get_bin_idx(size/DSIZE);
	 if(bin_idx == -1) {
		 exit(invalid_bin);
	 }
 
	 // insert it in the free list
	 char* cur_block = g_mem_tracker.bin_pointers[bin_idx];
	 if(cur_block == NULL) {
		 g_mem_tracker.bin_pointers[bin_idx] = block;
		 PUT(block + WSIZE, (uintptr_t)NULL); 
		 PUT(block + 2*WSIZE, (uintptr_t)NULL);
		 return;
	 }
 
     // insert at beginning of list
		 PUT(block + WSIZE, (uintptr_t)cur_block);
		 PUT(block + 2*WSIZE, (uintptr_t)NULL);
		 g_mem_tracker.bin_pointers[bin_idx] = block;
		 PUT(cur_block + 2*WSIZE, (uintptr_t)block);
		 return;
 }
 
/**********************************************************
* find_fit
* Traverse the heap searching for a block to fit asize double words
* Return NULL if no free blocks can handle that size
* Assumed that asize is aligned
**********************************************************/
void * find_fit(size_t asize)
{
	// get the linked list for this size
	int bin_idx = get_bin_idx(asize);
	if(bin_idx == -1) {
		exit(invalid_bin);
	}
 
	char * free_block = NULL;
	uintptr_t best_fit = UINTPTR_MAX;
	uintptr_t block_size = 0;
 
	for(; bin_idx < bin_total; ++bin_idx) { // traverse the bins until we find a free block
		// uintptr_t prev_node = NULL;
		char * cur_block = g_mem_tracker.bin_pointers[bin_idx];
		while(cur_block != NULL) { // iterate through the nodes in this bin
			block_size = GET_SIZE(cur_block);
			if(block_size >= (asize * DSIZE)) { // found a node with sufficient size
				uintptr_t difference = block_size - asize * DSIZE;
				if (best_fit > difference) {
					best_fit =  difference;
					free_block = cur_block;
				}
				if(difference == 0)
					break;
			}
			cur_block = (char *)NEXT_BLKP(cur_block);
		}
		if(free_block != NULL) {
			break;
		}
	}

	if(free_block != NULL) { // free block found then remove it from the list
		remove_from_free(free_block);
	}
	 
	return free_block;
}

char * extend(size_t asize) {
	char * bp;
	size_t extendsize = MAX(asize, CHUNKSIZE);
	char* heap_end = mem_heap_hi();
	uintptr_t epilogue = GET(heap_end-(WSIZE-1));
	uintptr_t tag = UNPACK_TAGS(epilogue);
	if(tag & 0b10) {
		if ((bp = extend_heap(extendsize*(DSIZE/WSIZE))) == NULL)
			return NULL;
	}
	else {
		char* last_bp = heap_end-(2*WSIZE-1);
		uintptr_t last_ftr = GET(last_bp);
		size_t size = UNPACK_SIZE(last_ftr);
		char * last_hdr = (char*)(last_bp - size+ WSIZE);
		size_t needed_size = asize - size/DSIZE;
		if ((bp = extend_heap(needed_size*(DSIZE/WSIZE))) == NULL)
			return NULL;
		bp = last_hdr;
	}
	return bp;
}
 
 /**********************************************************
  * place
  * Mark the block as allocated
  **********************************************************/
 void place(void* bp, size_t asize)
 {
   /* Get the current block size and tags in one load */
   uintptr_t hdr = GET(bp);
   size_t bsize = UNPACK_SIZE(hdr)/DSIZE; 
   if(bsize >= (asize + 2)) { // check if the block size is larger than needed by more than 1 double word if so insert extra space back into free list
	 uintptr_t extra_size = bsize - asize; // in double words
	 


	 char* extra_block = bp + (asize*DSIZE);
	 // put header and footer for new unallocated block
	 PUT(extra_block, PACK(extra_size*DSIZE, 2)); // put header, indicate previous block is allocated
	 PUT(extra_block + (extra_size*DSIZE)-WSIZE, PACK(extra_size*DSIZE, 2)); // put footer
     extra_block = coalesce(extra_block, false, NULL, NULL, false);
	 insert_in_free(extra_block); // insert free block into linked list
	 bsize -= extra_size;
   }
   else{ // indicate to the next block it is allocated
	 char* next_bp = NEXT_BLK(bp);
	 uintptr_t header_of_next_blk = GET(next_bp);
	 header_of_next_blk |= 0b10; // set allocated bit
	 PUT(next_bp, header_of_next_blk);
   }
   uintptr_t tags = UNPACK_TAGS(hdr);
   tags |= 0b1; // indicated it is allocated
   PUT(HDRP(bp), PACK(bsize*DSIZE, tags));
   available_dw -= bsize;
 }
 
 /**********************************************************
  * mm_free
  * Free the block and coalesce with neighbouring blocks
  **********************************************************/
 void mm_free(void *bp)
 {
	 if(bp == NULL){
	   return;
	 }
	 char * actual_bp = bp - WSIZE;
	 uintptr_t header = GET(actual_bp);
     size_t size = UNPACK_SIZE(header);
     size_t tag = UNPACK_TAGS(header);
	 tag &= ~0b1; // clear allocated tag
	 PUT(actual_bp, PACK(size,tag));
	 PUT(actual_bp+size-WSIZE, PACK(size,tag));
     char* new_bp = coalesce(actual_bp, false, NULL, NULL, false);
	 insert_in_free(new_bp);
	 available_dw += size/DSIZE;
 }
 
 
 /**********************************************************
  * mm_malloc
  * Allocate a block of size bytes.
  * The type of search is determined by find_fit
  * The decision of splitting the block, or not is determined
  *   in place(..)
  * If no block satisfies the request, the heap is extended
  **********************************************************/
 void *mm_malloc(size_t size)
 {
	size_t asize; /* adjusted block size */
	size_t extendsize; /* amount to extend heap if no fit */
	char * bp;

	/* Ignore spurious requests */
	if (size == 0)
		return NULL;

	/* Adjust block size to include overhead and alignment reqs. also convert to double word units */
	asize = adjusted_size(size);

	if(asize > available_dw) { // wont fit
		bp = extend(asize);
		if(bp == NULL)
			return NULL;
	}
	else { // try to find a fit
		bp = find_fit(asize);
		if(bp == NULL) { // cant fit probably due to fragmentation
			bp = extend(asize);
			if(bp == NULL)
				return NULL;
		}	
	}

	/* Search the free list for a fit */
	place(bp, asize);
	return bp+WSIZE;
 }
 
 /**********************************************************
  * mm_realloc
  * Implemented simply in terms of mm_malloc and mm_free
  *********************************************************/
 void *mm_realloc(void *ptr, size_t size)
 {
	/* If size == 0 then this is just free, and we return NULL. */
	/* If oldptr is NULL, then this is just malloc. */
	if (ptr == NULL) {
		return (mm_malloc(size));
	}
	
	if(size == 0){
		mm_free(ptr);
		return NULL;
	}
	
	char* old_bp = ptr-WSIZE;
	size_t asize = adjusted_size(size);
	
	size_t old_head = GET(old_bp);
	size_t old_size = UNPACK_SIZE(old_head);
	size_t old_tag = UNPACK_TAGS(old_head);
 
	if(asize == old_size/DSIZE) {
		return ptr;
	}
	else if(asize > old_size/DSIZE) { // larger allocation
		size_t coalesced_size;
		size_t forward_size;
		size_t needed_size = asize - old_size/DSIZE;
		char* coalesced_bp = coalesce(old_bp, true, &coalesced_size, &forward_size, false);        
		
		// see if we can extend the tail
		char* next_bp = old_bp + old_size;
		uintptr_t next_hdr = GET(next_bp);
		size_t next_size = UNPACK_SIZE(next_hdr);
		size_t next_tag = UNPACK_TAGS(next_hdr);
		if(next_size == 0 || ((next_tag & 0b1) == 0 && GET_SIZE(next_bp+forward_size) == 0)) { 
			if(needed_size > forward_size/DSIZE) {
				size_t growth_amt = needed_size - forward_size/DSIZE;
				char* res_bp = extend_heap(growth_amt*2);
				if(res_bp != NULL){
					size_t new_size= old_size+needed_size*DSIZE;
					PUT(old_bp, PACK(new_size, old_tag));
					char* new_fp = old_bp+new_size-WSIZE;
					PUT(new_fp+WSIZE, PACK(0, 3));
					available_dw -= forward_size/DSIZE + growth_amt;
					return old_bp+WSIZE;
				}
			}
		}
		if(coalesced_size >= asize*DSIZE){ // the new coalesced block is large enough
			coalesce(old_bp, false, NULL, NULL, true);
			if(coalesced_bp < old_bp){
				memmove(coalesced_bp+WSIZE, ptr, old_size-WSIZE);
			}
			place(coalesced_bp, asize);
			available_dw += old_size/DSIZE;
			return coalesced_bp+WSIZE;
		}
	}
	else { // smaller allocation
		place(old_bp, asize);
		available_dw += (old_size/DSIZE);
		return ptr;
	}
	void *newptr = mm_malloc(size);
	if (newptr == NULL)
		return NULL;

	memcpy(newptr, ptr, old_size-WSIZE);
	mm_free(ptr);
	return newptr;
 }
 
/**********************************************************
* mm_check
* Check the consistency of the memory heap
* Return nonzero if the heap is consistant.
*********************************************************/
int mm_check(void){
	
}
 