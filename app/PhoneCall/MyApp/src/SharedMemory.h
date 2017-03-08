#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_
#include <QImage>
#include <sys/shm.h>
class SharedMemory
{
public:
    SharedMemory();
    ~SharedMemory();
    bool GetImageFromSharedMemory(QImage &image);
    bool DeleteSharedMemory();

    bool GetImageFromV4l2SharedMemory(QImage &image);
    bool DeleteV4l2SharedMemory();
};
#endif
