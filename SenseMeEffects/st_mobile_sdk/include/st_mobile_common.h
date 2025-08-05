/**
*@file st_mobile_common.h

*\~chinese
* 提供了返回值错误码、图像常用格式、创建检测模式等通用枚举类型,
* 提供人脸结构体的定义
* 同时提供了图像通用处理、检测阈值设置、log输出等API函数的声明

*\~english
* Provides common enumeration types for return error codes, common image formats, and creation of detection modes.
* Defines structures for human faces.
* Also provides function declarations for general image processing, detection threshold settings, log output, and more.
**/
#ifndef INCLUDE_STMOBILE_ST_MOBILE_COMMON_H_
#define INCLUDE_STMOBILE_ST_MOBILE_COMMON_H_

#include <stdint.h>

#ifdef _MSC_VER
#	ifdef SDK_EXPORTS
#		define ST_SDK_API_ __declspec(dllexport)
#	else
#		define ST_SDK_API_
#	endif
#else /* _MSC_VER */
#	ifdef SDK_EXPORTS
#		define ST_SDK_API_ __attribute__((visibility ("default")))
#	else
#		define ST_SDK_API_
#	endif
#endif

#ifdef __cplusplus
#	define ST_SDK_API extern "C" ST_SDK_API_
#else
#	define ST_SDK_API ST_SDK_API_
#endif

/// \~chinese 句柄类型声明,本sdk以句柄来管理不同功能,每个句柄内函数多线程并不安全,单个句柄不建议顺序安全调用,注意保护每个句柄的安全和生命周期；句柄之间独立,大部分功能支持多个句柄并行处理
/// \~english Handle type declaration. This SDK manages different functionalities using handles. The functions within each handle are not thread-safe. It is important to ensure the safety and lifecycle of each handle. The handles are independent of each other, and most functionalities support parallel processing with multiple handles
typedef void *st_handle_t;

/// \~chinese 结果声明,常用于表示运行结果,ST_OK正常, 不正常则返回相应的错误码 \~english Result declaration, commonly used to represent the outcome of an operation. ST_OK indicates successful execution, while an error code is returned to indicate any abnormalities or failures
typedef int   st_result_t;

#define ST_OK                               0   ///< \~chinese 正常运行 \~english Successful execution

#define ST_E_INVALIDARG                     -1  ///< \~chinese 无效参数 \~english Invalid argument
#define ST_E_HANDLE                         -2  ///< \~chinese 句柄错误 \~english Handle error
#define ST_E_OUTOFMEMORY                    -3  ///< \~chinese 内存不足 \~english Out of memory
#define ST_E_FAIL                           -4  ///< \~chinese 内部错误 \~english Internal error
#define ST_E_DELNOTFOUND                    -5  ///< \~chinese 定义缺失 \~english Definition missing
#define ST_E_INVALID_PIXEL_FORMAT           -6  ///< \~chinese 不支持的图像格式 \~english Unsupported image format
#define ST_E_FILE_NOT_FOUND                 -7  ///< \~chinese 文件不存在 \~english File not found
#define ST_E_INVALID_FILE_FORMAT            -8  ///< \~chinese 文件格式不正确导致加载失败 \~english Invalid file format causing loading failure
#define ST_E_FILE_EXPIRE                    -9  ///< \~chinese 文件过期 \~english File expired

#define ST_E_INVALID_AUTH                   -13 ///< \~chinese license不合法 \~english  Invalid license
#define ST_E_INVALID_APPID                  -14 ///< \~chinese 包名错误 \~english Incorrect package name
#define ST_E_AUTH_EXPIRE                    -15 ///< \~chinese license过期 \~english License expired
#define ST_E_UUID_MISMATCH                  -16 ///< \~chinese UUID不匹配 \~english UUID mismatch
#define ST_E_ONLINE_AUTH_CONNECT_FAIL       -17 ///< \~chinese 在线验证连接失败 \~english UUID  Online verification connection failed
#define ST_E_ONLINE_AUTH_TIMEOUT            -18 ///< \~chinese 在线验证超时 \~english Online verification timeout
#define ST_E_ONLINE_AUTH_INVALID            -19 ///< \~chinese 在线签发服务器端返回错误 \~english Error returned by online signing server
#define ST_E_LICENSE_IS_NOT_ACTIVABLE       -20 ///< \~chinese license不可激活 \~english License cannot be activated
#define ST_E_ACTIVE_FAIL                    -21 ///< \~chinese license激活失败 \~english License activation failed
#define ST_E_ACTIVE_CODE_INVALID            -22 ///< \~chinese 激活码无效 \~english Invalid activation code

#define ST_E_NO_CAPABILITY                  -23 ///< \~chinese license文件没有提供这个能力 \~english The capability of this license is not provided
#define ST_E_PLATFORM_NOTSUPPORTED          -24 ///< \~chinese license不支持这个平台 \~english This platform is not supported by the license
#define ST_E_SUBMODEL_NOT_EXIST             -26 ///< \~chinese 子模型不存在 \~english Submodel does not exist
#define ST_E_ONLINE_ACTIVATE_NO_NEED        -27 ///< \~chinese 不需要在线激活 \~english Online activation not required
#define ST_E_ONLINE_ACTIVATE_FAIL           -28 ///< \~chinese 在线激活失败 \~english Online activation failed
#define ST_E_ONLINE_ACTIVATE_CODE_INVALID   -29 ///< \~chinese 在线激活码无效 \~english Invalid online activation code
#define ST_E_ONLINE_ACTIVATE_CONNECT_FAIL   -30 ///< \~chinese 在线激活连接失败 \~english Online activation connection failed

#define ST_E_MODEL_NOT_IN_MEMORY            -31 ///< \~chinese 模型不在内存中 \~english Model is not in memory
#define ST_E_UNSUPPORTED_ZIP                -32 ///< \~chinese 当前sdk不支持的素材包 \~english Unsupported asset package for the current SDK
#define ST_E_PACKAGE_EXIST_IN_MEMORY        -33 ///< \~chinese 素材包已存在在内存中,不重复加载,或相同动画正在播放,不重复播放 \~english Asset package already exists in memory, no duplicate loading, or the same animation is already playing, no duplicate playback

#define ST_E_NOT_CONNECT_TO_NETWORK         -34 ///< \~chinese 设备没有联网 \~english Device not connected to the network
#define ST_E_OTHER_LINK_ERRORS_IN_HTTPS     -35 ///< \~chinese https中的其他链接错误 \~english  Other link errors in HTTPS
#define ST_E_CERTIFICAT_NOT_BE_TRUSTED      -36 ///< \~chinese windows系统有病毒或被黑导致证书不被信任 \~english The certificate is not trusted due to virus or hacking on the Windows system

#define ST_E_LICENSE_LIMIT_EXCEEDED         -37 ///< \~chinese license激活次数已用完 \~english License activation limit exceeded

#define ST_E_NOFACE                     	-38 ///< \~chinese 没有检测到人脸 \~english No face detected

#define ST_E_API_UNSUPPORTED                -39 ///< \~chinese 该API暂不支持 \~english The API is not supported
#define ST_E_API_DEPRECATED                 -40 ///< \~chinese 该API已标记为废弃,应替换其他API或停止使用 \~english The API has been deprecated and should be replaced with other APIs or discontinued
#define ST_E_ARG_UNSUPPORTED                -41 ///< \~chinese 该参数不支持 \~english The argument is not supported
#define ST_E_PRECONDITION                   -42 ///< \~chinese 前置条件不满足 \~english Preconditions are not met
#define ST_E_SIGN_ACTIVATION_CODE_TOKEN_EXPIRE -43 ///< \~chinese 激活码token过期 \~english Activation code token expired
#define ST_E_SIGN_ACTIVATION_CODE_EXPIRE    -44 ///< \~chinese 激活码过期 \~english Activation code expired
#define ST_E_CHIP_NOTSUPPORTED              -45 ///< \~chinese 当前芯片不支持此模型 \~english The current chip does not support this model
// 渲染相关错误码
#define ST_E_INVALID_GL_CONTEXT             -100 ///< \~chinese OpenGL Context错误,当前为空,或不一致 \~english OpenGL context error, currently empty or inconsistent
#define ST_E_RENDER_DISABLED                -101 ///< \~chinese 创建句柄时候没有开启渲染 \~english Rendering not enabled when creating the handle


// STA错误码
#define ST_E_FORBID_PUSH_DATA_WHEN_STOPPING   -200 ///< \~chinese 调用Stop接口过程中不再接收数据 \~english Data push is forbidden while stopping. This means that data should not be pushed when the Stop interface is called

#define ST_E_OUT_OF_TIME_AREA                 -300 ///< \~chinese 输入的时间戳不在当前音素时间区域内 \~english The input timestamp is not within the current phoneme time area

#ifndef CHECK_FLAG
#define CHECK_FLAG(action,flag) (((action)&(flag)) == flag)
#endif

/// \~chinese 矩形 \~english Rectangle definition
typedef struct st_rect_t {
    int left;   ///< \~chinese 矩形最左边的坐标 \~english Left coordinate of the rectangle
    int top;    ///< \~chinese 矩形最上边的坐标 \~english Top coordinate of the rectangle
    int right;  ///< \~chinese 矩形最右边的坐标 \~english Right coordinate of the rectangle
    int bottom; ///< \~chinese 矩形最下边的坐标 \~english Bottom coordinate of the rectangle
} st_rect_t;

/// \~chinese 2d float点 \~english 2D point with floating-point definition
typedef struct st_pointf_t {
    float x;    ///< \~chinese 点的水平方向坐标,为浮点数 \~english The horizontal coordinate of the point as a float
    float y;    ///< \~chinese 点的竖直方向坐标,为浮点数 \~english The vertical coordinate of the point as a float
} st_pointf_t;

/// \~chinese 3d float 点 \~english 3D point with floating-point definition
typedef struct st_point3f_t {
	float x;    ///< \~chinese 点的水平方向坐标 \~english The horizontal coordinate of the point
	float y;    ///< \~chinese 点的竖直方向坐标 \~english The vertical coordinate of the point
	float z;    ///< \~chinese 点的深度坐标    \~english The depth coordinate of the point
} st_point3f_t;

/// \~chinese 2d int 点 \~english 2D point with integer definition
typedef struct st_pointi_t {
    int x;      ///< \~chinese 点的水平方向坐标,为整数 \~english The horizontal coordinate of the point as an int
    int y;      ///< \~chinese 点的竖直方向坐标,为整数 \~english The vertical coordinate of the point as an int
} st_pointi_t;

/// \~chinese 四元数定义了在三维空间中表示旋转的数学工具，可用于旋转和插值计算 \~english A quaternion is a mathematical tool for representing rotations in three-dimensional space. It can be used for rotation and interpolation calculations
typedef struct st_quaternion_t {
	float w;     ///< \~chinese 四元数的实部  \~english Real part of the quaternion
	float x;     ///< \~chinese 四元数的虚部i \~english Imaginary part i of the quaternion
	float y;     ///< \~chinese 四元数的虚部j \~english Imaginary part j of the quaternion
	float z;     ///< \~chinese 四元数的虚部k \~english Imaginary part k of the quaternion
} st_quaternion_t;

/// \~chinese 颜色类型 \~english Color definition
typedef struct st_color_t {
    float r;
    float g;
    float b;
    float a;
} st_color_t;

/// \~chinese 图像每个像素类型定义 \~english Pixel format definition
typedef enum {
    ST_PIX_FMT_GRAY8 = 0,   ///< Y    1        8bpp ( \~chinese 单通道8bit灰度像素 \~english Single-channel 8-bit grayscale pixel)
    ST_PIX_FMT_YUV420P = 1, ///< YUV  4:2:0   12bpp ( \~chinese 3通道, 一个亮度通道, 另两个为U分量和V分量通道, 所有通道都是连续的. 只支持I420 \~english 3 channels, one luminance channel, and the other two are U and V component channels, all channels are contiguous. Only supports I420)
    ST_PIX_FMT_NV12 = 2,    ///< YUV  4:2:0   12bpp ( \~chinese 2通道, 一个通道是连续的亮度通道, 另一通道为UV分量交错 \~english 2 channels, one channel is a contiguous luminance channel, and the other channel is interleaved UV components)
    ST_PIX_FMT_NV21 = 3,    ///< YUV  4:2:0   12bpp ( \~chinese 2通道, 一个通道是连续的亮度通道, 另一通道为VU分量交错 \~english 2 channels, one channel is a contiguous luminance channel, and the other channel is interleaved VU components)
    ST_PIX_FMT_BGRA8888 = 4,///< BGRA 8:8:8:8 32bpp ( \~chinese 4通道32bit BGRA 像素 \~english 4-channel 32-bit BGRA pixel)
    ST_PIX_FMT_BGR888 = 5,  ///< BGR  8:8:8   24bpp ( \~chinese 3通道24bit BGR 像素 \~english 3-channel 24-bit BGR pixel)
    ST_PIX_FMT_RGBA8888 = 6,///< RGBA 8:8:8:8 32bpp ( \~chinese 4通道32bit RGBA 像素 \~english 4-channel 32-bit RGBA pixel)
    ST_PIX_FMT_RGB888 = 7,  ///< RGB  8:8:8   24bpp ( \~chinese 3通道24bit RGB 像素 \~english 3-channel 24-bit RGB pixel)
    ST_PIX_FMT_FLOAT = 8    ///< Y    1       32bpp ( \~chinese 1通道 32bit float 像素 \~english Single-channel 32-bit float pixel)
} st_pixel_format;

/// \~chinese 图像旋转方向 \~english Image rotate type
typedef enum {
    ST_CLOCKWISE_ROTATE_0 = 0,  ///< \~chinese 图像不需要旋转,图像中的人脸为正脸 \~english Image does not need to be rotated, the face in the image is upright
    ST_CLOCKWISE_ROTATE_90 = 1, ///< \~chinese 图像需要顺时针旋转90度,使图像中的人脸为正 \~english Image needs to be rotated clockwise by 90 degrees to make the face in the image upright
    ST_CLOCKWISE_ROTATE_180 = 2,///< \~chinese 图像需要顺时针旋转180度,使图像中的人脸为正 \~english Image needs to be rotated clockwise by 180 degrees to make the face in the image upright
    ST_CLOCKWISE_ROTATE_270 = 3 ///< \~chinese 图像需要顺时针旋转270度,使图像中的人脸为正 \~english Image needs to be rotated clockwise by 270 degrees to make the face in the image upright
} st_rotate_type;

/// \~chinese 图像格式定义 \~english Image defination
typedef struct st_image_t {
    unsigned char *data;    ///< \~chinese 图像数据指针 \~english Image data pointer
    st_pixel_format pixel_format;   ///< \~chinese 像素格式 \~english Pixel format
    int width;              ///< \~chinese 宽度(以像素为单位) \~english Width (in pixels)
    int height;             ///< \~chinese 高度(以像素为单位) \~english Height(in pixels)
    int stride;             ///< \~chinese 跨度, 即每行所占的字节数 \~english Stride, the number of bytes occupied by each row
    double time_stamp;      ///< \~chinese 时间戳 \~english Time stamp
} st_image_t;

/// \~chinese @brief 方向类型
/// \~english @brief Direction type
typedef enum st_left_or_right_t {
    ST_LOR_UNKNOWN = 0,   ///< \~chinese 未知方向 \~english Unknown direction
    ST_LOR_RIGHT = 1,     ///< \~chinese 右      \~english Right
    ST_LOR_LEFT = 2       ///< \~chinese 左      \~english Left
} st_left_or_right_t;

/// \~chinese @brief 人脸106点信息
/// \~english @brief 106-point facial information
typedef struct st_mobile_106_t {
    st_rect_t rect;         ///< \~chinese 代表面部的矩形区域 \~english Rectangle representing the face area
    float score;            ///< \~chinese 置信度 \~english Confidence score
    st_pointf_t points_array[106];  ///< \~chinese 人脸106关键点的数组 \~english Array of 106 facial key points
    float visibility_array[106];    ///< \~chinese 对应点的能见度,点未被遮挡1.0,被遮挡0.0 \~english Visibility of each point, 1.0 for visible points and 0.0 for occluded points
    float yaw;              ///< \~chinese 水平转角,真实度量的左负右正 \~english Yaw angle, negative for left and positive for right in real-world measurement
    float pitch;            ///< \~chinese 俯仰角,真实度量的上负下正 \~english Pitch angle, negative for up and positive for down in real-world measurement
    float roll;             ///< \~chinese 旋转角,真实度量的左负右正 \~english Roll angle, negative for left and positive for right in real-world measurement
    float eye_dist;         ///< \~chinese 两眼间距 \~english  Distance between the two eyes
    int ID;                 ///< \~chinese faceID: 每个检测到的人脸拥有唯一的faceID.人脸跟踪丢失以后重新被检测到,会有一个新的faceID \~english faceID: Each detected face has a unique faceID. When a face is lost and re-detected, a new faceID will be assigned
} st_mobile_106_t, *p_st_mobile_106_t;

/// \~chinese @brief 耳朵关键点信息
/// \~english @brief Ear keypoint information
typedef struct st_mobile_ear_t {
	st_pointf_t *p_ear_points;          ///< \~chinese 耳朵关键点. 没有检测到时为NULL.耳朵左右各有18个关键点,共36个关键点,0-4为左耳靠近内耳区域的一条线,5-17为左耳外耳廓,18-22为右耳靠近内耳区域的一条线,23-35为右耳外耳廓 \~english Ear key points. NULL when not detected. Each ear has 18 key points, with a total of 36 key points. Points 0-4 represent a line near the inner ear of the left ear, points 5-17 represent the outer contour of the left ear, points 18-22 represent a line near the inner ear of the right ear, and points 23-35 represent the outer contour of the right ear.
	int ear_points_count;               ///< \~chinese 耳朵关键点个数. 检测到时为ST_MOBILE_EAR_POINTS_COUNT, 没有检测到时为0 \~english Number of ear key points. ST_MOBILE_EAR_POINTS_COUNT when detected, 0 when not detected
	float left_ear_score;               ///< \~chinese 左耳检测结果置信度: [0.0, 1.0] \~english Confidence score of left ear detection: [0.0, 1.0]
	float right_ear_score;              ///< \~chinese 右耳检测结果置信度: [0.0, 1.0] \~english Confidence score of right ear detection: [0.0, 1.0]
} st_mobile_ear_t, *p_st_mobile_ear_t;

/// \~chinese @brief 额头关键点信息
/// \~english @brief Forehead keypoint information
typedef struct st_mobile_forehead_t {
	st_pointf_t *p_forehead_points;     ///< \~chinese 额头点 \~english Forehead points
	int forehead_points_count;          ///< \~chinese 额头点个数 \~english Number of forehead points
} st_mobile_forehead_t, *p_st_mobile_forehead_t;

/// \~chinese @brief mesh部位
/// \~english @brief Mesh parts
typedef enum {
	ST_MOBILE_MESH_PART_FACE = 1,     ///< \~chinese 人脸部位 \~english Face part
	ST_MOBILE_MESH_PART_EYE = 2,      ///< \~chinese 眼睛部位 \~english Eye part
	ST_MOBILE_MESH_PART_MOUTH = 4,    ///< \~chinese 嘴巴部位 \~english Mouth part
	ST_MOBILE_MESH_PART_SKULL = 8,    ///< \~chinese 后脑勺部位 \~english Skull part
	ST_MOBILE_MESH_PART_EAR = 16,     ///< \~chinese 耳朵部位 \~english Ear part
	ST_MOBILE_MESH_PART_NECK = 32,    ///< \~chinese 脖子部位 \~english Neck part
	ST_MOBILE_MESH_PART_EYEBROW = 64  ///< \~chinese 眉毛部位 \~english Eyebrow part
} st_mobile_mesh_part;

typedef struct st_mobile_mesh_part_uv_t{
	st_mobile_mesh_part part;     ///< \~chinese mesh关键部位 \~english Mesh key part
	st_pointf_t* p_uv;            ///< \~chinese 纹理坐标数组  \~english Texture coordinate array
	int uv_points_count;          ///< \~chinese 纹理坐标个数  \~english Number of texture coordinates
}st_mobile_mesh_part_uv_t, *p_st_mobile_mesh_part_uv_t;

/// \~chinese 3d点的坐标系类型 \~english 3D point coordinate system type
typedef enum
{
	ST_3D_SCREEN_COORDINATE = 0,  ///< \~chinese 屏幕坐标系,也叫正交坐标系,（x,y)和图像坐标相同,z表示深度 \~english Screen coordinate system, also known as orthogonal coordinate system, (x, y) is the same as image coordinates, z represents depth
	ST_3D_WORLD_COORDINATE = 1,   ///< \~chinese 3d世界坐标系 \~english 3D world coordinate system
	ST_3D_CAM_COORDINATE = 2      ///< \~chinese 3d摄像机坐标系,是摄像头透视投影坐标系, 原点在摄像机 \~english  3D camera coordinate system, which is the camera's perspective projection coordinate system with the camera as the origin
} st_3d_coordinate_type;

/// \~chinese @brief 3d mesh关键点信息
/// \~english @brief 3D mesh keypoint information
typedef struct st_mobile_face_mesh_t
{
	st_point3f_t *p_face_mesh_points;   ///< \~chinese 3DMesh关键点数组 \~english Array of 3D mesh key points
	st_point3f_t *p_face_mesh_normal;   ///< \~chinese 3DMesh法线,每个法线对应一个关键点 \~english 3D mesh normals, each normal corresponds to a key point
	st_pointf_t *p_texcoords;           ///< \~chinese 3DMesh纹理坐标,每个纹理坐标对应一个关键点 \~english 3D mesh texture coordinates, each texture coordinate corresponds to a key point
	int face_mesh_points_count;         ///< \~chinese 3DMesh关键点的数目 \~english Number of key points in 3D mesh
	float scale;                        ///< \~chinese 3DMesh相对于标准平均脸的大小信息,值越大表示人脸越大（人脸越靠前) \~english Size information of the 3D mesh relative to the standard average face, larger values indicate larger faces (closer faces)
	st_mobile_mesh_part_uv_t * p_part_uv; ///< \~chinese uv纹理信息,目前仅支持眉毛 \~english UV texture information, currently only supports eyebrows
	int part_uv_count;                    ///< \~chinese uv纹理数目,目前仅支持眉毛 \~english Number of UV textures

	float transform_mat[4][4];          ///< \~chinese 旋转变换矩阵,用于将关键点从屏幕空间变换到3D空间 \~english Rotation transformation matrix used to transform key points from screen space to 3D space
	float transform_inv_mat[4][4];      ///< \~chinese 旋转变换矩阵的逆矩阵,用于将关键点从3D空间变换到屏幕空间 \~english Inverse matrix of the rotation transformation matrix used to transform key points from 3D space to screen space
	float view_mat[4][4];               ///< \~chinese 视角矩阵 \~english View matrix
	float project_mat[4][4];            ///< \~chinese 投影矩阵 \~english Projection matrix
	st_3d_coordinate_type coord_type;	///< \~chinese 类型见st_3d_coordinate_type_t 0 屏幕坐标系  1世界坐标系 2相机坐标系 \~english Coordinate type (st_3d_coordinate_type_t): 0 - Screen coordinate system, 1 - World coordinate system, 2 - Camera coordinate system
	int extra_bits;                     ///< \~chinese 3d pose 结果描述, 枚举值见 st_3dpose_extra_bit_t 目前表示当前结果与图像是否是镜像关系 \~english 3D pose result description, enumeration values see st_3dpose_extra_bit_t, currently indicates whether the current result is mirrored to the image
} st_mobile_face_mesh_t, *p_st_mobile_face_mesh_t;

/// \~chinese @brief 多平面image数据结构,支持单平面（RGBA、BGRA）,双平面（NV21/NV12)、三平面（YUV420）
/// \~english @brief Multiple planes image data definition, supports single-plane (RGBA, BGRA), dual-plane (NV21, NV12), and triple-plane (YUV420)
typedef struct
{
    unsigned char* planes[3];   ///< \~chinese 图像平面内存地址 \~english Image Plane
    int strides[3];             ///< \~chinese 图像每行的跨距,有效跨距应该与plane对应 \~english Stride of each row in the image, valid stride should correspond to the plane
    int width;                  ///< \~chinese 图像宽度 \~english Image width
    int height;                 ///< \~chinese 图像高度 \~english Image height
    st_pixel_format format;     ///< \~chinese 图像的格式 \~english Input image format
} st_multiplane_image_t;

/// \~chinese 人脸检测内部参数 \~english Internal information for face detection
typedef struct st_mobile_face_extra_info {
	float affine_mat[3][3];				   ///< \~chinese 仿射变换矩阵 \~english Affine transformation matrix
	int model_input_size;                  ///< \~chinese 内部模型输入大小 \~english Internal model input size
} st_mobile_face_extra_info;

typedef enum
{
	ST_3DPOSE_NEED_MIRROR = 1,      // pose result needs mirror
	ST_3DPOSE_DELAY_INVALID = 2,    // pose currently invalid due to delay strategy.
} st_3dpose_extra_bit_t;

typedef struct
{
	float mat[16];       // 4x4 column major pose matrix.
	int extra_bits;      // detect image/result mirror flag.
} st_3dpose_t;



// \~chinese create时指定的config, 需根据使用场景来使用, 预览使用ST_MOBILE_DETECT_MODE_PREVIEW, 离线视频处理使用ST_MOBILE_DETECT_MODE_VIDEO_POST_PROCESS, 图片使用ST_MOBILE_DETECT_MODE_IMAGE \~english Specify the detection mode based on the specific use case. ST_MOBILE_DETECT_MODE_PREVIEW is suitable for real-time preview scenarios, such as live camera streams. ST_MOBILE_DETECT_MODE_VIDEO_POST_PROCESS is designed for offline video processing. It allows for more accurate face detection by utilizing temporal information across video frames. ST_MOBILE_DETECT_MODE_IMAGE is optimized for processing static images
#define ST_MOBILE_TRACKING_MULTI_THREAD         0x00000000  ///< \~chinese 多线程,功耗较多,卡顿较少 \~english Multi-threaded tracking mode, which consumes more power but provides smoother performance with fewer frame drops
#define ST_MOBILE_TRACKING_SINGLE_THREAD        0x00010000  ///< \~chinese 单线程,功耗较少,对于性能弱的手机,会偶尔有卡顿现象 \~english single-threaded tracking mode, which consumes less power but may experience occasional frame drops, especially on low-performance devices
/// \~chinese 检测模式
#define ST_MOBILE_DETECT_MODE_VIDEO             0x00020000  ///< \~chinese 旧版本视频检测,效果同预览检测,后续会更新为预览检测 \~english Old version of video detection mode, same as ST_MOBILE_DETECT_MODE_PREVIEW
#define ST_MOBILE_DETECT_MODE_PREVIEW           0x00020000  ///< \~chinese 预览检测 \~english Preview detection mode, suitable for real-time preview scenarios, such as live camera streams
#define ST_MOBILE_DETECT_MODE_IMAGE             0x00040000  ///< \~chinese 图片检测 与预览检测互斥,只能同时使用一个 \~english Image detection mode, optimized for processing static images
#define ST_MOBILE_DETECT_MODE_VIDEO_POST_PROCESS 0x00100000 ///< \~chinese 视频后处理检测,与预览、图片检测互斥,只能同时使用一个 \~english Video post-processing detection, mutually exclusive with preview and image detection, only one can be used at a time
#define ST_MOBILE_ENABLE_INPUT_CUSTOM           0x00080000  ///< \~chinese 使用用户自定义的结果作为输入,必须和视频/图片模式混用,目前使用人脸结果有效 \~english Enable input customization using user-defined results, currently valid for face results

//#define ST_MOBILE_TRACKING_ENABLE_DEBOUNCE      0x00000010  ///< \~chinese 打开人脸106点和三维旋转角度去抖动 \~english Enable debouncing for face 106 points and 3D rotation angles
//#define ST_MOBILE_TRACKING_ENABLE_FACE_ACTION   0x00000020  ///< \~chinese 检测脸部动作：张嘴、眨眼、抬眉、点头、摇头 \~english Enable face actions detection: mouth open, blink, raise eyebrows, nod, shake head

/// \~chinese @brief 人脸检测结果
/// \~english @brief Face detection result
typedef struct st_mobile_face_t {
    st_mobile_106_t face106;               ///< \~chinese 人脸信息,包含矩形框、106点、head pose信息等 \~english Face information including bounding box, 106 points, head pose, etc
    st_pointf_t *p_extra_face_points;      ///< \~chinese 眼睛、眉毛、嘴唇关键点. 没有检测到时为NULL \~english Extra facial keypoints for eyes, eyebrows, and lips. NULL if not detected
    int extra_face_points_count;           ///< \~chinese 眼睛、眉毛、嘴唇关键点个数. 检测到时为ST_MOBILE_EXTRA_FACE_POINTS_COUNT, 没有检测到时为0 \~english Number of extra facial keypoints. ST_MOBILE_EXTRA_FACE_POINTS_COUNT when detected, 0 if not detected
    st_pointf_t * p_tongue_points;         ///< \~chinese 舌头关键点数组 \~english Tongue keypoints array
    float * p_tongue_points_score;         ///< \~chinese 舌头关键点对应的置信度 \~english Confidence scores for tongue keypoints
    int tongue_points_count;               ///< \~chinese 舌头关键点的数目 \~english Number of tongue keypoints
    st_mobile_face_mesh_t * p_face_mesh;   ///< \~chinese 3d mesh信息,包括3d mesh关键点及个数 \~english 3D mesh information including keypoints and count
    st_pointf_t *p_eyeball_center;         ///< \~chinese 眼球中心关键点. 没有检测到时为NULL \~english Eyeball center keypoints. NULL if not detected
    int eyeball_center_points_count;       ///< \~chinese 眼球中心关键点个数. 检测到时为ST_MOBILE_EYEBALL_CENTER_POINTS_COUNT, 没有检测到时为0 \~english Number of eyeball center keypoints. ST_MOBILE_EYEBALL_CENTER_POINTS_COUNT when detected, 0 if not detected
    st_pointf_t *p_eyeball_contour;        ///< \~chinese 眼球轮廓关键点. 没有检测到时为NULL \~english Eyeball contour keypoints. NULL if not detected
    int eyeball_contour_points_count;      ///< \~chinese 眼球轮廓关键点个数. 检测到时为ST_MOBILE_EYEBALL_CONTOUR_POINTS_COUNT, 没有检测到时为0 \~english Number of eyeball contour keypoints. ST_MOBILE_EYEBALL_CONTOUR_POINTS_COUNT when detected, 0 if not detected
    float left_eyeball_score;              ///< \~chinese 左眼球检测结果（中心点和轮廓点）置信度: [0.0, 1.0],282左眼阈值为0.8 \~english Confidence score for left eyeball detection (center point and contour points): [0.0, 1.0]. Threshold for the left eye is 0.8
    float right_eyeball_score;             ///< \~chinese 右眼球检测结果（中心点和轮廓点）置信度: [0.0, 1.0],282右眼阈值为0.8 \~english Confidence score for right eyeball detection (center point and contour points): [0.0, 1.0]. Threshold for the right eye is 0.8
    st_mobile_ear_t* p_face_ear;           ///< \~chinese 耳朵信息,包括耳朵关键点及个数,左右耳置信度 \~english Ear information including ear keypoints, count, and confidence for left and right ears
    st_point3f_t *p_gaze_direction;        ///< \~chinese 左眼和右眼视线方向,没有检测到是为NULL \~english Gaze directions for left and right eyes. NULL if not detected
    float *p_gaze_score;                   ///< \~chinese 视线置信度: [0.0, 1.0], 建议阈值为0.5 \~english Gaze direction confidence scores: [0.0, 1.0]. Recommended threshold is 0.5
    unsigned long long face_action;        ///< \~chinese 脸部动作 \~english Facial actions
    float *p_face_blend_shapes;            ///< \~chinese 面部表情动画系数 \~english Facial blend shape coefficients for animation
    int blend_shapes_count;                ///< \~chinese 面部表情动画数目 \~english Number of facial blend shapes
    int s_type;                            ///
    float *p_face_action_score;            ///< \~chinese 脸部动作置信度, eye, mouth, pitch, yaw, brow \~english Confidence scores for facial actions: eye, mouth, pitch, yaw, brow
    int face_action_score_count;           ///< \~chinese 脸部动作数目 \~english Number of facial actions
    st_mobile_forehead_t* p_face_forehead; ///< \~chinese 额头点信息,包括额头点坐标和个数 \~english Forehead point information including coordinates and count
    st_color_t hair_color;                 ///< \~chinese avatar发色, rgb取值范围[0.0, 1.0]; 其中a(alpha)值不必要,设置默认值为1.0 \~english Avatar hair color. RGB values range from [0.0, 1.0]. The alpha value is not necessary and is set to the default value of 1.0
    st_mobile_face_extra_info face_extra_info;///< \~chinese 人脸检测模型内部参数 \~english  Internal info of the face detection model
    st_3dpose_t *p_face_pose;                 ///< \~chinese 人脸新pose3d信息 \~english 3D pose information of the face
} st_mobile_face_t, *p_st_mobile_face_t;

/// \~chinese @brief 设置眨眼动作的阈值,置信度为[0,1], 默认阈值为0.5
/// \~english @brief Set the threshold for eye blink action. Confidence range is [0, 1]
ST_SDK_API void
st_mobile_set_eyeblink_threshold(
    float threshold
);
/// \~chinese @brief 设置张嘴动作的阈值,置信度为[0,1], 默认阈值为0.5
/// \~english @brief Set the threshold for mouth open action. Confidence range is [0, 1]
ST_SDK_API void
st_mobile_set_mouthah_threshold(
    float threshold
);
/// \~chinese @brief 设置摇头动作的阈值,置信度为[0,1], 默认阈值为0.5
/// \~english @brief Set the threshold for head shake action. Confidence range is [0, 1]
ST_SDK_API void
st_mobile_set_headyaw_threshold(
    float threshold
);
/// \~chinese @brief 设置点头动作的阈值,置信度为[0,1], 默认阈值为0.5
/// \~english @brief Set the threshold for head nod action. Confidence range is [0, 1]
ST_SDK_API void
st_mobile_set_headpitch_threshold(
    float threshold
);
/// \~chinese @brief 设置挑眉毛动作的阈值,置信度为[0,1], 默认阈值为0.5
/// \~english @brief Set the threshold for eyebrow raise action. Confidence range is [0, 1]
ST_SDK_API void
st_mobile_set_browjump_threshold(
    float threshold
);

/// \~chinese @brief 设置人脸三维旋转角度去抖动的阈值. 若不设置, 使用默认值. 默认值0.5, 建议取值范围：[0.0, 1.0]. 阈值越大, 去抖动效果越好, 跟踪延时越大
/// \~english @brief Set the threshold for face 3D head pose jittering. If not set, use the default value. Default value is 0.5. Recommended range is [0.0, 1.0]. A larger threshold value provides better jitter reduction but increases tracking latency
ST_SDK_API void
st_mobile_set_headpose_threshold(
    float threshold
);

/// \~chinese @brief 设置只使用SSE指令集,仅windows上有效,建议在不支持fma或sse2指令集的windows系统上（如xp系统)使用
/// \~english @brief Set to only use SSE instruction set. Only works on Windows. Recommended for Windows systems that do not support FMA or SSE2 instruction sets (e.g., Windows XP)
ST_SDK_API void
st_mobile_set_sse_only(
bool sse_only
);

/// \~chinese 支持的颜色转换格式 \~english Supported color conversion formats
typedef enum {
    ST_BGRA_YUV420P = 0,    ///< \~chinese ST_PIX_FMT_BGRA8888到ST_PIX_FMT_YUV420P转换 \~english ST_PIX_FMT_BGRA8888 to ST_PIX_FMT_YUV420P
    ST_BGR_YUV420P = 1,     ///< \~chinese ST_PIX_FMT_BGR888到ST_PIX_FMT_YUV420P转换 \~english ST_PIX_FMT_BGR888 to ST_PIX_FMT_YUV420P
    ST_BGRA_NV12 = 2,       ///< \~chinese ST_PIX_FMT_BGRA8888到ST_PIX_FMT_NV12转换 \~english ST_PIX_FMT_BGRA8888 to ST_PIX_FMT_NV12
    ST_BGR_NV12 = 3,        ///< \~chinese ST_PIX_FMT_BGR888到ST_PIX_FMT_NV12转换 \~english ST_PIX_FMT_BGR888 to ST_PIX_FMT_NV12
    ST_BGRA_NV21 = 4,       ///< \~chinese ST_PIX_FMT_BGRA8888到ST_PIX_FMT_NV21转换 \~english ST_PIX_FMT_BGRA8888 to ST_PIX_FMT_NV21
    ST_BGR_NV21 = 5,        ///< \~chinese ST_PIX_FMT_BGR888到ST_PIX_FMT_NV21转换 \~english ST_PIX_FMT_BGR888 to ST_PIX_FMT_NV21
    ST_YUV420P_BGRA = 6,    ///< \~chinese ST_PIX_FMT_YUV420P到ST_PIX_FMT_BGRA8888转换 \~english ST_PIX_FMT_YUV420P to ST_PIX_FMT_BGRA8888
    ST_YUV420P_BGR = 7,     ///< \~chinese ST_PIX_FMT_YUV420P到ST_PIX_FMT_BGR888转换 \~english ST_PIX_FMT_YUV420P to ST_PIX_FMT_BGR888
    ST_NV12_BGRA = 8,       ///< \~chinese ST_PIX_FMT_NV12到ST_PIX_FMT_BGRA8888转换 \~english ST_PIX_FMT_NV12 to ST_PIX_FMT_BGRA8888
    ST_NV12_BGR = 9,        ///< \~chinese ST_PIX_FMT_NV12到ST_PIX_FMT_BGR888转换 \~english ST_PIX_FMT_NV12 to ST_PIX_FMT_BGR888
    ST_NV21_BGRA = 10,      ///< \~chinese ST_PIX_FMT_NV21到ST_PIX_FMT_BGRA8888转换 \~english ST_PIX_FMT_NV21 to ST_PIX_FMT_BGRA8888
    ST_NV21_BGR = 11,       ///< \~chinese ST_PIX_FMT_NV21到ST_PIX_FMT_BGR888转换 \~english ST_PIX_FMT_NV21 to ST_PIX_FMT_BGR888
    ST_BGRA_GRAY = 12,      ///< \~chinese ST_PIX_FMT_BGRA8888到ST_PIX_FMT_GRAY8转换 \~english ST_PIX_FMT_BGRA8888 to ST_PIX_FMT_GRAY8
    ST_BGR_BGRA = 13,       ///< \~chinese ST_PIX_FMT_BGR888到ST_PIX_FMT_BGRA8888转换 \~english ST_PIX_FMT_BGR888 to ST_PIX_FMT_BGRA8888
    ST_BGRA_BGR = 14,       ///< \~chinese ST_PIX_FMT_BGRA8888到ST_PIX_FMT_BGR888转换 \~english ST_PIX_FMT_BGRA8888 to ST_PIX_FMT_BGR888
    ST_YUV420P_GRAY = 15,   ///< \~chinese ST_PIX_FMT_YUV420P到ST_PIX_FMT_GRAY8转换 \~english ST_PIX_FMT_YUV420P to ST_PIX_FMT_GRAY8
    ST_NV12_GRAY = 16,      ///< \~chinese ST_PIX_FMT_NV12到ST_PIX_FMT_GRAY8转换 \~english ST_PIX_FMT_NV12 to ST_PIX_FMT_GRAY8
    ST_NV21_GRAY = 17,      ///< \~chinese ST_PIX_FMT_NV21到ST_PIX_FMT_GRAY8转换 \~english ST_PIX_FMT_NV21 to ST_PIX_FMT_GRAY8
    ST_BGR_GRAY = 18,       ///< \~chinese ST_PIX_FMT_BGR888到ST_PIX_FMT_GRAY8转换 \~english ST_PIX_FMT_BGR888 to ST_PIX_FMT_GRAY8
    ST_GRAY_YUV420P = 19,   ///< \~chinese ST_PIX_FMT_GRAY8到ST_PIX_FMT_YUV420P转换 \~english ST_PIX_FMT_GRAY8 to ST_PIX_FMT_YUV420P
    ST_GRAY_NV12 = 20,      ///< \~chinese ST_PIX_FMT_GRAY8到ST_PIX_FMT_NV12转换 \~english ST_PIX_FMT_GRAY8 to ST_PIX_FMT_NV12
    ST_GRAY_NV21 = 21,      ///< \~chinese ST_PIX_FMT_GRAY8到ST_PIX_FMT_NV21转换 \~english ST_PIX_FMT_GRAY8 to ST_PIX_FMT_NV21
    ST_NV12_YUV420P = 22,   ///< \~chinese ST_PIX_FMT_NV12到ST_PIX_FMT_YUV420P转换 \~english ST_PIX_FMT_NV12 to ST_PIX_FMT_YUV420P
    ST_NV21_YUV420P = 23,   ///< \~chinese ST_PIX_FMT_NV21到ST_PIX_FMT_YUV420P转换 \~english ST_PIX_FMT_NV21 to ST_PIX_FMT_YUV420P
    ST_NV21_RGBA = 24,      ///< \~chinese ST_PIX_FMT_NV21到ST_PIX_FMT_RGBA8888转换 \~english ST_PIX_FMT_NV21 to ST_PIX_FMT_RGBA8888
    ST_BGR_RGBA = 25,       ///< \~chinese ST_PIX_FMT_BGR888到ST_PIX_FMT_RGBA8888转换 \~english ST_PIX_FMT_BGR888 to ST_PIX_FMT_RGBA8888
    ST_BGRA_RGBA = 26,      ///< \~chinese ST_PIX_FMT_BGRA8888到ST_PIX_FMT_RGBA8888转换 \~english ST_PIX_FMT_BGRA8888 to ST_PIX_FMT_RGBA8888
    ST_RGBA_BGRA = 27,      ///< \~chinese ST_PIX_FMT_RGBA8888到ST_PIX_FMT_BGRA8888转换 \~english ST_PIX_FMT_RGBA8888 to ST_PIX_FMT_BGRA8888
    ST_GRAY_BGR = 28,       ///< \~chinese ST_PIX_FMT_GRAY8到ST_PIX_FMT_BGR888转换 \~english ST_PIX_FMT_GRAY8 to ST_PIX_FMT_BGR888
    ST_GRAY_BGRA = 29,      ///< \~chinese ST_PIX_FMT_GRAY8到ST_PIX_FMT_BGRA8888转换 \~english ST_PIX_FMT_GRAY8 to ST_PIX_FMT_BGRA8888
    ST_NV12_RGBA = 30,      ///< \~chinese ST_PIX_FMT_NV12到ST_PIX_FMT_RGBA8888转换 \~english ST_PIX_FMT_NV12 to ST_PIX_FMT_RGBA8888
    ST_NV12_RGB = 31,       ///< \~chinese ST_PIX_FMT_NV12到ST_PIX_FMT_RGB888转换 \~english ST_PIX_FMT_NV12 to ST_PIX_FMT_RGB888
    ST_RGBA_NV12 = 32,      ///< \~chinese ST_PIX_FMT_RGBA8888到ST_PIX_FMT_NV12转换 \~english ST_PIX_FMT_RGBA8888 to ST_PIX_FMT_NV12
    ST_RGB_NV12 = 33,       ///< \~chinese ST_PIX_FMT_RGB888到ST_PIX_FMT_NV12转换 \~english ST_PIX_FMT_RGB888 to ST_PIX_FMT_NV12
    ST_RGBA_BGR = 34,       ///< \~chinese ST_PIX_FMT_RGBA888到ST_PIX_FMT_BGR888转换 \~english ST_PIX_FMT_RGBA888 to ST_PIX_FMT_BGR888
    ST_BGRA_RGB = 35,       ///< \~chinese ST_PIX_FMT_BGRA888到ST_PIX_FMT_RGB888转换 \~english ST_PIX_FMT_BGRA888 to ST_PIX_FMT_RGB888
    ST_RGBA_GRAY = 36,      ///< \~chinese ST_PIX_FMT_RGBA8888到ST_PIX_FMT_GRAY8转换 \~english ST_PIX_FMT_RGBA8888 to ST_PIX_FMT_GRAY8
    ST_RGB_GRAY = 37,       ///< \~chinese ST_PIX_FMT_RGB888到ST_PIX_FMT_GRAY8转换 \~english ST_PIX_FMT_RGB888 to ST_PIX_FMT_GRAY8
    ST_RGB_BGR = 38,        ///< \~chinese ST_PIX_FMT_RGB888到ST_PIX_FMT_BGR888转换 \~english ST_PIX_FMT_RGB888 to ST_PIX_FMT_BGR888
    ST_BGR_RGB = 39,        ///< \~chinese ST_PIX_FMT_BGR888到ST_PIX_FMT_RGB888转换 \~english ST_PIX_FMT_BGR888 to ST_PIX_FMT_RGB888
    ST_YUV420P_RGBA = 40,   ///< \~chinese ST_PIX_FMT_YUV420P到ST_PIX_FMT_RGBA8888转换 \~english ST_PIX_FMT_YUV420P to ST_PIX_FMT_RGBA8888
    ST_RGBA_YUV420P = 41,   ///< \~chinese ST_PIX_FMT_RGBA8888到ST_PIX_FMT_YUV420P转换 \~english ST_PIX_FMT_RGBA8888 to ST_PIX_FMT_YUV420P
    ST_RGBA_NV21 = 42,       ///< \~chinese ST_PIX_FMT_RGBA8888到ST_PIX_FMT_NV21转换 \~english ST_PIX_FMT_RGBA8888 to ST_PIX_FMT_NV21
	ST_NV21_RGB = 43,      ///< \~chinese ST_PIX_FMT_NV21到ST_PIX_FMT_RGB888转换 \~english ST_PIX_FMT_NV21 to ST_PIX_FMT_RGB
} st_color_convert_type;

/// \~chinese
/// @brief 图像颜色格式转换
/// @param[in] image_src 用于待转换的图像数据
/// @param[out] image_dst 转换后的图像数据
/// @param[in] image_width 用于转换的图像的宽度(以像素为单位)
/// @param[in] image_height 用于转换的图像的高度(以像素为单位),用户分配内存
/// @param[in] type 需要转换的颜色格式
/// @return  正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Convert image color format
/// @param[in] image_src Input image data to be converted
/// @param[out] image_dst Converted image data
/// @param[in] image_width Width of the image to be converted in pixels
/// @param[in] image_height Height of the image to be converted in pixels (user allocated memory)
/// @param[in] type Image format conversion type, defined as src to dst style
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_color_convert(
    const unsigned char *image_src,
    unsigned char *image_dst,
    int image_width,
    int image_height,
    st_color_convert_type type
);

/// \~chinese
/// @brief 旋转图像
/// @param[in] image_src 待旋转的图像数据
/// @param[out] image_dst 旋转后的图像数据, 由客户分配内存.旋转后,图像会变成紧凑的（没有padding）
/// @param[in] image_width 待旋转的图像的宽度, 旋转后图像的宽度可能会发生变化,由用户处理
/// @param[in] image_height 待旋转的图像的高度, 旋转后图像的高度可能会发生变化,由用户处理
/// @param[in] image_stride 待旋转的图像的跨度, 旋转后图像的跨度可能会发生变化,由用户处理
/// @param[in] pixel_format 待旋转的图像的格式
/// @param[in] rotate_type 顺时针旋转角度
/// \~english
/// @brief Rotate image
/// @param[in] image_src The input image data to be rotated
/// @param[out] image_dst The rotated image data(user allocated memory), the image will be compact (no padding) after rotation
/// @param[in] image_width Width of the image to be rotated. The width of the rotated image may change, to be handled by the user
/// @param[in] image_height Height of the rotated image may change
/// @param[in] image_stride Stride of the rotated image may change
/// @param[in] pixel_format Pixel format of the image to be rotated
/// @param[in] rotate_type The clockwise rotation type, 90, 180, 270
ST_SDK_API st_result_t
st_mobile_image_rotate(
    const unsigned char *image_src,
    unsigned char *image_dst,
    int image_width,
    int image_height,
    int image_stride,
    st_pixel_format pixel_format,
    st_rotate_type rotate_type
);

typedef enum
{
    version_1 = 1,
    version_2 = 2,
} st3d_face3d_delegate_version_t;

/// \~chinese @brief 图像resize类型
/// \~english @brief Image resize type
typedef enum {
	ST_MOBILE_RESIZE_TYPE_NEAREST_POINT = 0,   ///< \~chinese 最近邻插值 \~english Nearest-neighbor interpolation
	ST_MOBILE_RESIZE_TYPE_LINEAR = 1,          ///< \~chinese 线性插值   \~english Linear interpolation
} st_mobile_resize_type;

/// \~chinese
/// @brief 缩放图像
/// @param[in] src 原始图像
/// @param[out] dst 目标图像, 用户分配内存
/// @param[in] type 缩放方法
/// \~english
/// @brief  Scale image
/// @param[in] src The source image
/// @param[out] dst The target image, memory allocated by the user
/// @param[in] type The scaling method
ST_SDK_API st_result_t
st_mobile_image_resize(
	const st_image_t * src,
	st_image_t* dst,
	st_mobile_resize_type type
);

/// @brief 3D rigid transform structure.
typedef struct st_mobile_transform_t {
    float position[3];
    float eulerAngle[3];    /// euler in angle.
    float scale[3];
} st_mobile_transform_t;

/// \~chinese
/// @brief  将Translation,Rotation,Scale分量合成为一个4X4矩阵（列优先）,右手坐标系
/// @param[in] p_trs st_mobile_transform_t结构体表示的TRS分量
/// @param[out] mat4x4  转换之后的列优先存储的4X4齐次变换矩阵
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Combine Translation, Rotation, and Scale components into a 4x4 matrix (column-major), right-handed coordinate system
/// @param[in] p_trs TRS components represented by the st_mobile_transform_t structure
/// @param[out] mat4x4 The resulting 4x4 homogeneous transformation matrix stored in column-major order
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_convert_trs_to_matrix(const st_mobile_transform_t *p_trs, float mat4x4[16]);

/// \~chinese
/// @brief 将4X4矩阵（列优先）分解为Translation,Rotation,Scale分量,右手坐标系
/// @param[in] mat4x4 列优先存储的4X4齐次变换矩阵
/// @param[out] p_trs st_mobile_transform_t结构体表示的分解之后的TRS分量
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Decompose a 4x4 matrix (column-major) into Translation, Rotation, and Scale components, right-handed coordinate system
/// @param[in] mat4x4 The 4x4 homogeneous transformation matrix stored in column-major order
/// @param[out] p_trs The decomposed TRS components represented by the st_mobile_transform_t structure
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_convert_matrix_to_trs(const float mat4x4[16], st_mobile_transform_t *p_trs);

/// \~chinese @brief 性能/效果优先级
/// \~english @brief Performance/Quality priority
typedef enum {
    ST_PREFER_EFFECT,
    ST_PREFER_PERFORMANCE,
    ST_PREFER_AUTO_TUNE,
    ST_PREFER_NOTHING,
} st_performance_hint_t;

/// \~chinese @brief 纹理信息
/// \~english @brief Texture information
typedef struct {
    uint64_t id;            ///< \~chinese 纹理id  \~english Texture ID
    int width;              ///< \~chinese 纹理的宽 \~english Texture width
    int height;             ///< \~chinese 纹理的高 \~english Texture height
    st_pixel_format format; ///< \~chinese 纹理像素格式, 目前仅支持RGBA \~english Texture pixel format, currently only supports RGBA
} st_mobile_texture_t;

typedef void* (* st_mobile_GLloadproc)(const char *name);

/// \~chinese
/// @brief ANGLE GLES context设置gl API的loader函数指针，用于SDK内部运行时加载GLES API
/// @param[in] gl_loader st_mobile_GLloadproc类型的函数指针，一般在使用GLFW创建context时可以传入glfwGetProcAddress
/// @return 正常返回ST_OK,否则返回错误类型
/// \~chinese
/// @brief GL API loader function pointer for ANGLE GLES context, used to load GLES APIs inside SDK
/// @param[in] gl_loader st_mobile_GLloadproc type function pointer，glfwGetProcAddress could be passed in when created context with GLFW
/// @return error code, ST_OK for success, else see error code define
ST_SDK_API st_result_t
st_mobile_set_gl_loader(st_mobile_GLloadproc gl_loader);

// \~chinese 文件缓存存储的数据地址和文件字节长度 \~english File buffer address and buffer byte length
typedef struct
{
	char* p_data;
	int len;
} st_mobile_buffer_t;

//\~chinese 文件名字和文件缓冲 \~english File name and file buffer
typedef struct
{
	char* file_path;
	st_mobile_buffer_t buffer;
} st_mobile_file_buffer_t;

/// \~chinese
/// @brief 获取zip中的文件数据
/// @param[in] p_zip_path zip文件路径（相对或绝对）
/// @param[out] p_file_buffers 接收解压后文件数据的数组指针,传出底层分配的空间
/// @param[out] p_file_num  zip文件中压缩的文件总数
/// @param[out] p_zip_id  解压后zip资源的id,用于后续通知底层销毁对应资源
/// @return  成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Retrieve file data from a zip file
/// @param[in] p_zip_path Path to the zip file (relative or absolute)
/// @param[out] p_file_buffers Pointer to an array of the decompressed file data
/// @param[out] p_file_num Total number of compressed files in the zip file
/// @param[out] p_zip_id ID of the decompressed zip resource, used to destroy the corresponding resource later
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_unzip(const char* p_zip_path, st_mobile_file_buffer_t** p_file_buffers, int* p_file_num, int* p_zip_id);

/// \~chinese
/// @brief 获取zip中的文件数据
/// @param[in] p_zip_path zip文件路径（相对或绝对）
/// @param[out] p_file_buffers 接收解压后文件数据的数组指针,传出底层分配的空间
/// @param[out] p_file_num zip文件中压缩的文件总数
/// @param[out] p_zip_id 解压后zip资源的id,用于后续通知底层销毁对应资源
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Retrieve file data from a zip file
/// @param[in] p_zip_path Path to the zip file (relative or absolute)
/// @param[out] p_file_buffers Pointer to an array of the decompressed file data
/// @param[out] p_file_num Total number of compressed files in the zip file
/// @param[out] p_zip_id ID of the decompressed zip resource, used to destroy the corresponding resource later
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_unzip_from_buffer(const st_mobile_buffer_t* p_zip_buffer, st_mobile_file_buffer_t** p_file_buffers, int* p_file_num, int* p_zip_id);

/// \~chinese
/// @brief 获取zip中的文件数据
/// @param[in] zip_id st_mobile_unzip API返回的资源id,在调用该接口后,内部资源销毁
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Retrieve file data from a zip file
/// @param[in] zip_id The resource ID returned by the st_mobile_unzip API used to destroy internal resources
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_clear_unzipped_resource(int zip_id);

/// \~chinese @brief log层级定义
/// \~english @brief Log level
typedef enum {
    ST_LOG_DEBUG,
    ST_LOG_TRACE,
    ST_LOG_INFO,
    ST_LOG_WARNING,
    ST_LOG_ERROR,
    ST_LOG_DISABLE,
} st_log_level_t;

// this is the singleton API (thread safe), and would affect whole st_mobile APIs.
/// \~chinese
/// @brief 设置st_mobile当前的log层级,层级关系为自底向上的包含关系,如ST_LOG_ERROR包含所有其他可log的level。
///        将当前的log层级设置为ST_LOG_DISABLE将禁用所有log。
///        该接口保证线程安全。
/// @param[in] level 将设置的log层级
/// @return  正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Set the current log level for st_mobile, where the levels are hierarchical from bottom to top, such that ST_LOG_ERROR includes all other log levels. Setting the log level to ST_LOG_DISABLE will disable all logs. This API ensures thread safety
/// @param[in] level The log level to be set
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_set_log_level(st_log_level_t level);

/// \~chinese
/// @brief 获取当前的log层级
/// @param[out] p_level 应该非空,用于获取当前的log层级
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Get the current log level
/// @param[out] p_level A non-null pointer used to retrieve the current log level
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_get_log_level(st_log_level_t* p_level);

/// \~chinese
/// @brief 将log重定向到文件中,如果传入的文件路径为空,则重置为输出到标准设备流
/// @param[in] p_file_path 重定向log文件的全路径
/// @param[in] b_tranc_file 是否清除文件内容,true - 清除文件内容,false - 不清除文件内容
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Redirect log output to a file. If the file path is empty, reset it to the standard stream
/// @param[in] p_file_path The full path to the file for log redirection
/// @param[in] b_tranc_file Whether to truncate the file content
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_redirect_log_to_file(const char* p_file_path, bool b_tranc_file);

/// @brief log回调函数
/// @param[in] log_level log级别
/// @param[in] st_log_str log内容
typedef void(*st_log_callback)(st_log_level_t log_level, const char* st_log_str);

/// @brief 设置log回调函数,设置后log将不再打印到输出窗口
/// @param pfunclog 函数指针
/// @return 正常返回ST_OK,否则返回错误类型
ST_SDK_API st_result_t
st_mobile_set_log_callback(st_log_callback pfunclog);

/// \~chinese
/// @brief 开启debug模式，并输出调试信息、文件等到传入的debug文件夹路径。默认关闭，非线程安全。
/// @param[in] p_debug_path 存放debug信息、文件的文件夹路径
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief enable debug mode, SDK would output debug info & files to provided folder.
/// @param[in] p_debug_path folder path to which debug info & files would be dump.
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_enable_debug_mode(const char* p_debug_path);

/// \~chinese
/// @brief 关闭debug模式，停止输出调试信息、文件等，非线程安全。
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief disable debug mode, stop output debug info & files.
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_disable_debug_mode();

typedef enum {
    ST_AVATAR_EYE_RIGHT_CLOSE,                          // 00右眼闭合
    ST_AVATAR_EYE_RIGHT_DOWN,                           // 01右眼下看
    ST_AVATAR_EYE_RIGHT_INWARD,                         // 02右眼向内看(向左看)
    ST_AVATAR_EYE_RIGHT_OUTWARD,                        // 03右眼向外看(向右看)
    ST_AVATAR_EYE_RIGHT_UP,                             // 04右眼向上看
    ST_AVATAR_EYE_RIGHT_NARROW,                         // 05右眼眯眼
    ST_AVATAR_EYE_RIGHT_WIDE,                           // 06右眼圆睁
    ST_AVATAR_EYE_LEFT_CLOSE,                           // 07左眼闭合
    ST_AVATAR_EYE_LEFT_DOWN,                            // 08左眼下看
    ST_AVATAR_EYE_LEFT_INWARD,                          // 09左眼向内看(向右看)
    ST_AVATAR_EYE_LEFT_OUTWARD,                         // 10左眼向外看(向左看)
    ST_AVATAR_EYE_LEFT_UP,                              // 11左眼上看
    ST_AVATAR_EYE_LEFT_NARROW,                          // 12左眼眯眼
    ST_AVATAR_EYE_LEFT_WIDE,                            // 13左眼圆睁
    ST_AVATAR_JAW_FORWARD,                              // 14下颚前突(嘴闭合)
    ST_AVATAR_JAW_RIGHT,                                // 15下颚右移(嘴闭合)
    ST_AVATAR_JAW_LEFT,                                 // 16下颚左移(嘴闭合)
    ST_AVATAR_JAW_OPEN,                                 // 17下颚向下张开(嘴自然张开)
    ST_AVATAR_MOUTH_CLOSE,                              // 18嘴形自然闭合(下颚向下张开)
    ST_AVATAR_MOUTH_ROUND,                              // 19嘟嘴,嘴唇往前突
    ST_AVATAR_MOUTH_PUCKER,                             // 20撅嘴,嘴唇往外翘
    ST_AVATAR_MOUTH_BOTH_LIP_RIGHT,                     // 21上下嘴唇右移
    ST_AVATAR_MOUTH_BOTH_LIP_LEFT,                      // 22上下嘴唇左移
    ST_AVATAR_MOUTH_RIGHT_CORNER_UP,                    // 23右嘴角向上扬
    ST_AVATAR_MOUTH_LEFT_CORNER_UP,                     // 24左嘴角向上扬
    ST_AVATAR_MOUTH_RIGHT_CORNER_DOWN,                  // 25右嘴角向下撇
    ST_AVATAR_MOUTH_LEFT_CORNER_DOWN,                   // 26左嘴角向下撇
    ST_AVATAR_MOUTH_RIGHT_CORNER_BACKWARD,              // 27右嘴角向后撇
    ST_AVATAR_MOUTH_LEFT_CORNER_BACKWARD,               // 28左嘴角向后撇
    ST_AVATAR_MOUTH_RIGHT_CORNER_OUTWARD,               // 29右嘴角水平向外移(右移)
    ST_AVATAR_MOUTH_LEFT_CORNER_OUTWARD,                // 30左嘴角水平向外移(左移)
    ST_AVATAR_MOUTH_LOWER_LIP_INWARD,                   // 31下嘴唇内卷
    ST_AVATAR_MOUTH_UPPER_LIP_INWARD,                   // 32上嘴唇内卷
    ST_AVATAR_MOUTH_LOWER_LIP_OUTWARD,                  // 33下嘴唇外翻
    ST_AVATAR_MOUTH_UPPER_LIP_OUTWARD,                  // 34上嘴唇外翻
    ST_AVATAR_MOUTH_LOWER_LIP_RIGHT_UP,                 // 35下嘴唇右上翘
    ST_AVATAR_MOUTH_LOWER_LIP_LEFT_UP,                  // 36下嘴唇左上翘
    ST_AVATAR_MOUTH_LOWER_LIP_RIGHT_DOWN,               // 37下嘴唇右下垂
    ST_AVATAR_MOUTH_LOWER_LIP_LEFT_DOWN,                // 38下嘴唇左下垂
    ST_AVATAR_MOUTH_UPPER_LIP_RIGHT_UP,                 // 39上嘴唇右上翘
    ST_AVATAR_MOUTH_UPPER_LIP_LEFT_UP,                  // 40上嘴唇左上翘
    ST_AVATAR_MOUTH_LIP_PART,                           // 41微张嘴
    ST_AVATAR_EYEBROW_RIGHT_OUTER_DOWN,                 // 42右眉毛外垂
    ST_AVATAR_EYEBROW_LEFT_OUTER_DOWN,                  // 43左眉毛外垂
    ST_AVATAR_EYEBROW_BOTH_INNER_UP,                    // 44双眉向上内挑
    ST_AVATAR_EYEBROW_RIGHT_OUTER_UP,                   // 45右眉外挑
    ST_AVATAR_EYEBROW_LEFT_OUTER_UP,                    // 46左眉外挑
    ST_AVATAR_CHEEK_BOTH_OUTWARD,                       // 47双面颊前突
    ST_AVATAR_CHEEK_RIGHT_UP,                           // 48右面颊上提
    ST_AVATAR_CHEEK_LEFT_UP,                            // 49左面颊上提
    ST_AVATAR_NOSE_RIGHT_UP,                            // 50右鼻子上提
    ST_AVATAR_NOSE_LEFT_UP,                             // 51左鼻子上提
    ST_AVATAR_TONGUE_OUTWARD,                           // 52舌头伸出
    ST_AVATAR_EXPRESSION_NUM,
} ST_AVATAR_EXPRESSION_INDEX;


/// \~chinese @brief 设置snpe相关库在手机上的绝对路径
/// \~english @brief Set the absolute path of the snpe library on the mobile device
ST_SDK_API bool
st_mobile_set_snpe_library_path(const char* path);

/// \~chinese @brief 设置自定义OpenGL实现动态库（目前只支持Windows平台）的路径，只需调用一次，应该保证在调用渲染接口前先调用才能生效
/// \~english @brief Set the path for a custom OpenGL implementation dynamic library (currently only supported on Windows platform). This should be called only once before rendering APIs
ST_SDK_API st_result_t
st_mobile_set_external_opengl_lib_path(const char* path);

/// @}

/// \~chinese @brief 获取sdk版本号
/// \~english @brief Get the SDK version
ST_SDK_API
const char* st_mobile_get_version();

/// \~chinese
/// @brief 获取模型有效期
/// @param [in] path 待验证的模型文件的绝对路径
/// @param [out] start_date 时间限制的起始点
/// @param [out] end_date 时间限制的结束点
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Get the expiration date of a model
/// @param [in] path The absolute path of the model file to be validated
/// @param [out] start_date The start date of the time restriction
/// @param [out] end_date The end date of the time restriction
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API
st_result_t
st_mobile_get_model_time_limit(const char* path, unsigned int* start_date, unsigned int* end_date);

/// \~chinese
/// @brief 获取模型有效期
/// @param [in] buffer  待验证的模型缓存
/// @param [in] buffer_size 缓存大小
/// @param [out] start_date 时间限制的起始点
/// @param [out] end_date 时间限制的结束点
/// @return 正常返回ST_OK,否则返回错误类型
/// \~english
/// @brief Get the expiration date of a model
/// @param [in] buffer The buffer of the model to be validated
/// @param [in] buffer_size Length of the buffer
/// @param [out] start_date The start date of the time restriction
/// @param [out] end_date The end date of the time restriction
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API
st_result_t
st_mobile_get_model_time_limit_from_buffer(const unsigned char* buffer, unsigned int buffer_size, unsigned int* start_date, unsigned int* end_date);

/// \~chinese
/// @brief 获取标准人脸图
/// @param[in] img_in 输入人脸图像 只支持3通道和4通道,不支持带padding的图像
/// @param[in] p_face 输入人脸关键点
/// @param[in] standard_len 要求的标准人脸长度, 目前只支持512
/// @param[out] crop_img 抠出的标准人脸, 内存由上层分配, 按照格式分配512*512*3 或者512*512*4
/// \~english
/// @brief Get standardized face image
/// @param[in] img_in Input face image. Only supports 3-channel and 4-channel images without padding
/// @param[in] p_face Input face keypoints
/// @param[in] standard_len Required length of the standardized face. Currently only supports 512
/// @param[out] crop_img Cropped standardized face image in the size of 512*512*3 or 512*512*4. Memory is allocated by user
ST_SDK_API st_result_t
st_mobile_crop_standard_face(
    const st_image_t* img_in,
    const st_mobile_face_t* p_face,
    int standard_len,
    st_image_t *crop_img
    );

/// \~chinese
/// @brief 获取人脸图(不是标准的,仅做裁剪和缩放)
/// @param[in] img_in 输入人脸图像 只支持3通道和4通道,不支持带padding的图像
/// @param[in] p_face 输入人脸关键点
/// @param[out] crop_img 抠出的标准人脸, data内存由底层分配, 宽高不固定, 最大宽高为1024;一定注意释放内存
/// \~english
/// @brief Get face image (non-standard, only for cropping and scaling)
/// @param[in] img_in Input face image. Only supports 3-channel and 4-channel images without padding
/// @param[in] p_face Input face 106 keypoints
/// @param[out] crop_img Cropped face image. Memory is allocated by user. The width and height are not fixed and can be up to 1024. Make sure to release the memory properly
ST_SDK_API st_result_t
st_mobile_crop_face(
    const st_image_t* img_in,
    const st_mobile_face_t* p_face,
    st_image_t *crop_img
    );

/// \~chinese
/// @brief 计算边框图和效果图贴回目标图后的坐标，图像坐标系以左上角为坐标原点，topleft:(0,0) topright:(w,0) bottomleft:(0,h) bottomright:(w,h)
/// @param[in] src_imgs 待变换图像数组,仅需要width 和 height 属性
/// @param[in] src_img_num 待变换图像数组长度
/// @param[in] tar_img 目标图,仅需要width 和 height属性
/// @param[in] src_face 待变换图像人脸检测结果
/// @param[in] tar_face 目标图像人脸检测结果
/// @param[out] dst_coords 变换后图像在目标图中的顶点坐标，顺序与src_imgs一一对应,点序按照topleft,topright,bottomleft,borromright排列
/// \~english
/// @brief Calculate the coordinates of the bounding box of image and pasted back onto the target image. The image coordinate system has its origin at the top-left corner, with the following coordinates: topleft: (0,0), topright: (w,0), bottomleft: (0,h), bottomright: (w,h)
/// @param[in] src_imgs Array of source images to be transformed
/// @param[in] src_img_num Length of the source image array
/// @param[in] tar_img Target image
/// @param[in] src_face Face detection results of the source images
/// @param[in] tar_face Face detection results of the target image
/// @param[out] dst_coords Vertex coordinates of the transformed images in the target image, corresponding one-to-one with src_imgs, arranged in the order of topleft, topright, bottomleft, bottomright
ST_SDK_API st_result_t
st_mobile_paste_face_back_coords(
    const st_image_t* src_imgs,
    int src_img_num,
    const st_image_t* tar_img,
    const st_mobile_face_t* src_face,
    const st_mobile_face_t* tar_face,
    st_pointf_t (*dst_coords)[4]
);

/// \~chinese @brief 释放底层分配的图像数据
/// \~english @brief Release internal image buffer
ST_SDK_API void
st_mobile_release_image(st_image_t * img);

#endif // INCLUDE_STMOBILE_ST_MOBILE_COMMON_H_
