#include "MemonryManagement.Hpp"
#include <cstring>
#include <assert.h>
using namespace My;
//数据大小  页面大小    对齐数据
void Allocator::reset(unsigned dataSize,unsigned pageSize,unsigned alignment){
    assert(alignment>1);
    freeAll();
    unsigned maxHeaderData=sizeof(blockHeader)>dataSize ? sizeof(blockHeader):dataSize;
    mAlignmentSize=(maxHeaderData%alignment)?(alignment-maxHeaderData%alignment):0;
    mBlockSize=maxHeaderData+mAlignmentSize;
    mBlocksPerSize=(mPageSize-sizeof(pageHeader))/mBlockSize;

}
void * Allocator::allocate(void){
    if(!mFreeList){
        pageHeader *newPage=reinterpret_cast<pageHeader *>(new char[mPageSize]);
#if defined(_DEBUG)        
        fillFreePage(newPage);
#endif
        if(mPageList){
            newPage->next=mPageList;
        }
        mPageList=newPage;
        blockHeader *currBlock=newPage->blocks();
        for(unsigned i=0;i<mBlocksPerSize-1;++i){
            currBlock->next=nextBLock(currBlock);
            currBlock=nextBLock(currBlock);
        }
        currBlock->next=nullptr;
        mFreeList=newPage->blocks();

    }
    blockHeader *freeBlock=mFreeList;
    mFreeList=mFreeList->next;
    --mNumBlocks;
#if defined(_DEBUG)
    fillAllocateBlock(freeBlock);
#endif
    return freeBlock;
}
void Allocator::free(void *p){
    blockHeader *block=reinterpret_cast<blockHeader *>(p);
#if defined(_DEBUG)
        fillFreeBlock(block);
#endif
    block->next=mFreeList;
    mFreeList=block;
    ++mNumFreeBlocks;
}
void Allocator::freeAll(void){
    pageHeader *pageWalker=mPageList;
    while(mPageList){
        pageHeader *temp=pageWalker;
        pageWalker=pageWalker->next;
        delete[] reinterpret_cast<char *>(temp);
    }
    mPageList=nullptr;
    mFreeList=nullptr;
    mNumBlocks=0;
    mNumPage=0;
    mNumFreeBlocks=0;
}
#if defined(_DEBUG)
void Allocator::fillFreePage(pageHeader *p){
    p->next=nullptr;
    blockHeader *temp=p->blocks();
    for(unsigned i=0;i<mBlocksPerSize;++i){
#if defined(_DEBUG)
        fillFreeBlock(temp);
#endif
        temp=nextBLock(temp);
    }
}
#endif
#if defined(_DEBUG)
void Allocator::fillFreeBlock(blockHeader *p){
    std::memset(p,PATTERN_FREE,mBlockSize-mAlignmentSize);
    std::memset(reinterpret_cast<char *>(p)+mBlockSize-mAlignmentSize,PATTERN_ALIGN,mAlignmentSize);

}
#endif
blockHeader* Allocator::nextBLock (blockHeader *p){
    return reinterpret_cast<blockHeader *>(reinterpret_cast<char *>(p)+mBlockSize);
}

