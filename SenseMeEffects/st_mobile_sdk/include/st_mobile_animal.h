/**
*@file st_mobile_animal.h

* \~chinese
* 提供动物脸的检测功能，支持预览、视频、图片模式，能够跟踪猫脸和狗脸，获取脸部特征点，并提供跟踪结果的旋转、缩放和镜像变换
*@attention
* 该文件中的API不保证线程安全.多线程调用时,需要确保安全调用.
* 例如在 detect/reset 没有执行完就执行 destroy 可能造成crash;
* 在 detect 执行过程中调用 reset 函数可能会造成crash.

* 一般调用步骤：创建句柄->加载模型->跟踪检测->销毁句柄

* \~english
* Provides animal face detection functionality that supports preview, video, and image modes. It is capable of tracking cat faces and dog faces, and extracting facial landmarks. Additionally, it offers rotation, scaling, and mirroring transformations for the tracked results.
*@attention
* The APIs in this file do not guarantee thread safety. When calling them in a multi-threaded environment, it is important to ensure safe usage.
* For example, calling destroy before detect or reset has completed may result in a crash.
* Calling the reset function during the execution of detect may also lead to a crash.

* General pipeline: Create a handle -> Add models -> Detect features -> Destroy the handle.

* st_mobile_tracker_animal_face_create

* st_mobile_tracker_animal_face_add_sub_model

* st_mobile_tracker_animal_face_track

* st_mobile_tracker_animal_face_destroy

**/
#ifndef INCLUDE_STMOBILE_ST_MOBILE_ANIMAL_H_
#define INCLUDE_STMOBILE_ST_MOBILE_ANIMAL_H_

#include "st_mobile_common.h"
#include "st_mobile_struct.h"


/// \~chinese @brief 动物类型
/// \~english @brief Animal type
typedef enum {
	ST_MOBILE_ANIMAL_CAT_FACE = 0,         ///< \~chinese 猫脸 \~english Cat face
	ST_MOBILE_ANIMAL_DOG_FACE= 10,         ///< \~chinese 狗脸 \~english Dog face
} st_mobile_animal_type;

/// \~chinese @brief 动物面部检测结果
/// \~english @brief Animal face detection results
typedef struct st_mobile_animal_face_t {
	int id;                 ///< \~chinese 每个检测到的脸拥有唯一的ID.跟踪丢失以后重新被检测到,会有一个新的ID \~english Each detected face has a unique ID. When a face is lost and later re-detected, it will have a new ID
	st_rect_t rect;         ///< \~chinese 代表面部的矩形区域 \~english A rectangular region of an animal face
	float score;            ///< \~chinese 置信度 \~english Confidence score
	st_pointf_t *p_key_points;  ///< \~chinese 关键点 \~english Keypoints of the face
	int key_points_count;       ///< \~chinese 关键点个数 \~english The number of keypoints
	float ear_score[2];     ///< \~chinese 耳朵关键点 只有狗脸支持 \~english Ear keypoints, only supported for dog faces
	float yaw;              ///< \~chinese 水平转角,真实度量的左负右正 \~english Yaw angle in real measurements, negative for left and positive for right
	float pitch;            ///< \~chinese 俯仰角,真实度量的上负下正 \~english Pitch angle in real measurements, negative for up and positive for down
	float roll;             ///< \~chinese 旋转角,真实度量的左负右正 \~english The rotation around its vertical axis in real measurements, negative for left and positive for right
	st_mobile_animal_type animal_type;  ///< \~chinese 动物类型 \~english Type of animal, cat or dog
} st_mobile_animal_face_t, *p_st_mobile_animal_face_t;

/// \~chinese @brief 动物检测结果
/// \~english @brief Animal detection results
typedef struct st_mobile_animal_result_t
{
	st_mobile_animal_face_t* p_animal_face_array;  // \~chinese 检测到的动物脸数组 \~english Array of detected animal faces
	int count;                                     // \~chinese 动物脸个数 \~english The number of detected animal faces
	st_mobile_segment_t* p_animal_segments;        // 动物脸分割
	int segment_count;                             // 分割个数
	unsigned char* p_extra_buffer;                 // \~chinese 额外信息 \~english Additional information related to the animal
	int extra_buffer_length;
}st_mobile_animal_result_t, *p_st_mobile_animal_result_t;

/// \~chinese
/// @brief  创建实时动物脸关键点跟踪句柄
/// @param[in] model_path 模型文件的绝对路径或相对路径,例如models/cat.model 可以是null, 为NULL时需要调用st_mobile_animal_face_add_sub_model添加模型
/// @param[in] config 配置选项 预览时使用ST_MOBILE_DETECT_MODE_PREVIEW, 离线视频处理使用ST_MOBILE_DETECT_MODE_VIDEO_POST_PROCESS， 图片使用ST_MOBILE_DETECT_MODE_IMAGE
/// @param[out] handle 动物脸跟踪句柄,失败返回NULL
/// @return 成功返回ST_OK, 失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Create a handle for real-time animal face keypoint tracking
/// @param[in] model_path Absolute or relative path to the model file. set to NULL when calling st_mobile_animal_face_add_sub_model to add the required models
/// @param[in] config configuration mode
/// @param[out] handle The handle for animal face tracking, or NULL if creation fails
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_create(
	const char *model_path,
	unsigned int config,
	st_handle_t *handle
);

/// \~chinese
/// @brief 创建实时动物脸关键点跟踪句柄
/// @param[in] buffer 模型缓存起始地址,为NULL时需要调用st_mobile_animal_face_add_sub_model添加需要的模型
/// @param[in] buffer_size 模型缓存大小
/// @param[in] config 配置选项 预览时使用ST_MOBILE_DETECT_MODE_PREVIEW, 离线视频处理使用ST_MOBILE_DETECT_MODE_VIDEO_POST_PROCESS， 图片使用ST_MOBILE_DETECT_MODE_IMAGE
/// @param[out] handle 动物脸跟踪句柄,失败返回NULL
/// @return 成功返回ST_OK, 失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Create a handle for real-time animal face keypoint tracking
/// @param[in] buffer Pointer to the model buffer. set to NULL when calling st_mobile_animal_face_add_sub_model to add the required models
/// @param[in] buffer_size Length of the model buffer
/// @param[in] config Configuration mode, use ST_MOBILE_DETECT_MODE_PREVIEW for preview, ST_MOBILE_DETECT_MODE_VIDEO_POST_PROCESS for offline video processing, and ST_MOBILE_DETECT_MODE_IMAGE for images
/// @param[out] handle The handle for animal face tracking, or NULL if creation fails
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_create_from_buffer(
    const unsigned char* buffer,
    unsigned int buffer_size,
    unsigned int config,
    st_handle_t *handle
);

/// \~chinese
/// @brief 添加子模型. Android建议使用st_mobile_animal_face_add_sub_model_from_buffer
/// @param[in] handle 动物脸跟踪句柄
/// @param[in] model_path 模型文件的路径. 后添加的会覆盖之前添加的同类子模型。加载模型耗时较长, 建议在初始化创建句柄时就加载模型
/// \~english
/// @brief Add a sub-model for animal face detection. For Android, it is recommended to use st_mobile_animal_face_add_sub_model_from_buffer
/// @param[in] handle  The handle for animal face tracking
/// @param[in] model_path Path to the model file. Sub-models added later will overwrite previously added sub-models of the same type. Loading models can be time-consuming, so it is recommended to load the models during the creation of the handle
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_add_sub_model(
    st_handle_t handle,
    const char *model_path
);

/// \~chinese
/// @brief 添加子模型.
/// @param[in] handle 动物脸跟踪句柄
/// @param[in] buffer 模型缓存起始地址
/// @param[in] buffer_size 模型缓存大小
/// \~english
/// @brief Add a sub-model
/// @param[in] handle The handle for animal face tracking
/// @param[in] buffer Pointer to the model buffer
/// @param[in] buffer_size Length of the model buffer
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_add_sub_model_from_buffer(
    st_handle_t handle,
    const unsigned char* buffer,
    unsigned int buffer_size
);
/// \~chinese @brief 动物脸检测模型类型
/// \~english @brief Animal face detection model type, used for removing models
typedef enum {
	ST_MOBILE_ANIMAL_MODEL_CAT_FACE = 0,         ///< \~chinese 猫脸 \~english Cat face
	ST_MOBILE_ANIMAL_MODEL_DOG_FACE= 10,         ///< \~chinese 狗脸 \~english Dog face
	ST_MOBILE_ANIMAL_MODEL_SEGMENT = 20,         ///动物分割, animal segment
} st_mobile_animal_model_type;

/// \~chinese
/// @brief 删除子模型.
/// @param[in] handle 动物脸跟踪句柄
/// @param[in] model_type 删除对应的模型,可用来减少内存
/// @return  成功返回ST_OK， 失败返回其他错误码
/// \~english
/// @brief Remove a previously added sub-model to reduce memory usage
/// @param[in] handle The handle for animal face tracking
/// @param[in] model_type The model type of the corresponding sub-model to be removed
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API
st_result_t st_mobile_tracker_animal_face_remove_model_by_config(
	st_handle_t handle,
	st_mobile_animal_model_type model_type
	);

/// \~chinese @brief 动物脸检测参数类型
/// \~english @brief Animal face detection parameter types
typedef enum {
	/// \~chinese 设置检测到的最大猫脸数目N,持续track已检测到的N个猫脸直到猫脸数小于N再继续做detect.默认32 \~english Set the maximum number of cat faces detected to N. Continue tracking the already detected N cat faces until the number of cat faces is less than N before performing further detection. The default value is 32
	ST_MOBILE_PARAM_CAT_LIMIT = 1,
	/// \~chinese 设置tracker每多少帧进行一次猫脸detect. \~english Set the frequency at which the tracker performs cat face detection
	ST_MOBILE_PARAM_CAT_DETECT_INTERVAL_LIMIT = 2,
	/// \~chinese 设置猫脸跟踪的阈值 \~english Set the threshold for cat face tracking to adjust the sensitivity of the cat face tracking algorithm. A lower threshold may result in more accurate tracking but may also increase the likelihood of false positives
	ST_MOBILE_PARAM_CAT_THRESHOLD = 3,
	/// \~chinese 设置检测结果延迟帧数[0,2],检测接口输出结果为(当前-N)帧的结果, 默认0不延迟.  注意：在该参数修改过程中,当前检测结果会清空重新开始检测 \~english Set the number of frames to delay the detection results. The detection interface outputs results from the (current - N) frame. The default value is 0, which means no delay. Note that modifying this parameter will clear the current results and start the detection process again
	ST_MOBILE_PARAM_ANIMAL_DELAY_FRAME = 4,
	/// \~chinese 设置检测到的最大狗脸数目N,持续track已检测到的N个狗脸直到狗脸数小于N再继续做detect.默认32 \~english Set the maximum number of dog faces detected to N. Continue tracking the already detected N dog faces until the number of dog faces is less than N before performing further detection. The default value is 32
	ST_MOBILE_PARAM_DOG_LIMIT = 101,
	/// \~chinese 设置tracker每多少帧进行一次狗脸detect. \~english Set the frequency at which the tracker performs dog face detection
	ST_MOBILE_PARAM_DOG_DETECT_INTERVAL_LIMIT = 102,
	/// \~chinese 设置狗脸跟踪的阈值 \~english Set the threshold for dog face tracking to adjust the sensitivity of the dog face tracking algorithm
	ST_MOBILE_PARAM_DOG_THRESHOLD = 103,

	/// \~chinese 设置预处理图像的最长边，默认为原图大小。 值越大，耗时越长，检测到的数目会多一些 \~english Set the maximum length of the preprocessed image(longest side), a larger value may increase the number of detected animal faces but longer processing time
	ST_MOBILE_PARAM_ANIMAL_PREPROCESS_MAX_SIZE = 500,

	// 设置分割结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转)
	ST_MOBILE_PARAM_ANIMAL_SEGMENT_ROTATE = 601,

	// 设置分割检测结果边缘模糊程度 取值范围0-1 视频版默认是0.5，图片版默认是1
	ST_MOBILE_PARAM_ANIMAL_SEGMENT_RESULT_BLUR = 602

} st_animal_face_param_type;

/// \~chinese
/// @brief 设置参数
/// @param[in] handle 已初始化的动物脸跟踪句柄
/// @param[in] type 参数关键字,例如ST_MOBILE_CAT_LIMIT
/// @param[in] value 参数取值
/// @return 成功返回ST_OK,错误则返回错误码,错误码定义在st_mobile_common.h 中,如ST_E_FAIL等
/// \~english
/// @brief  Set the animal face detection parameters
/// @param[in] handle The initialized animal face tracking handle
/// @param[in] type Parameter type to be set, such as ST_MOBILE_CAT_LIMIT
/// @param[in] value The value to set for the parameter
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_setparam(
	st_handle_t handle,
	st_animal_face_param_type type,
	float value
);

/// \~chinese
/// @brief  重置实时动物脸关键点跟踪,清空track造成的缓存,当切换分辨率、切换前后摄像头、切换视频、两帧图像差别较大时建议调用reset
/// @param [in] handle 已初始化的实时目标动物脸关键点跟踪句柄
/// @return  成功返回ST_OK, 失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief  Reset the real-time animal face tracking hdanle and clear the cache. It is recommended to call this function when switching resolutions, switching between front and rear cameras, switching videos, or when there is a significant difference between two consecutive frames
/// @param [in] handle The initialized animal face tracking handle
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_reset(
	st_handle_t handle
);

/// \~chinese @brief 检测类型定义
/// \~english @brief Enumeration of detection types
#define ST_MOBILE_CAT_DETECT		0x00000001  ///< \~chinese 猫脸检测 \~english Cat detection
#define ST_MOBILE_DOG_DETECT        0x00000010  ///< \~chinese 狗脸检测 \~english Dog detection
#define ST_MOBILE_SEG_ANIMAL        0x00000020  ///< \~chinese 猫狗脸分割

/// \~chinese
/// @brief 对连续视频帧进行实时快速动物脸关键点跟踪
/// @param[in] handle 已初始化的实时动物脸跟踪句柄
/// @param[in] image 用于检测的图像数据
/// @param[in] pixel_format 用于检测的图像数据的像素格式. 推荐使用GRAY8、NV12、NV21、YUV420P
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数；目前仅支持字节对齐的padding,不支持roi
/// @param[in] orientation 视频中动物脸的方向
/// @param[in] detect_config 需要检测的类型
/// @param[out] p_animal_result  检测到的动物脸信息数组,api负责管理内存,会覆盖上一次调用获取到的数据
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// @brief Perform real-time and efficient animal face keypoint tracking on consecutive frames
/// @param[in] handle The initialized animal face tracking handle
/// @param[in] image Input image data for detection
/// @param[in] pixel_format Pixel format of the image data used for detection, recommended formats include GRAY8, NV12, NV21, and YUV420P
/// @param[in] image_width Width of the image used for detection, in pixels
/// @param[in] image_height Height of the image, in pixels
/// @param[in] image_stride Stride of the image data, in bytes per row. only byte-aligned padding is supported, and region of interest (ROI) is not supported currently
/// @param[in] orientation  Orientation of the animal face in the video
/// @param[in] detect_config Detection config
/// @param[out] p_animal_result Array of detected animal face information. the API manages the memory and will overwrite the memory
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_track(
	st_handle_t handle,
	const unsigned char *image,
	st_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	st_rotate_type orientation,
	unsigned int detect_config,
	st_mobile_animal_result_t* p_animal_result
);

/// \~chinese
/// @brief  动物脸跟踪(输入为多平面图像)
/// @param[in] handle 已初始化的动物脸跟踪句柄
/// @param[in] image 用于检测的图像数据 只支持yuv，nv12,nv21格式
/// @param[in] orientation 图像中动物脸的方向
/// @param[in] detect_config  需要检测的类型
/// @param[out] p_animal_face  检测到的动物脸信息数组,由用户分配内存. 会覆盖上一次的检测结果
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Animal face tracking (input as a multi-plane image)
/// @param[in] handle The initialized animal face tracking handle
/// @param[in] image Input image data used for detection. only YUV, NV12, and NV21 formats are supported
/// @param[in] orientation Orientation of the animal face in the image
/// @param[in] detect_config Detection config
/// @param[out] p_animal_face Array of detected animal face information. The memory for the array should be allocated by the user. It will overwrite the previous detection results
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_tracker_animal_face_track_from_multi_plane_image(
    st_handle_t handle,
    const st_multiplane_image_t* image,
    st_rotate_type orientation,
	unsigned int detect_config,
	st_mobile_animal_result_t* p_animal_result
	);

/// \~chinese
/// @brief 销毁已初始化的动物脸跟踪句柄
/// @param[in] handle 已初始化的动物脸跟踪句柄
/// \~english
/// @brief Destroy the initialized animal face tracking handle
/// @param[in] handle The initialized animal face tracking handle
ST_SDK_API void
st_mobile_tracker_animal_face_destroy(
	st_handle_t handle
);

/// \~chinese
/// @brief 镜像animal检测结果.
/// @param[in] image_width 检测animal_face的图像的宽度(以像素为单位)
/// @param[in,out] p_animal_result 需要镜像的animal_face检测结果
/// \~english
/// @brief Mirror the animal detection results
/// @param[in] image_width Width of the image used for animal face detection, in pixels
/// @param[in,out] p_animal_result Animal face detection results to be mirrored
ST_SDK_API void
st_mobile_animal_face_mirror(
	int image_width,
	st_mobile_animal_result_t* p_animal_result
);

/// \~chinese
/// @brief 旋转animal_face检测结果
/// @param[in] image_width 检测animal_face的图像的宽度(以像素为单位)
/// @param[in] image_height 检测animal_face的图像的宽度(以像素为单位)
/// @param[in] orientation 顺时针旋转的角度
/// @param[in,out] p_result 需要旋转的animal_face检测结果
/// \~english
/// @brief  Rotate the animal detection results
/// @param[in] image_width Width of the image used for animal face detection, in pixels
/// @param[in] image_height Height of the image used for animal face detection, in pixels
/// @param[in] orientation Angle of clockwise rotation
/// @param[in,out] p_result Animal face detection results to be rotated
ST_SDK_API void
st_mobile_animal_face_rotate(
	int image_width,
	int image_height,
	st_rotate_type orientation,
	st_mobile_animal_result_t* p_animal_result
);

/// \~chinese
/// @brief  放大/缩小animal_face检测结果
/// @param[in] scale 缩放的尺度
/// @param[in,out] p_result 需要缩放的animal_face检测结果
/// \~english
/// @brief Scale up/down the animal face detection results
/// @param[in] scale Scale factor for resizing
/// @param[in,out] p_result Animal face detection results to be scaled
ST_SDK_API
void st_mobile_animal_face_resize(
	float scale,
	st_mobile_animal_result_t* p_animal_result
);

/// \~chinese
/// @brief 拷贝animal_face检测结果.
/// @param[in] p_animal_src  需要拷贝的animal_face检测结果
/// @param[out] p_animal_dst 需要拷贝的animal_face检测结果.注意初始化成员,如果指针非空, 默认是已被分配过内存, 不会重新分配内存
/// \~english
/// @brief Copy the animal face detection results
/// @param[in] p_animal_src Animal face detection results to be copyed
/// @param[out] p_animal_dst Copied animal face detection results, with initialized members. If the pointer is not empty, it is assumed that memory has already been allocated and will not be reallocated
ST_SDK_API void
st_mobile_animal_face_copy(
	const st_mobile_animal_result_t* p_animal_src,
	st_mobile_animal_result_t* p_animal_dst
);

/// \~chinese
/// @brief 删除animal_face 结果, 只能删除st_mobile_animal_face_copy的输出结果
/// @param[in] p_result 需要删除的animal_face检测结果
/// \~english
/// @brief Delete animal face results. Only results copyed from st_mobile_animal_face_copy can be deleted
/// @param[in] p_result Animal face detection results to be deleted
ST_SDK_API
void st_mobile_animal_face_delete(
	st_mobile_animal_result_t* p_animal_result
);

#endif // INCLUDE_STMOBILE_ST_MOBILE_ANIMAL_H_
