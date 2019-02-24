#ifndef __STM32F2xx_H
#define __STM32F2xx_H
typedef struct
{
  uint32_t CR;     /*!< DMA stream x configuration register      */
  uint32_t NDTR;   /*!< DMA stream x number of data register     */
  uint32_t PAR;    /*!< DMA stream x peripheral address register */
  uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;

extern DMA_Stream_TypeDef dma1_stream2, dma1_stream5, dma1_stream7, dma2_stream1, dma2_stream2, dma2_stream5, dma2_stream6, dma2_stream7;

#undef DMA1_Stream2
#undef DMA1_Stream5
#undef DMA1_Stream7
#undef DMA2_Stream1
#undef DMA2_Stream2
#undef DMA2_Stream5
#undef DMA2_Stream6
#undef DMA2_Stream7
#define DMA1_Stream2 (&dma1_stream2)
#define DMA1_Stream5 (&dma1_stream5)
#define DMA1_Stream7 (&dma1_stream7)
#define DMA2_Stream1 (&dma2_stream1)
#define DMA2_Stream2 (&dma2_stream2)
#define DMA2_Stream5 (&dma2_stream5)
#define DMA2_Stream6 (&dma2_stream6)
#define DMA2_Stream7 (&dma2_stream7)

inline void DMA_DeInit(DMA_Stream_TypeDef* DMAy_Streamx) { }
inline void DMA_Init(DMA_Stream_TypeDef* DMAy_Streamx, DMA_InitTypeDef* DMA_InitStruct) { }
inline void DMA_ITConfig(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT, FunctionalState NewState) { }
inline void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct) { }
inline void DMA_Cmd(DMA_Stream_TypeDef* DMAy_Streamx, FunctionalState NewState) { }
inline FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG) { return RESET; }
inline ITStatus DMA_GetITStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT) { return RESET; }
inline void DMA_ClearITPendingBit(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_IT) { }

#endif
