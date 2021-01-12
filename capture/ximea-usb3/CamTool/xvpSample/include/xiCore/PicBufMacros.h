#ifndef PICBUFMACROS_H
#define PICBUFMACROS_H

//! Moves to the next row from the current position in the buffer
#define NEXT_ROW(type, buf, stride) ((type*)(((quint8*)(buf))+(stride)))
#define NEXT_ROW_U8(buf, stride) NEXT_ROW(quint8, buf, stride)
#define NEXT_ROW_I8(buf, stride) NEXT_ROW(qint8, buf, stride)
#define NEXT_ROW_U16(buf, stride) NEXT_ROW(quint16, buf, stride)
#define NEXT_ROW_I16(buf, stride) NEXT_ROW(qint16, buf, stride)
#define NEXT_ROW_U32(buf, stride) NEXT_ROW(quint32, buf, stride)
#define NEXT_ROW_I32(buf, stride) NEXT_ROW(qint32, buf, stride)
#define NEXT_ROW_F(buf, stride) NEXT_ROW(float, buf, stride)
#define NEXT_ROW_D(buf, stride) NEXT_ROW(double, buf, stride)

//! Moves to the previous row from the current position in the buffer
#define PREV_ROW(type, buf, stride) ((type*)(((quint8*)(buf))-(stride)))
#define PREV_ROW_U8(buf, stride) PREV_ROW(quint8, buf, stride)
#define PREV_ROW_I8(buf, stride) PREV_ROW(qint8, buf, stride)
#define PREV_ROW_U16(buf, stride) PREV_ROW(quint16, buf, stride)
#define PREV_ROW_I16(buf, stride) PREV_ROW(qint16, buf, stride)
#define PREV_ROW_U32(buf, stride) PREV_ROW(quint32, buf, stride)
#define PREV_ROW_I32(buf, stride) PREV_ROW(qint32, buf, stride)
#define PREV_ROW_F(buf, stride) PREV_ROW(float, buf, stride)
#define PREV_ROW_D(buf, stride) PREV_ROW(double, buf, stride)

//! Moves to the next pixel from the current position in the buffer
#define NEXT_PIXEL(type, img, buf) (((type*)(buf))+(img).m_uiChannels)
#define NEXT_PIXEL_U8(img, buf) NEXT_PIXEL(quint8, img, buf)
#define NEXT_PIXEL_I8(img, buf) NEXT_PIXEL(qint8, img, buf)
#define NEXT_PIXEL_U16(img, buf) NEXT_PIXEL(quint16, img, buf)
#define NEXT_PIXEL_I16(img, buf) NEXT_PIXEL(qint16, img, buf)
#define NEXT_PIXEL_U32(img, buf) NEXT_PIXEL(quint32, img, buf)
#define NEXT_PIXEL_I32(img, buf) NEXT_PIXEL(qint32, img, buf)
#define NEXT_PIXEL_F(img, buf) NEXT_PIXEL(float, img, buf)
#define NEXT_PIXEL_D(img, buf) NEXT_PIXEL(double, img, buf)


//! Moves to the given pixel position in the buffer
#define PIXEL(type, img, col, row)\
    ((type*)(((quint8*)(img).m_pData)+quint32(row)*(img).m_uiStride+(quint32(col)*((img).m_uiChannels)*sizeof(type))))
#define PIXEL_U8(img, col, row) PIXEL(quint8, img, col, row)
#define PIXEL_I8(img, col, row) PIXEL(qint8, img, col, row)
#define PIXEL_U16(img, col, row) PIXEL(quint16, img, col, row)
#define PIXEL_I16(img, col, row) PIXEL(qint16, img, col, row)
#define PIXEL_U32(img, col, row) PIXEL(quint32, img, col, row)
#define PIXEL_I32(img, col, row) PIXEL(qint32, img, col, row)
#define PIXEL_F(img, col, row) PIXEL(float, img, col, row)
#define PIXEL_D(img, col, row) PIXEL(double, img, col, row)

//! Moves to the given channel from current pixel
#define PIX_CHANNEL(type, buf, channel) (((type*)buf)+(channel))
#define PIX_CHANNEL_U8(buf, channel) PIX_CHANNEL(quint8, buf, channel)
#define PIX_CHANNEL_CU8(buf, channel) PIX_CHANNEL(const quint8, buf, channel)
#define PIX_CHANNEL_I8(buf, channel) PIX_CHANNEL(qint8, buf, channel)
#define PIX_CHANNEL_CI8(buf, channel) PIX_CHANNEL(const qint8, buf, channel)
#define PIX_CHANNEL_U16(buf, channel) PIX_CHANNEL(quint16, buf, channel)
#define PIX_CHANNEL_CU16(buf, channel) PIX_CHANNEL(const quint16, buf, channel)
#define PIX_CHANNEL_I16(buf, channel) PIX_CHANNEL(qint16, buf, channel)
#define PIX_CHANNEL_CI16(buf, channel) PIX_CHANNEL(const qint16, buf, channel)
#define PIX_CHANNEL_U32(buf, channel) PIX_CHANNEL(quint32, buf, channel)
#define PIX_CHANNEL_CU32(buf, channel) PIX_CHANNEL(const quint32, buf, channel)
#define PIX_CHANNEL_I32(buf, channel) PIX_CHANNEL(qint32, buf, channel)
#define PIX_CHANNEL_CI32(buf, channel) PIX_CHANNEL(const qint32, buf, channel)
#define PIX_CHANNEL_F(buf, channel) PIX_CHANNEL(float, buf, channel)
#define PIX_CHANNEL_CF(buf, channel) PIX_CHANNEL(const float, buf, channel)
#define PIX_CHANNEL_D(buf, channel) PIX_CHANNEL(double, buf, channel)
#define PIX_CHANNEL_CD(buf, channel) PIX_CHANNEL(const double, buf, channel)

//! Moves to the given row of given channel in the planar image
#define PLANAR_CHANNEL_ROW(type, pic, row, channel)\
    ((type*)(((quint8*)(pic).m_pData)+(quint32(row)*(pic).m_uiStride)+(quint32(channel)*(pic).m_uiHeight*(pic).m_uiStride)))
#define PLANAR_CHANNEL_ROW_U8(pic, row, channel) PLANAR_CHANNEL_ROW(quint8, pic, row, channel)
#define PLANAR_CHANNEL_ROW_I8(pic, row, channel) PLANAR_CHANNEL_ROW(qint8, pic, row, channel)
#define PLANAR_CHANNEL_ROW_U16(pic, row, channel) PLANAR_CHANNEL_ROW(quint16, pic, row, channel)
#define PLANAR_CHANNEL_ROW_I16(pic, row, channel) PLANAR_CHANNEL_ROW(qint16, pic, row, channel)
#define PLANAR_CHANNEL_ROW_U32(pic, row, channel) PLANAR_CHANNEL_ROW(quint32, pic, row, channel)
#define PLANAR_CHANNEL_ROW_I32(pic, row, channel) PLANAR_CHANNEL_ROW(qint32, pic, row, channel)
#define PLANAR_CHANNEL_ROW_F(pic, row, channel) PLANAR_CHANNEL_ROW(float, pic, row, channel)
#define PLANAR_CHANNEL_ROW_D(pic, row, channel) PLANAR_CHANNEL_ROW(double, pic, row, channel)

//! Gets pointer to the row in the image
#define ROW(type, img, row) (type*)(((quint8*)(img).m_pData)+(quint32(row)*(img).m_uiStride))
#define ROW_U8(img, row) ROW(quint8, img, row)
#define ROW_I8(img, row) ROW(qint8, img, row)
#define ROW_U16(img, row) ROW(quint16, img, row)
#define ROW_I16(img, row) ROW(qint16, img, row)
#define ROW_U32(img, row) ROW(quint32, img, row)
#define ROW_I32(img, row) ROW(qint32, img, row)
#define ROW_F(img, row) ROW(float, img, row)
#define ROW_D(img, row) ROW(double, img, row)


#endif // PICBUFMACROS_H
