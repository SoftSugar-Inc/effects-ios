/**
 * @file st_mobile_effect.h
 *
 * @brief
 * \~chinese
 * 该接口主要提供了一些gl操作的封装接口，方便调用方可以创建长下文以及读取纹理内容等功能，相关接口定义在st_mobile_glutil.h文件中。
 * */
#ifndef ST_MOBILE_GLUTIL_H
#define ST_MOBILE_GLUTIL_H

#include "st_mobile_common.h"

/// \~chinese
/// @brief 创建句柄
/// @param[out] p_handle 句柄指针
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Create an handle
/// @param[out] p_handle Pointer to handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_create(st_handle_t *p_handle);

/// \~chinese
/// @brief 销毁句柄
/// @param[in] handle 已初始化的句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Destroy handle
/// @param[in] handle Initialized handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_destroy(st_handle_t handle);

/// \~chinese
/// @brief 创建OpenGL上下文
/// @param[in] handle 已初始化的句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief create OpenGL Context
/// @param[in] handle Initialized handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_create_context(st_handle_t handle, void *window);

/// \~chinese
/// @brief 销毁OpenGL上下文
/// @param[in] handle 已初始化的句柄
/// @param[in] window 指定设备window, 可为NULL
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Destroy OpenGL context
/// @param[in] handle Initialized handle
/// @param[in] window Specifies the native window, May be NULL
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_destroy_context(st_handle_t handle);

/// \~chinese
/// @brief 激活OpenGL上下文
/// @param[in] handle 已初始化的句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief make the OpenGL context current
/// @param[in] handle Initialized handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_make_context_current(st_handle_t handle);

/// \~chinese
/// @brief 切换当前OpenGL上下文
/// @param[in] handle 已初始化的句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief make the OpenGL context empty
/// @param[in] handle Initialized handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_make_context_empty(st_handle_t handle);

/// \~chinese
/// @brief 交换缓冲区
/// @param[in] handle 已初始化的句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief swap buffers
/// @param[in] handle Initialized handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_swap_buffers(st_handle_t handle);

typedef enum {
    PIXEL_READ_PBO,    ///< \~chinese PBO模式 \~english PBO mode
    PIXEL_READ_SYNC,   ///< \~chinese 同步模式 \~engish sync mode
} st_pixel_read_mode_t;

/// \~chinese
/// @brief 创建一个Read Pixel对象
/// @param[in] handle 已初始化的句柄
/// @param[out] pid 该Read Pixel对象对应的内部id
/// @param[in] mode Read Pixel对象的模式，详见st_pixel_read_mode_t
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief create a Read Pixel object
/// @param[in] handle Initialized handle
/// @param[out] pid the internal id of the Read Pixel object
/// @param[in] mode the mode of Read Pixel object, see st_pixel_read_mode_t
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_create_read_pixel_id(st_handle_t handle, int *pid, st_pixel_read_mode_t mode);

/// \~chinese
/// @brief 基于传入的id，销毁对应的Read Pixel对象
/// @param[in] handle 已初始化的句柄
/// @param[in] pid 要销毁的Read Pixel对象的id
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief destroy the Read Pixel object with the id
/// @param[in] handle Initialized handle
/// @param[in] pid the id of the Read Pixel object about to be destroyed
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_destroy_read_pixel_id(st_handle_t handle, int pid);

/// \~chinese
/// @brief 从纹理中读取图像数据
/// @param[in] handle 已初始化的句柄
/// @param[in] pid 该Read Pixel对象绑定的id
/// @param[in] p_tex 需要读取的纹理
/// @param[out] p_img 从纹理中读取的内容，需要调用方申请内存空间
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// @note 如果是PBO模式.，第一帧返回的数据无意义，并且需要在整个渲染流程结束以后将tex置为NULL再调用一次获取最后一帧数据。
/// \~english
/// @brief read image data from the texture
/// @param[in] handle Initialized handle
/// @param[in] pid the id of the Read Pixel
/// @param[in] p_tex the tex to be read
/// @param[out] p_img the buffer of the image, allocated by caller
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
/// @note If it's in PBO mode, the data returned from the first frame is meaningless, and you need
//to set 'tex' to NULL after the entire rendering process is finished before calling again to
//retrieve the last frame data.
ST_SDK_API st_result_t
st_mobile_glutil_read_pixel(st_handle_t handle, int pid, const st_mobile_texture_t *p_tex, st_image_t *p_img);

/// \~chinese
/// @brief 基于传入的id，重置对应的Read Pixel对象
/// @param[in] handle 已初始化的句柄
/// @param[in] pid 要重置的Read Pixel对象的id
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief reset the Read Pixel object with the id
/// @param[in] handle Initialized handle
/// @param[in] pid the id of the Read Pixel object about to be reset
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_reset_read_pixel_id(st_handle_t handle, int pid);

/// ========================================= gl resource ================================

typedef enum {
    GL_OBJECT_FRAMEBUFFER,
    GL_OBJECT_TEXTURE,
    GL_OBJECT_BUFFER,
    GL_OBJECT_PROGRAM
} st_globject_type_t;

/// \~chinese
/// @brief 创建gl资源对象
/// @param[in] handle 已初始化的句柄
/// @param[in] type 资源类型，详看st_globject_type_t结构体
/// @param[out] name 返回的资源名
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief create gl object
/// @param[in] handle Initialized handle
/// @param[in] type the type of gl object, see st_globject_type_t
/// @param[out] name the name of gl resource
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_create_object(st_handle_t handle, st_globject_type_t type, int *name);

/// \~chinese
/// @brief 销毁gl资源对象
/// @param[in] handle 已初始化的句柄
/// @param[in] name 资源对象名
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief destroy gl object
/// @param[in] handle Initialized handle
/// @param[in] name the name of gl resource
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_destroy_object(st_handle_t handle, int name);

/// \~chinese
/// @brief 绑定gl资源对象
/// @param[in] handle 已初始化的句柄
/// @param[in] name 资源对象名
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief bind gl object
/// @param[in] handle Initialized handle
/// @param[in] name the name of gl resource
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_bind_object(st_handle_t handle, int name);

/// \~chinese
/// @brief 解除绑定gl资源对象
/// @param[in] handle 已初始化的句柄
/// @param[in] name 资源对象name
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief unbind gl object
/// @param[in] handle Initialized handle
/// @param[in] name the name of gl resource
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_unbind_object(st_handle_t handle, int name);

/// \~chinese
/// @brief 获取gl对象内部id
/// @param[in] handle 已初始化的句柄
/// @param[in] name 资源对象name
/// @param[out] object_id gl对象id
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief get the id of gl object
/// @param[in] handle Initialized handle
/// @param[in] name the name of gl resource
/// @param[out] object_id the id of gl object
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_get_object_id(st_handle_t handle, int name, int *object_id);

/// \~chinese
/// @brief 绑定纹理到帧缓冲区
/// @param[in] handle 已初始化的句柄
/// @param[in] name 资源对象name
/// @param[in] tex 纹理对象，详见st_mobile_texture_t
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief attach tex to framebuffer
/// @param[in] handle Initialized handle
/// @param[in] name the name of framebuffer resource
/// @param[in] tex texture object, see st_mobile_texture_t
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_framebuffer_attach_tex(st_handle_t handle, int name, const st_mobile_texture_t *tex);

/// \~chinese
/// @brief 使用指定颜色清除缓冲区
/// @param[in] handle 已初始化的句柄
/// @param[in] name 资源对象name
/// @param[in] color 颜色值，详见st_color_t
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief clear framebuffer
/// @param[in] handle Initialized handle
/// @param[in] name the name of framebuffer resource
/// @param[in] color color to clear framebuffer, see st_color_t
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_framebuffer_clear_color(st_handle_t handle, int name, const st_color_t *color);

/// \~chinese
/// @brief 更新纹理内容
/// @param[in] handle 已初始化的句柄
/// @param[in] name 纹理对象name
/// @param[in] img 纹理数据,详见st_image_t
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update texture
/// @param[in] handle Initialized handle
/// @param[in] name the name of texture resource
/// @param[in] img the image used to update texture, see st_image_t
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_update_tex(st_handle_t handle, int name, const st_image_t *img);

typedef enum
{
    BUFFER_TARGET_VERTEX,
    BUFFER_TARGET_ELEMENT_INDEX
} st_glbuffer_target_t;

typedef enum
{
    BUFFER_USAGE_STATIC,
    BUFFER_USAGE_DYNAMIC
} st_glbuffer_usage_t;

typedef struct
{
    st_glbuffer_target_t target;
    st_glbuffer_usage_t usage;
    int size;
    void *buffer;
} st_glbuffer_t;

/// \~chinese
/// @brief 更新缓冲区内容
/// @param[in] handle 已初始化的句柄
/// @param[in] name 缓冲区对象name
/// @param[in] buffer 缓冲区数据,详见st_glbuffer_t
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update buffer
/// @param[in] handle Initialized handle
/// @param[in] name the name of buffer resource
/// @param[in] buffer the data used to update gl buffer, see st_glbuffer_t
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_update_buffer(st_handle_t handle, int name, const st_glbuffer_t *buffer);

/// \~chinese
/// @brief 编译gl程序
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] vsh 顶点着色器
/// @param[in] fsh 片元着色器
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief compile gl program
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] vsh vertex shader
/// @param[in] fsh fragment shader
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_compile_program(st_handle_t handle, int name, const char *vsh, const char *fsh);

/// \~chinese
/// @brief 设置顶点数据
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] vertex_name 顶点属性名称
/// @param[in] size 顶点数据大小
/// @param[in] stride 顶点数据步长
/// @param[in] buffer 顶点数据或者偏移
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief set vertex attribute
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] vertex_name the name of vertex attribute
/// @param[in] size the size of vertex attribute
/// @param[in] stride the stride of vertex data
/// @param[in] buffer the data or offset of vertex data
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_vertex_attrib(st_handle_t handle, int name, const char *vertex_name, int size, int stride, const void *buffer);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform1f(st_handle_t handle, int name, const char *uniform_name, float val);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val1 uniform变量的值
/// @param[in] val2 uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val1 the value of uniform variable
/// @param[in] val2 the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform2f(st_handle_t handle, int name, const char *uniform_name, float val1, float val2);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val1 uniform变量的值
/// @param[in] val2 uniform变量的值
/// @param[in] val3 uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val1 the value of uniform variable
/// @param[in] val2 the value of uniform variable
/// @param[in] val3 the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform3f(st_handle_t handle, int name, const char *uniform_name, float val1, float val2, float val3);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val1 uniform变量的值
/// @param[in] val2 uniform变量的值
/// @param[in] val3 uniform变量的值
/// @param[in] val4 uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val1 the value of uniform variable
/// @param[in] val2 the value of uniform variable
/// @param[in] val3 the value of uniform variable
/// @param[in] val4 the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform4f(st_handle_t handle, int name, const char *uniform_name, float val1, float val2, float val3, float val4);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform1i(st_handle_t handle, int name, const char *uniform_name, int val);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val1 uniform变量的值
/// @param[in] val2 uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val1 the value of uniform variable
/// @param[in] val2 the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform2i(st_handle_t handle, int name, const char *uniform_name, int val1, int val2);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val1 uniform变量的值
/// @param[in] val2 uniform变量的值
/// @param[in] val3 uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val1 the value of uniform variable
/// @param[in] val2 the value of uniform variable
/// @param[in] val3 the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform3i(st_handle_t handle, int name, const char *uniform_name, int val1, int val2, int val3);

/// \~chinese
/// @brief 更新uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val1 uniform变量的值
/// @param[in] val2 uniform变量的值
/// @param[in] val3 uniform变量的值
/// @param[in] val4 uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val1 the value of uniform variable
/// @param[in] val2 the value of uniform variable
/// @param[in] val3 the value of uniform variable
/// @param[in] val4 the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform4i(st_handle_t handle, int name, const char *uniform_name, int val1, int val2, int val3, int val4);

/// \~chinese
/// @brief 更新uniform mat2
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable of mat2
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform_mat2(st_handle_t handle, int name, const char *uniform_name, const float *val);

/// \~chinese
/// @brief 更新uniform mat3
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable of mat3
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform_mat3(st_handle_t handle, int name, const char *uniform_name, const float *val);

/// \~chinese
/// @brief 更新uniform mat4
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] val uniform变量的值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable of mat4
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] val the value of uniform variable
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform_mat4(st_handle_t handle, int name, const char *uniform_name, const float *val);

/// \~chinese
/// @brief 更新纹理uniform
/// @param[in] handle 已初始化的句柄
/// @param[in] name 程序对象name
/// @param[in] uniform_name uniform变量名称
/// @param[in] tex_name 纹理资源名
/// @param[in] pos 纹理激活位置
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief update uniform variable of texture
/// @param[in] handle Initialized handle
/// @param[in] name the name of program resource
/// @param[in] uniform_name the name of uniform variable
/// @param[in] tex_name the name of texture resource
/// @param[in] pos the active position of texture
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_glutil_program_bind_uniform_tex(st_handle_t handle, int name, const char *uniform_name, int tex_name, uint32_t pos);

#endif //ST_MOBILE_GLUTIL_H
