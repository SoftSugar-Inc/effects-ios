/**
 * @file st_mobile_color_convert.h

 * \~chinese 基于GPU（OpenGL）的YUV/RGB颜色转换接口， 支持rgba图像和nv21、nv12以及yuv图像之间的转换 \~english GPU-based (OpenGL) YUV/RGB color conversion interface, supporting conversions between rgba images and nv21, nv12, as well as yuv images.
 *@attention
 * \~chinese 本头文件中的所有接口都需要在GL Context中调用， API不保证线程安全.多线程调用时,需要确保安全调用 \~english All interfaces in this header file must be invoked within a GL Context. The API does not guarantee thread safety. When invoking from multiple threads, ensure safe calls.

 * \~chinese 一般调用步骤：创建句柄->准备输入、输出资源->调用转换接口->销毁句柄 \~english General calling process: Create handle -> Prepare input/output resources -> Call conversion interface -> Destroy handle

 * st_handle_t handle = NULL;

 * st_mobile_color_convert_create(&handle);

 * st_mobile_nv12_buffer_to_rgba_tex(handle, 1280, 720, ST_CLOCKWISE_ROTATE_0, false, p_buffer, tex_id);

 * st_mobile_color_convert_destroy(handle);

 * handle = NULL;

 * */


#ifndef INCLUDE_STMOBILE_ST_MOBILE_COLOR_CONVERT_H_
#define INCLUDE_STMOBILE_ST_MOBILE_COLOR_CONVERT_H_
#include "st_mobile_common.h"

/// \~chinese
/// @brief 创建颜色转换对应的句柄
/// @param[out] p_handle 句柄指针，存储内部分配的转换句柄
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Creates a handle corresponding to color conversion
/// @param[out] p_handle Pointer to handle, storing internally allocated conversion handle
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_color_convert_create(st_handle_t* p_handle);

/// \~chinese
/// @brief 销毁颜色转换对应的句柄，需要在OpenGL Context中调用
/// @param[in] handle 颜色转换句柄
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Destroys the handle corresponding to color conversion, must be called within an OpenGL Context
/// @param[in] handle The color conversion handle
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_color_convert_destroy(st_handle_t handle);

/// \~chinese
/// @brief color convert句柄级别的配置项，影响整个句柄
/// \~english
/// @brief color convert handle configuration mode
typedef enum {
    COLOR_CONVERT_ASYNC_READ      ///< \~chinese 是否启用异步读取模式，默认为0， 0代表同步读取， 1代表异步读取
} st_color_convert_param_t;

/// \~chinese
/// @brief 设置句柄的参数
/// @param[in] handle 颜色转换句柄
/// @param[in] param 参数类型
/// @param[in] val 参数数值，具体范围参考参数类型说明
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Sets the parameters for the color convert handle
/// @param[in] handle Initialized color convert handle
/// @param[in] param Parameter type
/// @param[in] val Parameter value, specific range refer to parameter type description
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_color_convert_set_param(st_handle_t handle, st_color_convert_param_t param, float val);

/// \~chinese
/// @brief 设置color convert输入buffer/texture的尺寸，提前调用该接口可以提升后续color convert接口的时间。
/// @param[in] handle 颜色转换句柄
/// @param[in] width 待转换图像的宽度
/// @param[in] height 待转换图像的高度
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Sets the size of the input buffer/texture for color convert, calling this interface in advance can improve the time of subsequent color convert interface.
/// @param[in] handle The color conversion handle
/// @param[in] width The width of the image to be converted
/// @param[in] height The height of the image to be converted
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_color_convert_set_size(st_handle_t handle, int width, int height);

/// \~chinese
/// @brief 对输入的nv21格式的buffer转换成RGBA格式，并输出到texId对应的OpenGL纹理中，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] width 待转换图像的宽度
/// @param[in] height 待转换图像的高度
/// @param[in] orientation 图像朝向，根据传入图像旋转角度，将图像转正。如果旋转角度为90或270，tex_out的宽高需要与buffer的宽高对调。
/// @param[in] horizontal_mirror 是否需要水平镜像，true - 水平镜像，false - 垂直镜像
/// @param[in] p_buffer NV21格式的图像buffer，需要预先分配空间（字节数：width * height * 3 / 2)
/// @param[out] tex_out RGBA格式输出纹理，需要在调用层预先创建
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input NV21 format buffer into RGBA format and outputs it to the OpenGL texture corresponding to texId, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] width The width of the image to be converted
/// @param[in] height The height of the image to be converted
/// @param[in] orientation The image orientation, according to the input image rotation angle, the image is corrected. If the rotation angle is 90 or 270, the width and height of tex_out need to be swapped with the width and height of the buffer.
/// @param[in] horizontal_mirror Whether horizontal mirroring is required, true - horizontal mirror, false - vertical mirror
/// @param[in] p_buffer NV21 format image buffer, space needs to be allocated in advance (byte number: width * height * 3 / 2)
/// @param[out] tex_out Output texture in RGBA format, needs to be created in advance by the calling layer
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_nv21_buffer_to_rgba_tex(st_handle_t handle, int width, int height,
                                  st_rotate_type orientation, bool horizontal_mirror,
                                  const unsigned char *p_buffer, int tex_out);

/// \~chinese
/// @brief 对输入的nv12格式的buffer转换成RGBA格式，并输出到texId对应的OpenGL纹理中，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] width 待转换图像的宽度
/// @param[in] height 待转换图像的高度
/// @param[in] orientation 图像朝向，根据传入图像旋转角度，将图像转正。如果旋转角度为90或270，tex_out的宽高需要与buffer的宽高对调。
/// @param[in] horizontal_mirror 是否需要水平镜像，true - 水平镜像，false - 垂直镜像
/// @param[in] p_buffer NV12格式的图像buffer
/// @param[out] tex_out RGBA格式输出纹理，需要在调用层预先创建
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input NV12 format buffer into RGBA format and outputs it to the OpenGL texture corresponding to texId, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] width The width of the image to be converted
/// @param[in] height The height of the image to be converted
/// @param[in] orientation The image orientation, according to the input image rotation angle, the image is corrected. If the rotation angle is 90 or 270, the width and height of tex_out need to be swapped with the width and height of the buffer.
/// @param[in] horizontal_mirror Whether horizontal mirroring is required, true - horizontal mirror, false - vertical mirror
/// @param[in] p_buffer NV12 format image buffer
/// @param[out] tex_out Output texture in RGBA format, needs to be created in advance by the calling layer
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_nv12_buffer_to_rgba_tex(st_handle_t handle, int width, int height,
                                  st_rotate_type orientation, bool horizontal_mirror,
                                  const unsigned char *p_buffer, int tex_out);

/// @brief YUV数据的区间格式，FULL对应Y的区间为[0, 255]，LIMITED对应的区间为[16, 235]
typedef enum {
    ST_MOBILE_YUV_RANGE_FULL,
    ST_MOBILE_YUV_RANGE_LIMITED,
} st_mobile_yuv_range_t;

/// @brief YUV数据的标准，未指定YUV config的接口内部使用的格式为ST_MOBILE_YUV_STANDARD_ANDROID。
typedef enum {
    ST_MOBILE_YUV_STANDARD_BT601,
    ST_MOBILE_YUV_STANDARD_BT709,
    ST_MOBILE_YUV_STANDARD_ANDROID,
} st_mobile_yuv_standard_t;

/// @brief YUV数据的配置，具体见前面区间与标准
typedef struct {
    st_mobile_yuv_standard_t standard;
    st_mobile_yuv_range_t range;
} st_mobile_yuv_config_t;


/// \~chinese
/// @brief 对输入的yuv buffer转换成RGBA texture，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_yuv_img yuv格式的图像，目前支持NV21、NV12两种格式输入
/// @param[in] orientation 图像朝向，根据传入图像旋转角度，将图像转正。如果旋转角度为90或270，tex_out的宽高需要与buffer的宽高对调。
/// @param[in] hHorMirror 是否需要水平镜像，true - 水平镜像，false - 垂直镜像
/// @param[out] dst_tex RGBA格式输出纹理，需要在调用层预先创建
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input yuv buffer to RGBA texture, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] p_yuv_img image in YUV format, currently supports NV21 and NV12 formats
/// @param[in] orientation image orientation, corrects the image based on the rotation angle provided. If the rotation angle is 90 or 270 degrees, the width and height of the tex_out should be swapped with the buffer’s width and height
/// @param[in] hHorMirror whether horizontal mirroring is required; true for horizontal mirroring, false for no mirroring
/// @param[out] dst_tex output texture in RGBA format, which should be created in advance by the caller
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_convert_yuv_buffer_2_rgba_tex(st_handle_t handle, const st_multiplane_image_t *p_yuv_img,
                                        st_rotate_type orientation, bool hHorMirror,
                                        int dst_tex);
/// \~chinese
/// @brief 对输入的yuv buffer转换成RGBA texture，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_yuv_img yuv格式的图像，目前支持NV21、NV12两种格式输入
/// @param[in] p_yuv_config 输入yuv图像的格式配置
/// @param[in] orientation 图像朝向，根据传入图像旋转角度，将图像转正。如果旋转角度为90或270，tex_out的宽高需要与buffer的宽高对调。
/// @param[in] hHorMirror 是否需要水平镜像，true - 水平镜像，false - 垂直镜像
/// @param[out] dst_tex RGBA格式输出纹理，需要在调用层预先创建
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input yuv buffer to RGBA texture, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] p_yuv_img image in YUV format, currently supports NV21 and NV12 formats
/// @param[in] p_yuv_config configuration of the input YUV image format
/// @param[in] orientation image orientation, corrects the image based on the rotation angle provided. If the rotation angle is 90 or 270 degrees, the width and height of the tex_out should be swapped with the buffer’s width and height
/// @param[in] hHorMirror whether horizontal mirroring is required; true for horizontal mirroring, false for no mirroring
/// @param[out] dst_tex output texture in RGBA format, which should be created in advance by the caller
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_convert_yuv_buffer_2_rgba_tex_ext(st_handle_t handle, const st_multiplane_image_t *p_yuv_img,
                                            const st_mobile_yuv_config_t *p_yuv_config,
                                            st_rotate_type orientation, bool hHorMirror,
                                            int dst_tex);

/// \~chinese
/// @brief 对输入的yuva buffer转换成RGBA texture，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_yuv_img yuv格式的图像，目前支持NV21、NV12两种格式输入
/// @param[in] p_alpha_img alpha通道图像，目前仅支持gray8格式, 且宽高需要与yuv图像一致, 可为空
/// @param[in] p_yuv_config 输入yuv图像的格式配置
/// @param[in] orientation 图像朝向，根据传入图像旋转角度，将图像转正。如果旋转角度为90或270，tex_out的宽高需要与buffer的宽高对调。
/// @param[in] hHorMirror 是否需要水平镜像，true - 水平镜像，false - 垂直镜像
/// @param[out] dst_tex RGBA格式输出纹理，需要在调用层预先创建
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input yuv buffer to RGBA texture, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] p_yuv_img image in YUV format, currently supports NV21 and NV12 formats
/// @param[in] p_alpha_img alpha channel image, only support gray8 format, it's size must be match with the size of yuv img, it can be pass NULL
/// @param[in] p_yuv_config configuration of the input YUV image format
/// @param[in] orientation image orientation, corrects the image based on the rotation angle provided. If the rotation angle is 90 or 270 degrees, the width and height of the tex_out should be swapped with the buffer’s width and height
/// @param[in] hHorMirror whether horizontal mirroring is required; true for horizontal mirroring, false for no mirroring
/// @param[out] dst_tex output texture in RGBA format, which should be created in advance by the caller
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_convert_yuva_buffer_2_rgba_tex(st_handle_t handle,
                                         const st_multiplane_image_t *p_yuv_img, const st_image_t *p_alpha_img,
                                         const st_mobile_yuv_config_t *p_yuv_config,
                                         st_rotate_type orientation, bool hHorMirror,
                                         int dst_tex);

/// \~chinese
/// @brief 该接口将废弃，请调用st_mobile_rgba_tex_to_nv21_buffer接口,
//         对输入的RGBA格式的texture转换成nv21格式的texture，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_src_tex 输入纹理
/// @param[in] p_dst_tex 待转换的YUV纹理，纹理格式需要是RGBA, 纹理大小为原始纹理[w/4, h*3/2]
/// \~english
/// @brief This interface will be deprecated, please call the st_mobile_rgba_tex_to_nv21_buffer interface
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_src_tex 输入纹理
/// @param[in] p_dst_tex 待转换的YUV纹理，纹理格式需要是RGBA, 纹理大小为原始纹理[w/4, h*3/2]
ST_SDK_API st_result_t
st_mobile_rgba_tex_to_nv21_tex(st_handle_t handle, const st_mobile_texture_t *p_src_tex,
                               st_mobile_texture_t *p_dst_tex);

/// \~chinese
/// @brief 对输入的RGBA格式texture转换成nv21格式的buffer，并输出到pYUVBuffer中，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] tex_in 输入纹理
/// @param[in] width 待转换图像的宽度
/// @param[in] height 待转换图像的高度
/// @param[out] p_buffer NV21格式的图像buffer，需要预先分配空间（字节数：width * height * 3 / 2)
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input RGBA format texture into NV21 format buffer and outputs to pYUVBuffer, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] tex_in Input texture
/// @param[in] width The width of the image to be converted
/// @param[in] height The height of the image to be converted
/// @param[out] p_buffer NV21 format image buffer, space needs to be allocated in advance (byte number: width * height * 3 / 2)
ST_SDK_API st_result_t
st_mobile_rgba_tex_to_nv21_buffer(st_handle_t handle, int tex_in, int width, int height, unsigned char* p_buffer);

/// \~chinese
/// @brief 对输入的RGBA格式texture转换成nv12格式的buffer，并输出到pYUVBuffer中，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] tex_in 输入纹理
/// @param[in] width 待转换图像的宽度
/// @param[in] height 待转换图像的高度
/// @param[out] p_buffer NV12格式的图像buffer，需要预先分配空间（字节数：width * height * 3 / 2)
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input RGBA format texture into NV12 format buffer and outputs to pYUVBuffer, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] tex_in Input texture
/// @param[in] width The width of the image to be converted
/// @param[in] height The height of the image to be converted
/// @param[out] p_buffer NV12 format image buffer, space needs to be allocated in advance (byte number: width * height * 3 / 2)
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_rgba_tex_to_nv12_buffer(st_handle_t handle, int tex_in, int width, int height, unsigned char* p_buffer);

/// \~chinese
/// @brief 对输入的RGBA格式texture转换成GRAY8格式的buffer，并输出到p_image中，需要在OpenGL Context中调用
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] tex_in 输入纹理
/// @param[out] p_image 输出图像buffer
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @attention 需要在OpenGL Context中调用
/// \~english
/// @brief Converts the input RGBA format texture into GRAY8 format buffer and outputs to p_image, must be called within an OpenGL Context
/// @param[in] handle Initialized color format conversion handle
/// @param[in] src_tex Input texture
/// @param[out] p_image Output image buffer, ensure that plane[0] is valid
/// @return Returns ST_OK on success, otherwise returns an error code, error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc.
/// @attention Must be called in an OpenGL Context
ST_SDK_API st_result_t
st_mobile_convert_rgba_tex_2_gray8_buffer(st_handle_t handle, int tex_in, st_multiplane_image_t *p_image);

/// @brief 对输入的RGBA格式texture转换成yuv格式的buffer，需要在OpenGL Context中调用
///        注意：需要在OpenGL Context中调用！！！
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] tex_in 输入纹理
/// @param[in] p_yuv_img 输出yuv图像
/// @param[in] p_yuv_config 输出yuv图像的格式配置
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_convert_rgba_tex_2_yuv_buffer(st_handle_t handle, int tex_in, st_multiplane_image_t *p_yuv_img,
                                        const st_mobile_yuv_config_t *p_yuv_config);

/// @brief 对输入的RGBA格式texture转换成yuv格式的buffer，需要在OpenGL Context中调用
///        注意：需要在OpenGL Context中调用！！！
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] tex_in 输入纹理
/// @param[in] p_yuv_img 输出yuv图像
/// @param[in] p_alpha_img 输出的原图alpha通道图像
/// @param[in] p_yuv_config 输出yuv图像的格式配置
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_convert_rgba_tex_2_yuva_buffer(st_handle_t handle, int tex_in,
                                         st_multiplane_image_t *p_yuv_img, st_image_t *p_alpha_img,
                                         const st_mobile_yuv_config_t *p_yuv_config);

/// @brief 对输入的RGBA格式texture转换成yuv格式的buffer，需要在OpenGL Context中调用
///        注意：需要在OpenGL Context中调用！！！
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_yuv_img 输出yuv图像
/// @param[in] p_yuv_config 输出yuv图像的格式配置
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_color_convert_end(st_handle_t handle, st_multiplane_image_t *p_yuv_img, st_image_t *p_alha_img);

/// @brief 对输入的BGRA/BGR格式buffer转换成rgba格式的tex
///        注意：需要在OpenGL Context中调用！！！
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_in_img 输入图像, 目前支持 BGR/BGRA两种
/// @param[out] dst_tex RGBA格式输出纹理，需要在调用层预先创建
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_convert_image_to_rgba_tex(st_handle_t handle, const st_image_t *p_in_img, int dst_tex);

/// @brief 对输入的BGRA/BGR格式buffer转换成rgba格式的tex
///        注意：需要在OpenGL Context中调用！！！
/// @param[in] handle 已初始化的颜色格式转换句柄
/// @param[in] p_in_img 输入图像, 目前支持 BGR/BGRA两种
/// @param[in] orientation image orientation, corrects the image based on the rotation angle provided. If the rotation angle is 90 or 270 degrees, the width and height of the tex_out should be swapped with the buffer’s width and height
/// @param[in] hHorMirror whether horizontal mirroring is required; true for horizontal mirroring, false for no mirroring
/// @param[out] dst_tex RGBA格式输出纹理，需要在调用层预先创建
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_convert_image_to_rgba_tex_ex(st_handle_t handle, const st_image_t *p_in_img,
                                       st_rotate_type orientation, bool hHorMirror,
                                       int dst_tex);

#endif //INCLUDE_STMOBILE_ST_MOBILE_COLOR_CONVERT_H_
