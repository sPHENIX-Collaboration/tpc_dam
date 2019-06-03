#include <linux/ioctl.h>

#define TRIGGER_DMA_TRANSFER _IO('p', 1)
#define DMA_STATUS           _IOR('p', 2, pldrv_io_t *)
#define SET_DMA_CONTROL      _IOW('p', 3, pldrv_io_t *)
#define DEBUG                _IOW('p', 4, pldrv_io_t *)
#define SET_BURST_LENGTH     _IOW('p', 5, pldrv_io_t *)
#define SET_BUFF_LENGTH      _IOW('p', 6, pldrv_io_t *)
#define SET_RATE             _IOW('p', 7, pldrv_io_t *)
#define READ_REG             _IOWR('p', 8, pldrv_io_t *)
#define WRITE_REG            _IOW('p', 9, pldrv_io_t *)
#define FEE_READ_REG         _IOWR('p', 10, feereg_io_t *)
#define FEE_WRITE_REG        _IOW('p', 11, feereg_io_t *)
#define READ_BURST_DMA       _IOR('p', 12, uint32_t **)
#define READ_CONT_DMA        _IOR('p', 13, uint32_t **)
#define WAIT_FOR_DMA         _IOR('p', 14, uint32_t *)
#define ENABLE_IRQ           _IO('p', 15)
#define DISABLE_IRQ          _IO('p', 16)
#define ENABLE_BURST         _IO('p', 17)
#define RESET_DMA            _IO('p', 18)

typedef struct {
    uint32_t address;
    uint64_t data;
} pldrv_io_t;

typedef struct {
    uint32_t id;
    uint32_t address;
    uint64_t data;
} feereg_io_t;
