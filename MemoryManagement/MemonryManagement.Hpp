#ifndef _MEMONRYMANAGEMENT
#define _MEMONRYMANAGEMENT
#include <stdio.h>
#include <malloc.h>
#define MEMONRYSIZE 20
namespace My{
    struct blockHeader{
        blockHeader *next;
    };
    struct pageHeader{
        pageHeader *next;
        blockHeader* blocks(){
            return reinterpret_cast<blockHeader *>(this+1);
        }
    };
    class Allocator{
        public :
            //debug 信息
            static const unsigned char PATTERN_ALIGN=0xFC;
            static const unsigned char PATTERN_ALLOC=0xFD;
            static const unsigned char PATTERN_FREE=0xFE;
            //构造函数
            Allocator(
                unsigned dataSize,
                unsigned pageSize,
                unsigned alignment

            ):mPageList(nullptr),mFreeList(nullptr){reset(dataSize,pageSize,alignment);};
            //构析函数
            ~Allocator(void){freeAll();};
            //重新设置分配器的大小
            void reset(
                unsigned dataSize,
                unsigned pageSize,
                unsigned alignment
            );
            //申请一个块内存
            void* allocate(void);
            void* allocate(unsigned size);
            
            //释放一个块内存
            void free(void *p);
            void free(void *p,unsigned size);

            //把所有内存返回给操作系统；
            void freeAll(void);
        private:
#if defined(_DEBUG)
            void fillFreePage(pageHeader *p);
            void fillFreeBlock(blockHeader *p);
            void fillAllocateBlock(blockHeader *p);
#endif
            blockHeader *nextBLock(blockHeader *p);

            //存储页面链表
            pageHeader *mPageList;
            //存储空闲内存的链表
            blockHeader *mFreeList;
            //数据大小设置
            unsigned mDataSize;
            unsigned mPageSize;
            unsigned mAlignmentSize;
            unsigned mBlockSize;
            unsigned mBlocksPerSize;
            //数据数量设置
            unsigned mNumPage;
            unsigned mNumBlocks;
            unsigned mNumFreeBlocks;

            //浅拷贝
            Allocator(const Allocator &clone);
            //深拷贝
            Allocator &operator=(const Allocator &rhs);


    };
    class memoryManager{
        public:
        template<typename T,typename... Args>
        T* New(Args... par){
            return new(allocator(sizeof(T))) T(Args);
        }
        template<typename T>
        void Delete(T *p){
            reinterpret_cast<T *>(p)->~T();
            free(p,sizeof(T));
        }
        virtual ~memoryManager()=0;
        virtual int initialize()=0;
        virtual void finalize()=0;
        virtual void Tick()=0;
        void* allocator(size_t size);
        void  free(void *p,size_t size);
    private:
        static size_t* mPBlockSizeLookUp;
        static My::Allocator* mPallocators;
        static My::Allocator* LookUpAllocator(size_t size);
    };
};
#endif