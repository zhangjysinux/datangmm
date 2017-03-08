#include "SharedMemory.h"
#define VBUFFER_SIZE 1500000
uchar SV_buffer[VBUFFER_SIZE];
uchar SV4L2_buffer[VBUFFER_SIZE];
pthread_mutex_t m_lock = PTHREAD_MUTEX_INITIALIZER;
struct SharedUseData
{
    int width;//frame width
    int height;//frame height
    unsigned char buf[VBUFFER_SIZE]; //frame buf data
};
SharedMemory::SharedMemory()
{
    DeleteSharedMemory();
    DeleteV4l2SharedMemory();
}
SharedMemory::~SharedMemory()
{

}
bool SharedMemory::DeleteSharedMemory()
{

}

bool SharedMemory::GetImageFromV4l2SharedMemory(QImage &image)
{
    //pthread_mutex_lock(&m_lock);
    void *shm = NULL;
    struct SharedUseData *shared=NULL;
    int shmid;
    //创建共享内存
    shmid = shmget((key_t)8000, sizeof(struct SharedUseData), 0666|IPC_CREAT);
    if(shmid == -1)
    {
        //pthread_mutex_unlock(&m_lock);
        return false;
    }
    shm = shmat(shmid, 0, 0);
    if(shm == (void*)-1)
    {
        //pthread_mutex_unlock(&m_lock);
        return false;
    }
    //printf("\nMemory attached at %X\n", (int)shm);
    //设置共享内存
    shared = (struct SharedUseData*)shm;
    uchar *rgbBuffer = shared->buf;
    int width = shared->width;
    int height = shared->height;
    //printf("v4l2width:%d\n",width);
    //printf("height:%d\n",height);
    int k = 0;
    for(int x = width-1 ; x >=0 ;x--)
    {
        for(int y = 0 ; y < height;y++)
        {
            SV4L2_buffer[k*3]=rgbBuffer[(y*width+x)*3];
            SV4L2_buffer[k*3+1]=rgbBuffer[(y*width+x)*3+1];
            SV4L2_buffer[k*3+2]=rgbBuffer[(y*width+x)*3+2];
            k++;
        }
    }
    image = QImage(SV4L2_buffer, height, width, QImage::Format_RGB888);
    //QImage image = QImage(rgbBuffer, width, height,QImage::Format_RGB888);
    if(shmdt(shm) == -1)
    {
        //fprintf(stderr, "shmdt failed\n");
        //pthread_mutex_unlock(&m_lock);
        return false;
    }
    //pthread_mutex_unlock(&m_lock);
    return true;
}

bool SharedMemory::DeleteV4l2SharedMemory()
{

}

bool SharedMemory::GetImageFromSharedMemory(QImage &image)
{
    pthread_mutex_lock(&m_lock);
    void *shm = NULL;
    struct SharedUseData *shared=NULL;
    int shmid;
    //创建共享内存
    shmid = shmget((key_t)1234, sizeof(struct SharedUseData), 0666|IPC_CREAT);
    if(shmid == -1)
    {
        pthread_mutex_unlock(&m_lock);
        return false;
    }
    shm = shmat(shmid, 0, 0);
    if(shm == (void*)-1)
    {
        pthread_mutex_unlock(&m_lock);
        return false;
    }
    //printf("\nMemory attached at %X\n", (int)shm);
    //设置共享内存
    shared = (struct SharedUseData*)shm;
    uchar *rgbBuffer = shared->buf+58;
    int width = 0;
    int height = 0;
    memcpy(&width, shared->buf + 18, sizeof(width));
    memcpy(&height, shared->buf + 22, sizeof(height));
    if (width == 0 || height == 0)
    {
     pthread_mutex_unlock(&m_lock);
     return false;
    }

    //printf("width:%d\n",width);
    //printf("height:%d\n",height);
    int k = 0;
    for(int x = width-1 ; x >=0 ;x--)
    {
        for(int y = 0 ; y < height;y++)
        {
            SV_buffer[k*3]=rgbBuffer[(y*width+x)*3];
            SV_buffer[k*3+1]=rgbBuffer[(y*width+x)*3+1];
            SV_buffer[k*3+2]=rgbBuffer[(y*width+x)*3+2];
            k++;
        }
    }
    image = QImage(SV_buffer, height, width, QImage::Format_RGB888);
    //QImage image = QImage(rgbBuffer, width, height,QImage::Format_RGB888);
    if(shmdt(shm) == -1)
    {
        //fprintf(stderr, "shmdt failed\n");
        pthread_mutex_unlock(&m_lock);
        return false;
    }
    pthread_mutex_unlock(&m_lock);
    return true;
}
