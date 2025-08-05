/**
*@file st_mobile_human_action.h

* \~chinese
* 提供人体相关的特征的检测，包括头部、面部、耳朵、嘴巴、眼睛等人体部位及肢体动作的检测与识别
* 同时提供检测结果的旋转、缩放、拷贝、镜像等功能
* 支持预览、视频、图片处理 三种模式

* @attention
* 该文件中的API不保证线程安全.多线程调用时,需要确保安全调用.例如在 detect/reset 没有执行完就执行 process 可能造成crash;在 detect 执行过程中调用 reset 函数可能会造成crash.

* 一般调用步骤：创建句柄->加载模型->获取相关数据(不是必须）->检测特征->销毁句柄

* \~english
* Provides detection of human-related features, including head, face, ears, mouth, eyes, and other body parts, as well as detection and recognition of body movements.
* Also provides functionality for rotating, scaling, copying, and mirroring the detection results.
* Supports three modes: preview, video, and image processing, for handling real-time previews, video streams, and static images, respectively.

* @attention
* The APIs in this file are not guaranteed to be thread-safe. When calling them in a multi-threaded environment, it is necessary to ensure safe usage. For example, calling process before detect/reset has completed may result in a crash. Calling reset during the execution of detect may also cause a crash.

* General pipeline: Create a handle -> Add models -> Retrieve relevant data (if necessary) -> Detect features -> Destroy the handle.

* st_mobile_human_action_create

* st_mobile_human_action_add_sub_model

* st_mobile_human_action_get_face_mesh_list

* st_mobile_human_action_detect

* st_mobile_human_action_destroy

**/
#ifndef INCLUDE_STMOBILE_ST_MOBILE_HUMAN_ACTION_H_
#define INCLUDE_STMOBILE_ST_MOBILE_HUMAN_ACTION_H_

#include "./st_mobile_common.h"
#include "./st_mobile_struct.h"

/// \~chinese 用于detect_config配置选项, 每一位均表示开启/关闭一个检测选项，部分也可用来判断检测结果中的human_action动作类型 \~english The detect_config configurations are used to enable or disable specific detection options. Each bit in the configuration represents the status of a detection option, and some bits can also be used to determine the human_action action types in the detection results.
#define ST_MOBILE_FACE_DETECT               0x00000001  ///< \~chinese 人脸检测 \~english Detect faces
#define ST_MOBILE_EYE_BLINK                 0x00000002  ///< \~chinese 眨眼 \~english Blink eye
#define ST_MOBILE_MOUTH_AH                  0x00000004  ///< \~chinese 嘴巴大张 \~english Open wide
#define ST_MOBILE_HEAD_YAW                  0x00000008  ///< \~chinese 摇头 \~english Shake head
#define ST_MOBILE_HEAD_PITCH                0x00000010  ///< \~chinese 点头 \~english Nod head
#define ST_MOBILE_BROW_JUMP                 0x00000020  ///< \~chinese 眉毛挑动, 该检测耗时较长，请在需要时开启 \~english Raise eyebrow. This detection is time-consuming, please enable it only when necessary）
#define ST_MOBILE_FACE_LIPS_UPWARD          0x00000040  ///< \~chinese 嘴角上扬 \~english Upward lips
#define ST_MOBILE_FACE_LIPS_POUTED          0x00000080  ///< \~chinese 嘟嘴 \~english Pouted lips
#define ST_MOBILE_HAND_DETECT               0x00000100  ///< \~chinese 检测手 \~english Detect hands
#define ST_MOBILE_DETECT_HAND_GESTURE       0x00000200  ///< \~chinese 检测手势类别 \~english Detect hand gesture
#define ST_MOBILE_DETECT_HAND_SSH           0x00000201  ///< \~chinese 特殊手势类别嘘, 需要手势和人脸一起判断 \~english Detect hand gesture
#define ST_MOBILE_SEG_BACKGROUND            0x00010000  ///< \~chinese 检测前景背景分割 \~english Detect foreground-background segmentation
#define ST_MOBILE_SEG_SALIENTOBJECT         0x00000400  ///< 显著性(前景)分割
#define ST_MOBILE_FACE_240_DETECT           0x01000000  ///< \~chinese 检测人脸240关键点 (deprecated) \~english Detect 240 face keypoints
#define ST_MOBILE_DETECT_EXTRA_FACE_POINTS  0x01000000  ///< \~chinese 检测人脸240关键点 \~english Detect 240 face keypoints
#define ST_MOBILE_DETECT_EYEBALL_CENTER     0x02000000  ///< \~chinese 检测眼球中心点 \~english Detect eyeball center points
#define ST_MOBILE_DETECT_EYEBALL_CONTOUR    0x04000000  ///< \~chinese 检测眼球轮廓点 \~english Detect eyeball contour points
#define ST_MOBILE_BODY_KEYPOINTS            0x08000000  ///< \~chinese 检测肢体关键点 \~english Detect body keypoints
#define ST_MOBILE_BODY_CONTOUR              0x10000000  ///< \~chinese 检测肢体轮廓点 \~english Detect body contour points
#define ST_MOBILE_SEG_HAIR                  0x20000000  ///< \~chinese 检测头发分割 \~english Detect hair segmentation
#define ST_MOBILE_DETECT_TONGUE             0x40000000  ///< \~chinese 检测舌头关键点 \~english Detect tongue keypoints
#define ST_MOBILE_SEG_HEAD                  0x0100000000    /// \~chinese 检测头部分割 \~english Detect head segmentation
#define ST_MOBILE_SEG_SKIN                  0x0200000000    /// \~chinese 检测皮肤分割 \~english Detect skin segmentation
#define ST_MOBILE_SEG_FACE_OCCLUSION        0x0400000000    /// \~chinese 检测面部遮挡分割 \~english Detect face occlusion segmentation
#define ST_MOBILE_DETECT_FOOT				0x0800000000	/// \~chinese 脚部关键点检测 \~english Detect foot keypoints
#define ST_MOBILE_BODY_ACTION5              0x1000000000    /// \~chinese 动感超人 暂时不支持 \~english Dynamic Superman, not supported currentlt
#define ST_MOBILE_SEG_TROUSER_LEG           0x1000000000    /// \~chinese 检测裤腿分割 \~english Detect trouser leg segmentation
#define ST_MOBILE_DETECT_FINGER             0x4000000000    /// \~chinese 检测手指关键点 \~english Detect finger keypoints
#define ST_MOBILE_DETECT_HAND_SKELETON_KEYPOINTS    0x20000000000  ///< \~chinese 检测单手关键点, 最多支持两只手的关键点检测 \~english Detect single hand keypoints, supports up to two hands
#define ST_MOBILE_DETECT_HAND_SKELETON_KEYPOINTS_3D 0x40000000000  ///< \~chinese 检测单手3d关键点 \~english Detect single hand 3D keypoints
#define ST_MOBILE_SEG_MULTI                         0x80000000000  ///< \~chinese 检测多类分割 \~english Detect multi-class segmentation
#define ST_MOBILE_DETECT_GAZE                       0x100000000000 ///< \~chinese 检测视线方向 \~english Detect gaze direction
#define ST_MOBILE_DETECT_DYNAMIC_GESTURE            0x200000000000 ///< \~chinese 检测动态手势 \~english Detect dynamic gestures
#define ST_MOBILE_DETECT_AVATAR_HELPINFO            0x800000000000 ///< \~chinese 检测avatar辅助信息 \~english Detect avatar information
#define ST_MOBILE_DETECT_FACE_S_COLOR               0x1000000000000 ///< \~chinese 依赖106关键点检测 \~english Detect face color
#define ST_MOBILE_DETECT_HAIR_COLOR                 0x100000000000000  ///< \~chinese avatar发色检测, 依赖106关键点和头发分割，目前只支持单人发色 \~english Detect hair color for avatar, relies on 106-point detection and hair segmentation, currently supports hair color detection for a single person
#define ST_MOBILE_BODY_KEYPOINTS_3D        0x2000000000000  ///< \~chinese 检测肢体3d关键点 \~english Detect 3D body keypoints
#define ST_MOBILE_DETECT_EAR               0x4000000000000  ///< \~chinese 检测耳朵关键点 \~english Detect ear keypoints
#define ST_MOBILE_DETECT_FOREHEAD          0x8000000000000  ///< \~chinese 检测额头关键点 \~english Detect forehead keypoints
#define ST_MOBILE_DETECT_FACE_MESH         0x40000000000000 ///< \~chinese 检测3dmesh关键点 \~english Detect 3D mesh keypoints
#define ST_MOBILE_DETECT_MOUTH_PARSE       0x80000000000000 ///< \~chinese 检测嘴部遮挡 \~english  Detect mouth occlusion
#define ST_MOBILE_DETECT_HEAD              0x200000000000000 ///< \~chinese 检测head关键点 \~english Detect head keypoints
#define ST_MOBILE_DETECT_HEAD_MESH         0x400000000000000 ///< \~chinese 检测head mesh关键点 \~english Detect head mesh keypoints
#define ST_MOBILE_DETECT_UPBODY_AVATAR     0x800000000000000 ///< \~chinese 检测半身avatar \~english Detect upper body avatars
#define ST_MOBILE_SEG_SKY                  0x1000000000000000 ///< \~chinese 检测天空分割 \~english Detect sky segmentation
#define ST_MOBILE_DEPTH_ESTIMATION         0X2000000000000000 ///< \~chinese 检测深度估计 \~english Estimates depth
#define ST_MOBILE_NAIL_DETECT              0x4000000000000000 ///< \~chinese 指甲关键点检测 \~english Detect nail keypoints
#define ST_MOBILE_SEG_CLOTH                0x000080000        ///< \~chinese 衣物检测 \~english Detect cloth segmentation
#define ST_MOBILE_SEG_GREEN                0x80000000         ///< \~chinese 绿幕分割 \~english Detect green screen segmentation
#define ST_MOBILE_WRIST_DETECT             0x8000000000000000 ///< \~chinese 手腕关键点检测 \~english Detect wrist keypoints
#define ST_MOBILE_DETECT_BODY_MESH         0x400000000000 ///< \~chinese 检测body mesh关键点 \~english Detect body mesh keypoints

/// \~chinese 以下是常见的检测detect_config \~english Here are some common detection configurations
#define ST_MOBILE_FACE_DETECT_FULL        0x000000FF        ///< \~chinese 检测所有脸部动作 \~english  Detects all facial features
#define ST_MOBILE_HAND_DETECT_FULL        0x00000300  ///< \~chinese 检测所有手势, 如果手势分类和手部骨骼点(2d/3d)的config同时打开时, 对于恭贺（抱拳)/双手合十/手势ILoveYou等组合手势只能检测出一个组合手势． \~english Detects all hand gestures. If both gesture classification and hand skeletal points (2D/3D) configurations are enabled, only one combined gesture, such as ST_MOBILE_HAND_CONGRATULATE (Fist-Palm Salute), ST_MOBILE_HAND_BLESS (Namaste, Praying Hands), or ST_MOBILE_HAND_ILOVEYOU, can be detected.
#define ST_MOBILE_BODY_DETECT_FULL        0x018000000       ///< \~chinese 检测肢体关键点和肢体轮廓点 \~english Detects body keypoints and body contour points

/// \~chinese 创建人体行为检测句柄的默认配置: 设置检测模式和检测类型 \~english Default configuration for creating a human action detection handle: sets the detection mode and type
/// \~chinese 旧版本视频检测, 检测人脸、手势和前后背景，效果同预览检测，后续会更新为预览检测 \~english Old version video detection, detects faces, hands, and foreground/background. Same as ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_PREVIEW
#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO                 ST_MOBILE_DETECT_MODE_VIDEO
/// \~chinese 预览检测, 检测人脸、手势和前后背景 \~english Preview detection, detects faces, hands, and foreground/background
#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_PREVIEW               ST_MOBILE_DETECT_MODE_PREVIEW
/// \~chinese 图片检测, 检测人脸、手势和前后背景 \~english Image detection, detects faces, hands, and foreground/background
#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_IMAGE                 ST_MOBILE_DETECT_MODE_IMAGE
/// \~chinese 视频后处理检测, 检测人脸、手势和前后背景 \~english Video post-processing detection, detects faces, hands, and foreground/background
#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO_POST_PROCESS    ST_MOBILE_DETECT_MODE_VIDEO_POST_PROCESS

/// \~chinese
/// @brief 创建人体行为检测句柄. Android建议使用st_mobile_human_action_create_from_buffer
/// @param[in] model_path 模型文件的路径,例如models/action.model. 为NULL时需要调用st_mobile_human_action_add_sub_model添加需要的模型
/// @param[in] config 配置选项 预览使用ST_MOBILE_DETECT_MODE_PREVIEW, 离线视频处理使用ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO_POST_PROCESS， 图片使用ST_MOBILE_DETECT_MODE_IMAG
/// @param[out] handle 人体行为检测句柄,失败返回NULL
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Create a human action detection handle. It is recommended to use st_mobile_human_action_create_from_buffer for Android
/// @param[in] model_path Path to the model file, e.g., "models/action.model". When set to NULL, you need to call st_mobile_human_action_add_sub_model to add the required models
/// @param[in] config Configuration mode: Use ST_MOBILE_DETECT_MODE_PREVIEW for preview mode, ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO_POST_PROCESS for offline video processing, or ST_MOBILE_DETECT_MODE_IMAGE for image mode
/// @param[out] handle The handle for human action detection, or NULL if creation failed
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_human_action_create(
    const char *model_path,
    unsigned int config,
    st_handle_t *handle
);

/// \~chinese
/// @brief 创建人体行为检测句柄
/// @param[in] buffer 模型缓存起始地址,为NULL时需要调用st_mobile_human_action_add_sub_model添加需要的模型
/// @param[in] buffer_size 模型缓存大小
/// @param[in] config 配置选项 预览使用ST_MOBILE_DETECT_MODE_PREVIEW, 离线视频处理使用ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO_POST_PROCESS， 图片使用ST_MOBILE_DETECT_MODE_IMAGE
/// @param[out] handle 人体行为检测句柄,失败返回NULL
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Create a human action detection handle
/// @param[in] buffer Pointer to the model buffer, when set to NULL, you need to call st_mobile_human_action_add_sub_model to add the required models
/// @param[in] buffer_size Length of the model buffer
/// @param[in] config Configuration mode: Use ST_MOBILE_DETECT_MODE_PREVIEW for preview mode, ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO_POST_PROCESS for offline video processing, or ST_MOBILE_DETECT_MODE_IMAGE for image mode
/// @param[out] handle The handle for human action detection, or NULL if creation failed
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_human_action_create_from_buffer(
    const unsigned char* buffer,
    unsigned int buffer_size,
	unsigned int config,
    st_handle_t *handle
);

/// \~chinese
/// @brief 通过子模型创建人体行为检测句柄, st_mobile_human_action_create和st_mobile_human_action_create_with_sub_models只能调一个
/// @param[in] model_path_arr 模型文件路径指针数组. 根据加载的子模型确定支持检测的类型. 如果包含相同的子模型, 后面的会覆盖前面的
/// @param[in] model_count 模型文件数目
/// @param[in] config 配置选项 预览使用ST_MOBILE_DETECT_MODE_PREVIEW, 离线视频处理使用ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO_POST_PROCESS， 图片使用ST_MOBILE_DETECT_MODE_IMAGE
/// @param[out] handle 人体行为检测句柄,失败返回NULL
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Create a human action detection handle using sub-models, either st_mobile_human_action_create or st_mobile_human_action_create_with_sub_models can be called, not both
/// @param[in] model_path_arr The model file paths array. The supported detection types are determined based on the added sub-models. If the same type of sub-model is loaded, the later one will override the previous one
/// @param[in] model_count Number of model files
/// @param[in] config Configuration mode: Use ST_MOBILE_DETECT_MODE_PREVIEW for preview mode, ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO_POST_PROCESS for offline video processing, or ST_MOBILE_DETECT_MODE_IMAGE for image mode
/// @param[out] handle \The handle for human action detection, or NULL if creation failed
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_human_action_create_with_sub_models(
    const char **model_path_arr,
    int model_count,
    unsigned int detect_mode,
    st_handle_t *handle
);

/// \~chinese
/// @brief 添加子模型. Android建议使用st_mobile_human_action_add_sub_model_from_buffer
/// @param[in] handle 人体行为检测句柄
/// @param[in] model_path 模型文件的路径. 后添加的会覆盖之前添加的同类子模型。加载模型耗时较长, 建议在初始化创建句柄时就加载模型
/// \~english
/// @brief Add a sub-model. It is recommended to use st_mobile_human_action_add_sub_model_from_buffer for Android
/// @param[in] handle The human action detection handle
/// @param[in] model_path Path to the model file. Sub-models added later will overwrite previously added sub-models of the same type. Loading models can be time-consuming, so it is recommended to load the models when creating the handle
ST_SDK_API st_result_t
st_mobile_human_action_add_sub_model(
    st_handle_t handle,
    const char *model_path
);
/// \~chinese
/// @brief 添加子模型.
/// @param[in] handle 人体行为检测句柄
/// @param[in] buffer 模型缓存起始地址
/// @param[in] buffer_size 模型缓存大小
/// \~english
/// @brief Add a sub-model
/// @param[in] handle The human action detection handle
/// @param[in] buffer The pointer to the model buffer
/// @param[in] buffer_size Length of the model buffer
ST_SDK_API st_result_t
st_mobile_human_action_add_sub_model_from_buffer(
    st_handle_t handle,
    const unsigned char* buffer,
    unsigned int buffer_size
);

/// \~chinese @brief 人脸形状
/// \~english @brief Face shape defination
typedef enum st_face_shape_t {
    ST_FACE_SHAPE_UNKNOWN,      ///< \~chinese 未知类型 \~english Unknown type
    ST_FACE_SHAPE_NATURAL,      ///< \~chinese 自然    \~english Natural
    ST_FACE_SHAPE_ROUND,        ///< \~chinese 圆脸    \~english Round face
    ST_FACE_SHAPE_SQUARE,       ///< \~chinese 方脸    \~english Square face
    ST_FACE_SHAPE_LONG,         ///< \~chinese 长脸    \~english Long face
    ST_FACE_SHAPE_RECTANGLE     ///< \~chinese 长形脸   \~english Bblong face
} st_face_shape_t;

/// \~chinese
/// @brief 获取人脸形状
/// @param[in] handle 已初始化的human_action句柄
/// @param[in] p_face 检测到的人脸信息
/// @param[out] p_face_shape 输出人脸形状结果
/// \~english
/// @brief Get the face shape
/// @param[in] handle The initialized human action handle
/// @param[in] p_face Detected face information
/// @param[out] p_face_shape The face shape result
ST_SDK_API st_result_t
st_mobile_human_action_get_face_shape(
    st_handle_t handle,
    const st_mobile_face_t *p_face,
    st_face_shape_t *p_face_shape
);

/// \~chinese @brief human action 所用的模型类别，仅用于根据模型类别删除模型
/// \~english @brief Human action model types , used for removing models
typedef enum {
	ST_MOBILE_MODEL_TYPE_FACE_106 = 0,                      ///< \~chinese 人脸106点 \~english Face 106 points
	ST_MOBILE_MODEL_TYPE_FACE_EXTRA= 1,                     ///< \~chinese 人脸240点 \~english Face 240 points
	ST_MOBILE_MODEL_TYPE_FACE_EYEBALL= 2,                   ///< \~chinese 眼球中心点 \~english Eyeball center points
	ST_MOBILE_MODEL_TYPE_FACE_TONGUE= 3,                    ///< \~chinese 舌头关键点 \~english Tongue keypoints
	ST_MOBILE_MODEL_TYPE_FACE_GAZE = 4,                     ///< \~chinese 视线方向 \~english Gaze direction
	ST_MOBILE_MODEL_TYPE_FACE_AVATAR_HELPER = 5,            ///< \~chinese 人脸avatar辅助信息开关 \~english Face avatar helper model
	ST_MOBILE_MODEL_TYPE_FACE_EAR = 6,                      ///< \~chinese 耳朵关键点 \~english  Ear keypoints
	ST_MOBILE_MODEL_TYPE_FACE_MESH = 7,                     ///< \~chinese 3dmesh关键点 \~english 3D mesh keypoints
	ST_MOBILE_MODEL_TYPE_AVARAE_HELPER = 8,                 ///< \~chinese avatar help 模型 \~english Avatar help model
	ST_MOBILE_MODEL_TYPE_FACE_POSE3D = 9,                   ///< \~chinese face pose 3d pose 模型 \~english Face pose 3D pose model

	ST_MOBILE_MODEL_TYPE_HAND_DETECT =100,                  ///< \~chinese 手势 \~english Hand gestures
	ST_MOBILE_MODEL_TYPE_HAND_SKELETON_KEYPOINTS_2D3D = 101,///< \~chinese 手势关节点 \~english Hand skeleton keypoints
	ST_MOBILE_MODEL_TYPE_HAND_DYNAMIC_GESTURE = 103,        ///< \~chinese 动态手势 \~english Dynamic hand gesture
//	ST_MOBILE_MODEL_TYPE_HAND_FARDISTANCE = 104             ///< \~chinese 远距离手势 \~english Far-distance hand gesture

	ST_MOBILE_MODEL_TYPE_BODY_2D =200,                      ///< \~chinese 肢体2d模型,半身肢体2d模型，肢体轮廓点模型 \~english Body 2D model, half-body 2D model, body contour point model
	ST_MOBILE_MODEL_TYPE_BODY_3D = 202,                     ///< \~chinese 肢体3d点 \~english Body 3D points
    ST_MOBILE_MODEL_TYPE_UPBODY_AVATAR = 203,               ///< \~chinese 半身肢体avatar \~english Upper body avatar
	ST_MOBILE_MODEL_TYPE_BODY_MESH = 204,                     ///< \~chinese 3dbodymesh \~english 3D body mesh keypoints

	ST_MOBILE_MODEL_TYPE_HEAD_DETECT = 300,                 ///< \~chinese 头部检测 \~english Head detection
	ST_MOBILE_MODEL_TYPE_HEAD_MESH = 301,                   ///< \~chinese 头部mesh \~english Head mesh

	ST_MOBILE_MODEL_TYPE_SEGMENT_FIGURE = 400,              ///< \~chinese 背景分割 \~english Background segmentation
	ST_MOBILE_MODEL_TYPE_SEGMENT_HAIR = 401,                ///< \~chinese 头发分割 \~english Hair segmentation
	ST_MOBILE_MODEL_TYPE_SEGMENT_MULTI = 402,               ///< \~chinese 多类分割 \~english Multi-class segmentation
	ST_MOBILE_MODEL_TYPE_SEGMENT_HEAD= 403,                 ///< \~chinese 头部分割 \~english Head segmentation
	ST_MOBILE_MODEL_TYPE_SEGMENT_SKIN = 404,                ///< \~chinese 皮肤分割 \~english Skin segmentation
	ST_MOBILE_MODEL_TYPE_SEGMENT_MOUTH_PARSE = 405,         ///< \~chinese 嘴部遮挡信息分割 \~english Mouth occlusion information segmentation
	ST_MOBILE_MODEL_TYPE_SEGMENT_FACE_OCCLUSION = 406,      ///< \~chinese 面部遮挡信息分割 \~english Face occlusion information segmentation
	ST_MOBILE_MODEL_TYPE_SEGMENT_SKY = 407,				    ///< \~chinese 天空分割 \~english Sky segmentation

	ST_MOBILE_MODEL_TYPE_DEPTH_ESTIMATION = 408,		    ///< \~chinese 深度估计信息 \~english Depth estimation information
    ST_MOBILE_MODEL_TYPE_SEGMENT_TROUSER_LEG = 409,         ///< \~chinese 裤腿分割 \~english Trouser leg segmentation
    ST_MOBILE_MODEL_TYPE_SEGMENT_CLOTH = 410,               ///< \~chinese 衣服分割 \~english Clothing segmentation
    ST_MOBILE_MODEL_TYPE_SEGMENT_SALIENTOBJECT = 411,       ///先装修好ｉ分割

	ST_MOBILE_MODEL_TYPE_NAIL = 501,                        ///< \~chinese 指甲检测 \~english Nail detection
    ST_MOBILE_MODEL_TYPE_FINGER = 502,					    ///< \~chinese 手指关键点检测 \~english Finger detection

	ST_MOBILE_MODEL_TYPE_FOOT = 600,					    ///< \~chinese 脚部关键点检测 \~english Foot keypoints detection

	ST_MOBILE_MODEL_TYPE_WRIST = 609,					    ///< \~chinese 手腕关键点检测 \~english Wrist keypoints detection
} st_mobile_model_type;

/// \~chinese
/// @brief 删除子模型
/// @param[in] handle 人体行为检测句柄
/// @param[in] model_type 删除对应的模型,可用来减少内存
/// @return 成功返回ST_OK， 失败返回其他错误码
/// \~english
/// @brief Remove a previously added sub-model to reduce memory usage
/// @param[in] handle The initialized human action handle
/// @param[in] model_type The model type of the corresponding sub-model to be removed
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API
st_result_t st_mobile_human_action_remove_model_by_config(
	st_handle_t handle,
	st_mobile_model_type model_type
	);

/// \~chinese
/// @brief 释放人体行为检测句柄
/// @param[in] handle 已初始化的人体行为句柄
/// \~english
/// @brief Destory human action handle
/// @param[in] handle The initialized human action handle
ST_SDK_API
void st_mobile_human_action_destroy(
    st_handle_t handle
);

/// \~chinese
/// @brief 人体行为检测
/// @param[in] handle 已初始化的人体行为句柄
/// @param[in] image 用于检测的图像数
/// @param[in] pixel_format 用于检测的图像数据的像素格式. 检测人脸建议使用NV12、NV21、YUV420P(转灰度图较快),检测手势和前后背景建议使用BGR、BGRA、RGB、RGBA
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数；目前仅支持字节对齐的padding,不支持roi
/// @param[in] orientation 图像中人脸的方向
/// @param[in] detect_config 需要检测的人体行为,例如ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH | ST_MOBILE_HAND_LOVE | ST_MOBILE_SEG_BACKGROUND
/// @param[out] p_human_action 检测到的人体行为,由用户分配内存. 会覆盖上一次的检测结果.
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Detect human actions
/// @param[in] handle The initialized human action handle
/// @param[in] image Image data used for detection
/// @param[in] pixel_format Pixel format of the image data. For face detection, it is recommended to use NV12, NV21, YUV420P (faster when converted to gray). For gesture and segmentation, it is recommended to use BGR, BGRA, RGB, RGBA
/// @param[in] image_width Width of the image used for detection (in pixels)
/// @param[in] image_height Height of the image (in pixels)
/// @param[in] image_stride Stride of the image (in pixels), which is the number of bytes per row. Currently, only byte-aligned padding is supported, and ROI is not supported.
/// @param[in] orientation Orientation of the face in the image
/// @param[in] detect_config Human actions to be detected, e.g., ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH | ST_MOBILE_HAND_LOVE | ST_MOBILE_SEG_BACKGROUND
/// @param[out] p_human_action Detected human actions, allocated by the user. The previous detection results will be overwritten
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_human_action_detect(
    st_handle_t handle,
    const unsigned char *image,
    st_pixel_format pixel_format,
    int image_width,
    int image_height,
    int image_stride,
    st_rotate_type orientation,
    unsigned long long detect_config,
    st_mobile_human_action_t *p_human_action
);

/// \~chinese
/// @brief 人体行为检测(输入为多平面图像)
/// @param[in] handle 已初始化的人体行为句柄
/// @param[in] image 用于检测的图像数据 只支持yuv，nv12,nv21格式
/// @param[in] orientation 图像中人脸的方向
/// @param[in] detect_config 需要检测的人体行为,例如ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH | ST_MOBILE_HAND_LOVE | ST_MOBILE_SEG_BACKGROUND
/// @param[out] p_human_action 检测到的人体行为,由用户分配内存. 会覆盖上一次的检测结果.
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Detect human actions (multi-plane image input)
/// @param[in] handle The initialized human action handle
/// @param[in] image Image data used for detection, only supports YUV, NV12, NV21 formats
/// @param[in] orientation Orientation of the face in the image
/// @param[in] detect_config Human actions to be detected, e.g., ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH | ST_MOBILE_HAND_LOVE | ST_MOBILE_SEG_BACKGROUND
/// @param[out] p_human_action Detected human actions, allocated by the user. The previous detection results will be overwritten
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_human_action_detect_from_multi_plane_image(
    st_handle_t handle,
    const st_multiplane_image_t* image,
    st_rotate_type orientation,
    unsigned long long detect_config,
    st_mobile_human_action_t *p_human_action
);

/// \~chinese
/// @brief 重置, 清除所有缓存信息. 视频模式下会在handle中缓存一些状态，当切换分辨率、切换前后摄像头、切换视频、两帧图像差别较大时建议调用reset
/// @param[in] handle 已初始化的human_action句柄
/// \~english
/// @brief Reset, clear all cached information. In video mode, some states will be cached in the handle. It is recommended to call reset when switching resolutions, switching between front and rear cameras, switching videos, or when there is a significant difference between two frames
/// @param[in] handle　The initialized human action handle
ST_SDK_API st_result_t
st_mobile_human_action_reset(
    st_handle_t handle
);

/// \~chinese
/// @brief 获取分割前景图像(人像/天空分割需要)
/// @param[in] handle　已初始化的human_action句柄
/// @param[in] 输入分割config 目前只支持config=ST_MOBILE_SEG_BACKGROUND或config=ST_MOBILE_SEG_SKY，不能并联使用．
/// @param[out] 输出分割前景图像 只返回ST_PIX_FMT_BGR888格式
/// @return 成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Get the segmented foreground image (required for human/sky segmentation)
/// @param[in] handle　The initialized human action handle
/// @param[in] 输入分割config The segmentation configuration. Currently, only ST_MOBILE_SEG_BACKGROUND or ST_MOBILE_SEG_SKY is supported. They cannot be used simultaneously
/// @param[out] 输出分割前景图像 Segmented foreground image, in the ST_PIX_FMT_BGR888 format
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_human_action_get_segment_foreground (
    st_handle_t handle,
    unsigned long long config,
    st_image_t* figure_matting_foreground
);

/// \~chinese
/// @brief 使用GPU对分割结果做后处理，并将处理后的结果输出到纹理上，需要在OpenGL context中调用
/// @param[in] handle　已初始化的human_action句柄
/// @param[in] p_segment CPU中的分割输出结果（st_mobile_human_action_t）结构体中
/// @param[in] p_src_tex 用于检测的预览/图片纹理
/// @param[in] segment_type 分割数据的类型，通过detect config区分，目前只支持天空分割（ST_MOBILE_SEG_SKY 和 ST_MOBILE_SEG_GREEN）
/// @param[out] p_dst_tex GPU处理后的分割结果，需要在上层预先创建texture，纹理比例需要与原图一致，分辨率可不同
/// @return 成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Perform post-processing on the segmentation result using GPU and output the processed result to a texture, should to be called within an OpenGL context
/// @param[in] handle The initialized human action handle
/// @param[in] p_segment The segmentation output result (st_mobile_human_action_t) in CPU
/// @param[in] p_src_tex The preview/image texture used for detection
/// @param[in] segment_type The type of segmentation data. It is determined by the detect configuration. Currently, only sky segmentation (ST_MOBILE_SEG_SKY and ST_MOBILE_SEG_GREEN) is supported
/// @param[out] p_dst_tex The GPU-processed segmentation texture which needs to be pre-created. The texture should have the same aspect ratio as the original image, but the resolution can be different
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_human_action_gpu_segment_refine(
    st_handle_t handle,
    const st_mobile_segment_t *p_segment,
    const st_mobile_texture_t *p_src_tex,
    unsigned long long segment_type,
    st_mobile_texture_t *p_dst_tex
);

/// \~chinese
/// @brief 释放humanAction handle内部的GL资源，需要在OpenGL context中调用。调用该接口之后，如果没有再调用GL相关的接口，handle的销毁不需要在GL线程中调用。否则，
///        如果handle销毁没有在GL线程中调用，GL相关资源可能会泄露
/// @param[in] handle　已初始化的human_action
/// @return 成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Release the inner GL resources，which should be called within an OpenGL context. After calling this, if no further OpenGL-related functions are called, the handle can be destroyed outside the OpenGL thread. Otherwise, it may result in resource leaks related to OpenGL
/// @param[in] handle　The initialized human action handle
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_human_action_release_gl_resource (
    st_handle_t handle
);

/// \~chinese @brief human_action参数类型
/// \~english @brief Human action parameter types
typedef enum {
    // \~chinese 人脸参数 \~english Face parameters
    /// \~chinese 设置检测到的最大人脸数目N(默认值32, 最大值32),持续track已检测到的N个人脸直到人脸数小于N再继续做detect.值越大,检测到的人脸数目越多,但相应耗时越长. 如果当前人脸数目达到上限，检测线程将休息 \~english Set the maximum number of detected faces N (default value: 32, maximum value: 32). Continue tracking the detected N faces until the number of faces becomes less than N before continuing detection. The larger the value, the more faces will be detected, but the longer the processing time. If the current number of faces reaches the limit, the detection thread will rest
    ST_HUMAN_ACTION_PARAM_FACELIMIT = 0,
    /// \~chinese 设置tracker每多少帧进行一次detect(默认值有人脸时24,无人脸时24/3=8). 值越大,cpu占用率越低, 但检测出新人脸的时间越长. \~english Set the number of frames between face detections (default value: 24 when a face is present, 8 when no face is present). The larger the value, the lower the CPU usage, but it takes longer to detect new faces
    ST_HUMAN_ACTION_PARAM_FACE_DETECT_INTERVAL = 1,
    /// \~chinese 设置106点、240点是否平滑(默认值0 不平滑),不平滑点会有些抖动，平滑点会有点滞后. \~english Set whether to smooth the 106-point or 240-point facial landmarks (default value: 0 for no smoothing). Non-smoothed landmarks may have some jitter, while smoothed landmarks may be slightly delayed
    ST_HUMAN_ACTION_PARAM_FACE_SMOOTH = 2,
    /// \~chinese 设置head_pose去抖动的阈值[0.0,1.0](默认值0.5),值越大, pose信息的值越稳定,但相应值会有滞后. \~english Set the threshold for head pose smoothing [0.0,1.0] (default value: 0.5). A higher value makes the pose information more stable, but introduces some delay
    ST_HUMAN_ACTION_PARAM_HEADPOSE_THRESHOLD = 3,
    /// \~chinese 设置脸部隔帧检测（对上一帧结果做拷贝），目的是减少耗时。默认每帧检测一次. 最多每10帧检测一次. 开启隔帧检测后, 只能对拷贝出来的检测结果做后处理. \~english Set the interval for face processing (copying the results from the previous frame) to reduce processing time. The default is to process every frame. The maximum interval is every 10 frames. When enabled, only the copied detection results can be post-processed
    ST_HUMAN_ACTION_PARAM_FACE_PROCESS_INTERVAL = 5,
    /// \~chinese 设置人脸106点检测的阈值[0.0,1.0] \~english Set the threshold for face detection [0.0,1.0]
    ST_HUMAN_ACTION_PARAM_FACE_THRESHOLD = 6,
    /// \~chinese /// 设置mesh渲染模式, mesh分为人脸，眼睛，嘴巴，后脑勺，耳朵，脖子，眉毛七个部位，2106模型只包含人脸，眼睛，嘴巴三个部位，3060/2396模型只包含人脸，眼睛，嘴巴，后脑勺，耳朵，脖子六个部位
    /// 参数值类型为st_mobile_mesh_part，默认只渲染人脸st_mobile_mesh_part::ST_MOBILE_MESH_PART_FACE，
    /// 可以设置多个部位，例如：渲染人脸和嘴巴，st_mobile_mesh_part::ST_MOBILE_MESH_PART_FACE + st_mobile_mesh_part::ST_MOBILE_MESH_PART_MOUTH \~english Set the mesh rendering mode. The mesh consists of seven parts: face, eyes, mouth, back of the head, ears, neck, and eyebrows. The 2106 model only includes the face, eyes, and mouth parts, while the 3060/2396 model includes the face, eyes, mouth, back of the head, ears, and neck parts. The parameter value type is st_mobile_mesh_part. By default, only the face part (st_mobile_mesh_part::ST_MOBILE_MESH_PART_FACE) is rendered. Multiple parts can be set, for example: render the face and mouth (st_mobile_mesh_part::ST_MOBILE_MESH_PART_FACE + st_mobile_mesh_part::ST_MOBILE_MESH_PART_MOUTH)
    ST_HUMAN_ACTION_PARAM_MESH_MODE = 20,
    /// \~chinese 设置mesh额头点扩展scale范围起始值（小于终止值，默认是2） \~english Set the starting scale value for expanding the forehead mesh points (default value: 2)
    ST_HUMAN_ACTION_PARAM_MESH_START_SCALE = 21,
    /// \~chinese 设置mesh额头点扩展scale范围终止值（大于起始值，默认是3） \~english Set the ending scale value for expanding the forehead mesh points (default value: 3)
    ST_HUMAN_ACTION_PARAM_MESH_END_SCALE = 22,
    /// \~chinese 设置mesh结果输出坐标系,(0: 屏幕坐标系， 1：世界坐标系） \~english Set the coordinate system for mesh output (0: screen coordinates, 1: world coordinates)
    // ST_HUMAN_ACTION_PARAM_MESH_OUTPUT_FORMAT = 23,

    /// \~chinese 获取mesh模型支持的关键点的数量（2106/3060/2396） \~english Get the number of keypoints supported by the mesh model (2106/3060/2396)
    ST_HUMAN_ACTION_PARAM_MESH_MODEL_VERTEX_NUM = 24,
    /// \~chinese 设置face mesh是否需要内缩282关键点坐标(只对face mesh有效，360度mesh不需要，0：不需要内缩，1：需要内缩) \~english Set whether to narrow the landmark coordinates of the face mesh (valid only for the face mesh). 0: no narrowing, 1: narrowing
    ST_HUMAN_ACTION_PARAM_FACE_MESH_NARROW_LANDMARK = 25,
    // /// \~chinese 设置face mesh是否需要计算边界点(0：不需要计算边界点，1：需要计算边界点） \~english Set whether to calculate boundary points for face mesh (0: do not calculate boundary points, 1: calculate boundary points)
    //ST_HUMAN_ACTION_PARAM_FACE_MESH_CONTOUR = 26,
    // /// \~chinese 设置mesh是否打开透视 1：打开透视， 0：打开正交 (默认打开正交) \~english Set whether the mesh is in perspective view or orthogonal view (1: perspective view, 0: orthogonal view. Default is orthogonal view)
    // ST_HUMAN_ACTION_PARAM_FACE_MESH_PERSPECTIVE = 26,
    /// \~chinese 设置face mesh结果输出坐标系,值使用st_3d_coordinate_type \~english Set the coordinate system for face mesh output, value uses st_3d_coordinate_type
    ST_HUMAN_ACTION_PARAM_FACE_MESH_OUTPUT_FORMAT = 26,
	/// \~chinese 设置head mesh结果输出坐标系, st_3d_coordinate_type，只支持0和1 \~english Set the coordinate system for head mesh output, value uses st_3d_coordinate_type
	ST_HUMAN_ACTION_PARAM_HEAD_MESH_OUTPUT_FORMAT = 27,

    /// \~chinese 设置body mesh结果输出坐标系,值使用st_3d_coordinate_type,只支持0和1 \~english Set the coordinate system for body mesh output, value uses st_3d_coordinate_type
    ST_HUMAN_ACTION_PARAM_BODY_MESH_OUTPUT_FORMAT = 52,

    // \~chinese 手部参数 \~english Hand parameters
    /// \~chinese 设置检测到的最大手数目N(只检测手势时默认值32,检测手骨骼点时默认值1,最大值32),持续track已检测到的N个hand直到手的数目小于N再继续做detect.值越大,检测到的hand数目越多,但相应耗时越长. 如果当前手数目达到上限，检测线程将休息 \~english Set the maximum number of hands detected N (default value 32 if only hand_detect, default value1 if detect hand_skeleton, maximum value 32). Continuously track the detected N hands until the number of hands is less than N, then continue to detect. The larger the value, the more hands detected, but the longer the corresponding time. If the current number of hands reaches the upper limit, the detection thread will rest
    ST_HUMAN_ACTION_PARAM_HAND_LIMIT = 101,
    /// \~chinese 设置手势检测每多少帧进行一次 detect (默认有手时30帧detect一次, 无手时10(30/3)帧detect一次). 值越大,cpu占用率越低, 但检测出新手的时间越长. \~english Set the interval for hand gesture detection (default is 30 frames for detecting hands and 10 (30/3) frames for detecting no hands). The larger the value, the lower the CPU usage, but the longer it takes to detect new hands
    ST_HUMAN_ACTION_PARAM_HAND_DETECT_INTERVAL = 102,
    /// \~chinese 设置手势隔帧检测（对上一帧结果做拷贝），目的是减少耗时。默认每帧检测一次. 最多每10帧检测一次. 开启隔帧检测后, 只能对拷贝出来的检测结果做后处理. \~english Set the interval for hand gesture detection using the previous frame's result as a copy, to reduce processing time. By default, detect every frame. Can detect every 10 frames at most. After enabling interval detection, only post-processing can be done on the copied detection result
    ST_HUMAN_ACTION_PARAM_HAND_PROCESS_INTERVAL = 103,
    /// \~chinese 设置手检测的阈值[0.0,1.0] \~english Set the threshold for hand detection [0.0,1.0]
    ST_HUMAN_ACTION_PARAM_HAND_THRESHOLD = 104,

    /// \~chinese 设置手骨架检测的阈值[0.0,1.0] \~english Set the threshold for hand skeleton detection [0.0,1.0]
    ST_HUMAN_ACTION_PARAM_HAND_SKELETON_THRESHOLD = 110,

    //  \~chinese 肢体参数 \~english Body parameters
    /// \~chinese 设置检测到的最大肢体数目N(默认值1),持续track已检测到的N个肢体直到肢体数小于N再继续做detect.值越大,检测到的body数目越多,但相应耗时越长. 如果当前肢体数目达到上限，检测线程将休息 \~english Set the maximum number of bodies detected N (default value 1). Continuously track the detected N bodies until the number of bodies is less than N, then continue to detect. The larger the value, the more bodies detected, but the longer the corresponding time. If the current number of bodies reaches the upper limit, the detection thread will rest
    ST_HUMAN_ACTION_PARAM_BODY_LIMIT = 200,
    /// \~chinese 设置肢体关键点检测每多少帧进行一次 detect (默认有肢体时30帧detect一次, 无body时10(30/3)帧detect一次). 值越大,cpu占用率越低, 但检测出新body的时间越长. \~english Set the interval for body keypoint detection (default is 30 frames for detecting bodies and 10 (30/3) frames for detecting no bodies). The larger the value, the lower the CPU usage, but the longer it takes to detect new bodies
    ST_HUMAN_ACTION_PARAM_BODY_DETECT_INTERVAL = 201,
    /// \~chinese 设置肢体隔帧检测（对上一帧结果做拷贝），目的是减少耗时。默认每帧检测一次. 最多每10帧检测一次. 开启隔帧检测后, 只能对拷贝出来的检测结果做后处理. \~english Set the parameters for frame skipping in body detection, which copies the results from the previous frame to reduce processing time
    ST_HUMAN_ACTION_PARAM_BODY_PROCESS_INTERVAL = 202,
    /// \~chinese 设置身体检测的阈值[0.0，1.0] \~english Set the threshold for body detection [0.0,1.0]
    ST_HUMAN_ACTION_PARAM_BODY_THRESHOLD = 203,
    /// \~chinese 已废弃 设置是否根据肢体信息检测摄像头运动状态 (0: 不检测; 1: 检测. 默认检测肢体轮廓点时检测摄像头运动状态) \~english (deprecated)
    //ST_HUMAN_ACTION_PARAM_DETECT_CAMERA_MOTION_WITH_BODY = 203,
    /// \~chinese 输入真实身高，单位为米，3D骨架乘以身高（整体缩放），得到真实的物理尺度，仅用于儿童肢体检测 \~english Set the input real height for child body detection
    ST_HUMAN_ACTION_PARAM_BODY_STATURE = 210,

    // \~chinese 人头分割参数 \~english Head segment parameters
    /// \~chinese 设置头部分割检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Set whether to rotate the head segment detection result image
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_RESULT_ROTATE = 300,
    /// \~chinese 设置人头分割边界区域上限阈值.开启温度系数时设置无效 \~english Set the upper threshold for head segment detection result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_MAX_THRESHOLD = 301,
    /// \~chinese 设置人头分割边界区域下限阈值 开启温度系数时设置无效 \~english Set the lower threshold for head segment detection result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_MIN_THRESHOLD = 302,
    /// \~chinese 头部分割后处理长边的长度[10,长边长度](默认长边240,短边=长边/原始图像长边*原始图像短边).值越大,头部分割后处理耗时越长,边缘部分效果越好. \~english Set the length of the longer side for post-processing the head segment result. The value should be in the range [10, length of the longer side]
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_MAX_SIZE = 303,
    /// \~chinese 设置最大人头分割个数，默认支持一个人头分割，face_id为人脸id; 若支持多个人头分割，则face id为-1. \~english Sets the maximum number of head segmentations. By default, only one head segmentation is supported, and the face ID is used as the ID for the head segmentation. If multiple head segmentations are supported, the face ID should be set to -1
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_MAX_COUNT = 304,
    /// \~chinese 设置头部分割检测结果边缘模糊程度 取值范围0-1 视频版默认0.5, 图片版默认是1 \~english Set the degree of edge blur for the head segment result. The value should be in the range [0, 1]. The default value is 0.5 for video and 1 for images
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_RESULT_BLUR = 305,
    /// \~chinese 设置头部分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the head segment result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, the edge boundaries should be processed by thresholds
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_USE_TEMPERATURE = 306,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for the head segment. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_EDGESHIFT = 307,
    /// \~chinese 设置人头实例分割，人头实例分割依赖人脸检测，且只有加载人头实例分割模型设置该参数有效．默认不开启，出单个mask; 开启则多人多mask，数量跟人脸个数对应． \~english Sets the head instance segmentation. Head instance segmentation relies on face detection and is only effective when the head instance segmentation model is loaded. By default, this parameter is disabled, resulting in a single mask. When enabled, multiple masks are generated, and the number of masks corresponds to the number of detected faces
    ST_HUMAN_ACTION_PARAM_HEAD_SEGMENT_INSTANCE = 308,

	// \~chinese 背景分割/人像分割参数 \~english Background/figure segmentation parameters
    /// \~chinese 输出的background结果中长边的长度[10,长边长度](默认长边为模型内部处理的长边，若设置会做resize处理输出).值越大,背景分割的耗时越长,边缘部分效果越好.值为0还原为默认值. \~english Set the length of the longer side for the background segmentation result. The value should be in the range [10, length of the longer side]. By default, the longer side is determined by the internal processing of the model. If set, the output will be resized accordingly. A larger value will result in longer processing time and better edge effect on the segmented background. Setting the value to 0 will restore the default value
    ST_HUMAN_ACTION_PARAM_BACKGROUND_MAX_SIZE = 400,
    /// \~chinese 背景分割羽化程度[0,1](默认值0.35),0 完全不羽化,1羽化程度最高,在strenth较小时,羽化程度基本不变.值越大,前景与背景之间的过度边缘部分越宽. \~english Set the degree of feathering for the background segmentation result. The value should be in the range [0, 1], where 0 means no feathering and 1 means the highest degree of feathering. When the strength is low, the degree of feathering remains almost unchanged. A higher value widens the transition edge between the foreground and background
    /// \~chinese 备注：只对1.5.0 人像分割模型有效 \~english Note: This parameter is only effective for the figure segmentation model v1.5.0
    ST_HUMAN_ACTION_PARAM_BACKGROUND_BLUR_STRENGTH = 401,
    /// \~chinese 设置前后背景检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Set whether to rotate the grayscale background result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image). By default, rotation is disabled
    ST_HUMAN_ACTION_PARAM_BACKGROUND_RESULT_ROTATE = 402,
    /// \~chinese 设置背景分割边界区域上限阈值. 开启温度系数时设置无效 \~english Set the upper threshold for the boundary region of the background segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_SEGMENT_MAX_THRESHOLD = 403,
    /// \~chinese 设置背景分割边界区域下限阈值 开启温度系数时设置无效 \~english Set the lower threshold for the boundary region of the background segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_SEGMENT_MIN_THRESHOLD = 404,
//	ST_HUMAN_ACTION_PARAM_SEGMENT_KERNAL_TYPE = 406, \~chinese 已废弃 \~english (deprecated)
    /// \~chinese 设置背景分割检测结果边缘模糊程度 取值范围0-1, 视频版默认是0.5 图片版默认是1 \~english Set the degree of edge blur for the background segmentation result. The value should be in the range [0, 1]. The default value is 0.5 for video and 1 for images
    ST_HUMAN_ACTION_PARAM_BACKGROUND_SEGMENT_RESULT_BLUR = 407,
	/// \~chinese 设置背景分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the background segmentation result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min max threshold should be used for mask postprocess. By default, this parameter is enabled
	ST_HUMAN_ACTION_PARAM_BACKGROUND_SEGMENT_USE_TEMPERATURE = 408,

    ST_HUMAN_ACTION_PARAM_BACKGROUND_SEGMENT_INSTANCE = 409,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for the background segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_BACKGROUND_SEGMENT_EDGESHIFT=399,

// \~chinese 头发分割参数 \~english Hair segmentation parameters
    /// \~chinese 头发结果中长边的长度[10,长边长度](默认长边240,短边=长边/原始图像长边*原始图像短边).值越大,头发分割的耗时越长,边缘部分效果越好. \~english Set the length of the longer side for the hair segmentation result. The value should be in the range [10, length of the longer side]. A larger value will result in longer processing time but better edge effect on the segmented hair
    ST_HUMAN_ACTION_PARAM_HAIR_MAX_SIZE = 410,
    /// \~chinese 头发分割羽化程度[0,1](默认值0.35),0 完全不羽化,1羽化程度最高,在strenth较小时,羽化程度基本不变.值越大,过度边缘部分越宽. \~english Set the hair segmentation feathering degree [0,1] (default value 0.35), 0 is no feathering at all, 1 is the highest feathering degree, when the strenth is small, the feathering degree is basically unchanged. The larger the value, the wider the excessive edge part
    ST_HUMAN_ACTION_PARAM_HAIR_BLUR_STRENGTH = 411,  // \~chinese 无效,可删除 \~english (deprecated)
    /// \~chinese 设置头发灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认0不旋转) \~english Set whether the direction of the hair grayscale image needs to be rotated (0: no rotation, keep it vertical; 1: rotation, the direction is consistent with the input image. By default, rotation is disabled
    ST_HUMAN_ACTION_PARAM_HAIR_RESULT_ROTATE = 412,
    /// \~chinese 设置头发分割边界区域上限阈值.开启温度系数时设置无效 \~english Set the upper limit threshold of the hair segmentation boundary area. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_HAIR_SEGMENT_MAX_THRESHOLD = 414,
    /// \~chinese 设置头发分割边界区域下限阈值 开启温度系数时设置无效 \~english Set the lower limit threshold of the hair segmentation boundary area. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_HAIR_SEGMENT_MIN_THRESHOLD = 415,
	/// \~chinese 设置头发分割检测结果边缘模糊程度 取值范围0-1 视频版默认是0.5，图片版默认是1 \~english Set the edge blurring degree of the hair segmentation detection result. The value range is 0-1. The video mode defaults to 0.5, and the image mode defaults to 1
	ST_HUMAN_ACTION_PARAM_HAIR_SEGMENT_RESULT_BLUR = 416,
	/// \~chinese 设置头发分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the hair segmentation result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min_threshold, max_threshold should be used for mask postprocess. By default, this parameter is enabled
	ST_HUMAN_ACTION_PARAM_HAIR_SEGMENT_USE_TEMPERATURE = 417,
    /// \~chinese 边缘移动参数，取值范围在[-2,2],默认值是1 \~english Set the edge shift parameter for the hair segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_HAIR_SEGMENT_EDGESHIFT=418,

// \~chinese 多类分割参数 \~english Multi-class segmentation parameters
    /// \~chinese 输出的multisegment结果中长边的长度. \~english Set the length of the longer side for the multi-class segmentation result
    ST_HUMAN_ACTION_PARAM_MULTI_SEGMENT_MAX_SIZE = 420,
    /// \~chinese 设置多类分割检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Sets whether to rotate the grayscale multi-class segmentation result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image). By default, rotation is disabled
    ST_HUMAN_ACTION_PARAM_MULTI_SEGMENT_RESULT_ROTATE = 421,


// \~chinese 皮肤分割参数 \~english Skin segmentation parameters
    /// \~chinese 输出的皮肤分割结果中长边的长度. \~english Set the length of the longer side for the skin segmentation result
    ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_MAX_SIZE = 430,
    /// \~chinese 设置皮肤分割边界区域上限阈值.开启温度系数时设置无效 \~english Set the upper threshold for the boundary region of the skin segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_MAX_THRESHOLD = 431,
    /// \~chinese 设置皮肤分割边界区域下限阈值，开启温度系数时设置无效 \~english Set the lower threshold for the boundary region of the skin segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_MIN_THRESHOLD = 432,
    /// \~chinese 设置皮肤分割检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Set whether to rotate the grayscale skin segmentation result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image). By default, rotation is disabled
    ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_RESULT_ROTATE = 433,
	/// \~chinese 设置皮肤分割检测结果边缘模糊程度 取值范围0-1 默认0.5 \~english Set the degree of edge blur for the skin segmentation result. The value should be in the range [0, 1], and the default value is 0.5
	ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_RESULT_BLUR = 434,
	/// \~chinese 设置皮肤分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the skin segmentation result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min max threshould should be used for mask postprocess. By default, this parameter is enabled
	ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_USE_TEMPERATURE = 435,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for the skin segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_EDGESHIFT=436,
    /// \~chinese 设置皮肤实例分割，皮肤实例分割依赖人脸检测，且只有加载皮肤实例模型设置该参数有效．默认不开启，只单个mask; 开启出多人多mask，数量跟人脸个数对应． \~english Set whether to enable skin instance segmentation. Skin instance segmentation relies on face detection, and this parameter is only effective when the skin instance model is loaded. By default, it is disabled, and only a single mask is generated. Enabling it will produce multiple masks corresponding to the number of detected faces
    ST_HUMAN_ACTION_PARAM_SKIN_SEGMENT_INSTANCE = 437,

// \~chinese 嘴唇分割 \~english Lip segmentation parameters
    /// \~chinese 设置嘴唇分割检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Set whether to rotate the grayscale lip segmentation result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image). The default is not to rotate
    ST_HUMAN_ACTION_PARAM_MOUTH_PARSE_RESULT_ROTATE = 450,
    /// \~chinese 设置是否使用嘴唇分割模型的点阈值，如果大于阈值，表示当前嘴无遮挡，不需要额外使用模型来分割，性能更优。 取值范围[0,1],默认值0.8 \~english Set whether to use threshold. If the value is greater than the threshold, it indicates that the lips are not occluded and there is no need to use the model for segmentation, which improves performance. The value range is [0, 1], with a default value of 0.8
    ST_HUMAN_ACTION_PARAM_MOUTH_PARSE_DETECT_THRESHOLD = 451,

// \~chinese 面部遮挡分割参数 \~english Face occlusion segmentation parameters
	/// \~chinese 设置面部遮挡检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Set whether to rotate the grayscale face occlusion segmentation result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image). By default, rotation is disabled
	ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_RESULT_ROTATE = 460,
	/// \~chinese 设置面部遮挡分割边界区域上限阈值，开启温度系数时设置无效 \~english Set the upper threshold for the boundary region of the face occlusion segmentation result. This setting is invalid when the temperature coefficient is enabled
	ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_MAX_THRESHOLD = 461,
	/// \~chinese 设置面部遮挡分割边界区域下限阈值，开启温度系数时设置无效 \~english Set the lower threshold for the boundary region of the face occlusion segmentation result. This setting is invalid when the temperature coefficient is enabled
	ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_MIN_THRESHOLD = 462,
	/// \~chinese 面部遮挡分割后处理长边的长度[10,长边长度](默认长边240,短边=长边/原始图像长边*原始图像短边).值越大,面部遮挡分割后处理耗时越长,边缘部分效果越好. \~english Set the length of the longer side for post-processing after face occlusion segmentation. The value should be in the range [10, length of the longer side]. A larger value leads to longer processing time and better edge quality in the segmented region. By default, the longer side is set to 240
	ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_MAX_SIZE = 463,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for the face occlusion segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_EDGESHIFT = 468,


	/// \~chinese 设置面部遮挡分割检测结果边缘模糊程度 默认参数0.5 \~english Set the degree of edge blur for the face occlusion segmentation result. The default value is 0.5
	ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_RESULT_BLUR = 464,
	/// \~chinese 设置面部遮挡分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the face occlusion segmentation result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min_threshold, max_threshold should be used for mask postprocess. By default, this parameter is enabled
	ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_USE_TEMPERATURE = 467,
    /// \~chinese 设置人脸遮挡实例分割，人脸遮挡实例分割依赖人脸检测，默认不开启，出单个mask; 开启则多人多mask，数量跟人脸个数对应 \~english Set whether to enable face occlusion instance segmentation. Face occlusion instance segmentation relies on face detection. By default, it is disabled, and only a single mask is generated. When enabled, multi occlusion masks will be generated according to the number of detected faces
    ST_HUMAN_ACTION_PARAM_FACE_OCCLUSION_SEGMENT_INSTANCE = 469,

	// \~chinese 通用参数 \~english General parameters
	/// \~chinese 设置预处理图像大小 \~english Set the maximum size of the preprocessed image
	ST_HUMAN_ACTION_PARAM_PREPROCESS_MAX_SIZE = 500, //
	/// \~chinese 摄像头x方向上的视场角，单位为度，3d点会需要 \~english Set the field of view angle in the x-direction of the camera in degrees. This parameter is required for 3D point calculations
    ST_HUMAN_ACTION_PARAM_CAM_FOVX = 211,
    /// \~chinese pose3d结果是否需要保持人头朝上(1: detect输出的结果永远保持人头朝上;  0: detect输出的pos方向和输入图片一致) 默认人头朝上 \~english Set whether the pose3D result should keep the head upright. The options are 1 (the pose3D result always keeps the head upright) and 0 (the pose3D result follows the same direction as the input image). By default, the head is kept upright
	ST_HUMAN_ACTION_PARAM_POS3D_UP = 502,
    /// \~chinese 设置检测结果延迟帧数[0,2],检测接口输出结果为(当前-N)帧的结果, 默认0不延迟.  注意：在该参数修改过程中,当前检测结果会清空重新开始检测 \~english Set the number of frames to delay the detection results. The detection interface outputs results from the (current - N) frame. The default value is 0, which means no delay. Note that modifying this parameter will clear the current results and start the detection process again
    ST_HUMAN_ACTION_PARAM_DELAY_FRAME = 503,
    /// \~chinese 使用GPU做后处理 \~english Set whether to use the GPU for post-processing
    ST_HUMAN_ACTION_PARAM_SEGMENT_POST_PROCESS_USE_GPU=504,
    //  \~chinese 天空分割参数 \~english Sky segmentation parameters
	/// \~chinese 设置目标特征图像 目前已废弃 \~english (Deprecated) Set the target feature image for sky segmentation
	ST_HUMAN_ACTION_PARAM_SKY_TARGET_IMAGE= 509,
	/// \~chinese 输出的sky结果中长边的长度 \~english Set the length of the longer side for the output sky segmentation result
	ST_HUMAN_ACTION_PARAM_SKY_MAX_SIZE = 510,
	/// \~chinese 天空分割检测结果灰度图的方向是否需要旋转 \~english Set whether to rotate the grayscale sky segmentation result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image)
	ST_HUMAN_ACTION_PARAM_SKY_RESULT_ROTATE = 511,
	/// \~chinese 设置天空分割边界区域上限阈值，开启温度系数时设置无效 \~english Set the upper threshold for the boundary region of the sky segmentation result. This setting is invalid when the temperature coefficient is enabled
	ST_HUMAN_ACTION_PARAM_SKY_SEGMENT_MAX_THRESHOLD = 512,
	/// \~chinese 设置天空分割边界区域下限阈值，开启温度系数时设置无效 \~english Set the lower threshold for the boundary region of the sky segmentation result. This setting is invalid when the temperature coefficient is enabled
	ST_HUMAN_ACTION_PARAM_SKY_SEGMENT_MIN_THRESHOLD = 513,
	/// \~chinese 设置天空分割检测结果边缘模糊程度 取值范围0-1，视频版默认参数0.5，图片版默认参数1 \~english Set the degree of edge blur for the sky segmentation result. The value should be in the range [0, 1]. For the video mode, the default value is 0.5, and for the image mode, the default value is 1
	ST_HUMAN_ACTION_PARAM_SKY_SEGMENT_RESULT_BLUR = 508,
	/// \~chinese 设置天空分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the sky segmentation result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min max threshold should be used for mask postprocess. By default, this parameter is enabled
	ST_HUMAN_ACTION_PARAM_SKY_SEGMENT_USE_TEMPERATURE = 507,
	/// \~chinese 使用CPU进行refine操作,默认是使用（＞0.5），当输入参数小于等于0.5时不使用 \~english Set whether to use CPU for sky segmentation refinement. The default behavior is to use CPU when the input parameter is greater than 0.5 and not use CPU when it is less than or equal to 0.5
	ST_HUMAN_ACTION_PARAM_SKY_SEGMENT_REFINE_CPU_PROCESS = 514,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for sky segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_SKY_SEGMENT_EDGESHIFT = 516,
    //  \~chinese 深度估计参数 \~english Depth estimation parameters
	/// \~chinese 输出的深度估计结果中长边的长度 \~english Set the length of the longer side for the output depth estimation result
	ST_HUMAN_ACTION_PARAM_DEPTH_ESTIMATION_MAX_SIZE = 515,

    // \~chinese 指甲检测 \~english Nail detection parameters
	/// \~chinese 设置检测到的最大目标数目N,有效范围为[1, 32], 返回的值可能比输入的值要小. 默认值为10. \~english Set the maximum number of detected nail targets. The valid range is [1, 32], but the returned value may be smaller than the input value. The default value is 10
	ST_HUMAN_ACTION_PARAM_NAIL_LIMIT = 602,
	/// \~chinese 设置指甲检测每多少帧进行一次 detect (默认指甲时30帧detect一次, 无指甲时10(30/3)帧detect一次). 值越大,cpu占用率越低, 但检测出新对象的时间越长. \~english Set the interval between nail detection in frames. When there are nails, the default interval is 30 frames, and when there are no nails, the default interval is 10 frames (30/3). A larger value reduces CPU usage but increases the time to detect new objects
	ST_HUMAN_ACTION_PARAM_NAIL_DETECT_INTERVAL = 603,
	/// \~chinese 设置指甲检测的阈值[0.0,1.0] \~english Set the threshold for nail detection. The value should be in the range [0.0, 1.0]
	ST_HUMAN_ACTION_PARAM_NAIL_THRESHOLD = 604,
	/// \~chinese 指甲平滑参数，取值范围[0,-) 默认参数是0.1 目前以废弃 \~english (Deprecated) Set the smooth parameter for nail detection. The valid range is [0, -). The default value is 0.1
	ST_HUMAN_ACTION_PARAM_NAIL_SMOOTH = 605,
    /// 指甲贴合参数,开启后pose矩阵会使美甲边缘更贴合指甲边缘
    ST_HUMAN_ACTION_PARAM_NAIL_USE_SCALE = 606,

    //	\~chinese 脚部参数 \~english Foot detection parameters
	/// \~chinese 设置检测到的最大脚的个数 \~english Set the maximum number of detected feet
	ST_HUMAN_ACTION_PARAM_FOOT_MAX_LIMIT = 700,
	/// \~chinese 设置脚部检测每多少帧进行一次 detect (默认有脚时30帧detect一次, 无脚时10(30/3)帧detect一次). 值越大,cpu占用率越低, 但检测出新对象的时间越长. \~english Set the interval between foot detections in frames. When feet are present, the default interval is 30 frames, and when no feet are present, the default interval is 10 frames (30/3). A larger value reduces CPU usage but increases the time to detect new objects
	ST_HUMAN_ACTION_PARAM_FOOT_DETECT_INTERVAL = 701,
	/// \~chinese 设置检测阈值[0.0,1.0] \~english Set the threshold for foot detection. The value should be in the range [0.0, 1.0]
	ST_HUMAN_ACTION_PARAM_FOOT_THRESHOLD = 702,
	/// \~chinese 是否输出足部关键点,0-不输出,否则输出,默认不输出 \~english set output foot keypoints or not, 0 - not, 1 - output, default is 0.
	ST_HUMAN_ACTION_PARAM_FOOT_OUTPUT_KEYPOINTS = 703,
	/// \~chinese 设置裤腿分割检测结果边缘模糊程度，取值范围0-1，默认参数0.5 \~english Set the degree of edge blur for trouser leg segmentation results. The value should be in the range [0, 1] and the default parameter is 0.5
	ST_HUMAN_ACTION_PARAM_TROUSER_LEG_SEGMENT_BLUR = 750,
	/// \~chinese 设置分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for trouser leg segmentation results. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min max threshold should be used for mask postprocess. By default, this parameter is enabled
	ST_HUMAN_ACTION_PARAM_TROUSER_LEG_SEGMENT_USE_TEMPERATURE = 751,
    /// 设置裤腿实例分割，裤腿实例分割依赖foot检测，且只有加载裤腿实例分割模型设置该参数有效．默认不开启，出单个mask; 开启则多人多mask，数量跟人脸个数对应．
    ST_HUMAN_ACTION_PARAM_TROUSER_LEG_SEGMENT_INSTANCE = 752,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for trouser leg segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_TROUSER_LEG_SEGMENT_EDGESHIFT = 451,

    // \~chinese 衣物分割 \~english Clothing segmentation parameters
    /// \~chinese 设置衣物分割检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Set whether to rotate the grayscale clothing segmentation result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image). The default is not to rotate
    ST_HUMAN_ACTION_PARAM_CLOTH_SEGMENT_RESULT_ROTATE = 800,
    /// \~chinese 设置衣物分割长边的长度 \~english Set the length of the longer side for the output clothing segmentation result
    ST_HUMAN_ACTION_PARAM_CLOTH_SEGMENT_MAX_SIZE = 801,
    /// \~chinese 设置衣物分割检测结果边缘模糊程度 取值范围0-1，视频版默认参数0.5，图片版默认参数1 \~english Set the degree of edge blur for the clothing segmentation result. The value should be in the range [0, 1]. For the video mode, the default value is 0.5, and for the image mode, the default value is 1
    ST_HUMAN_ACTION_PARAM_CLOTH_SEGMENT_RESULT_BLUR = 802,
    /// \~chinese 设置衣物分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the clothing segmentation result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min max threshold should be used for mask postprocess. By default, this parameter is enabled
    ST_HUMAN_ACTION_PARAM_CLOTH_SEGMENT_USE_TEMPERATURE = 803,
    /// \~chinese 设置衣物分割边界区域上限阈值，开启温度系数时设置无效 \~english Set the upper threshold for the boundary region of the clothing segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_CLOTH_SEGMENT_MAX_THRESHOLD = 804,
    /// \~chinese 设置衣物分割边界区域下限阈值，开启温度系数时设置无效 \~english Set the lower threshold for the boundary region of the clothing segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_CLOTH_SEGMENT_MIN_THRESHOLD = 805,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for clothing segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_CLOTH_SEGMENT_EDGESHIFT = 806,

	// \~chinese 手腕检测 \~english Wrist detection parameters
	/// \~chinese 设置检测到的最大目标数目N,有效范围为[1, 4], 返回的值可能比输入的值要小. 默认值为10. \~english Set the maximum number of detected wrists. The valid range is [1, 4], and the returned value may be smaller than the input value. The default value is 10
	ST_HUMAN_ACTION_PARAM_WRIST_LIMIT = 900,
	/// \~chinese 设置手腕检测每多少帧进行一次 detect (默认30帧detect一次, 无手腕10(30/3)帧detect一次). 值越大,cpu占用率越低, 但检测出新对象的时间越长. \~english Set the interval between wrist detections in frames. The default interval is 30 frames for wrist detection and 10 frames (30/3) for no wrist detection. A larger value reduces CPU usage but increases the time to detect new objects
	ST_HUMAN_ACTION_PARAM_WRIST_DETECT_INTERVAL = 901,
	/// \~chinese 设置手腕检测的roi参数，默认全0，设置检测roi区域，需enable roi需调用st_mobile_human_action_set_roi，默认全0，不开启 \~english Set the ROI (Region of Interest) for wrist detection. This parameter enables setting a specific region for detection. To enable ROI, you need to call st_mobile_human_action_set_roi and provide the ROI parameters. The default value is all zeros, indicating no ROI
	ST_HUMAN_ACTION_PARAM_WRIST_ROI=906,
	/// \~chinese 开启手腕检测roi设置，默认不打开 \~english Enable wrist detection ROI. By default, this parameter is disabled
	ST_HUMAN_ACTION_PARAM_WRIST_ENABLE_ROI=907,
	// /// \~chinese 打开roi设置之后，resize的ratio比例，(0,1] \~english Set the resize ratio in the range (0, 1] when ROI is enabled
	//ST_HUMAN_ACTION_PARAM_WRIST_ROI_RATIO=909,
	/// \~chinese 手表佩戴位置相比手腕宽度的比例 ,默认值是1.0 \~english Set the ratio between the position of the watch and the wrist width. The default value is 1.0
	ST_HUMAN_ACTION_PARAM_WRIST_WRIST_RATIO=911,
	/// \~chinese 手表渲染窗口的roi位置,单位是像素,调用st_mobile_human_action_set_roi进行设置，默认是原始图像，无需提前设置enable roi \~english Sets the ROI for the wrist rendering window in pixels. This parameter is used to specify a specific region for rendering. By default, the ROI is set to the original image, and there is no need to set the ROI in advance
	ST_HUMAN_ACTION_PARAM_WRIST_RENDER_ROI = 914,
	/// \~chinese 手表渲染窗口的尺度变化,(0，-] \~english Set the scale factor for the wrist rendering window. The value should be in the range (0, -)
	ST_HUMAN_ACTION_PARAM_WRIST_RENDER_ROI_SCALE = 915,
	/// \~chinese 设置绿幕分割颜色RGB值,默认为绿色,将颜色值（16进制数0xFFFFFF,按R、G、B排列）按float类型传入 \~english Set the RGB color value for green screen segmentation. The default color is green (hexadecimal value 0xFFFFFF, arranged as R, G, B). The color value should be passed as a float type
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_COLOR = 1000,
    /// \~chinese 设置绿幕分割使用CPU处理，默认为GPU \~english process green segment on CPU, default is on GPU
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_POST_PROCESS_USE_CPU = 1001,
    /// \~chinese 相似度，范围220-90， 默认140，调大可以小相似度 \~english Similarity, ranging from 220 to 90. The default value is 140. Increasing it can decrease the similarity threshold
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_SIMILAR = 1002,
    /// \~chinese 平滑度,建议范围 0-10, 默认7, 越大越平滑， 对边缘也有影响 \~english Smoothness. Recommended range 0-10, default 7. Higher values result in smoother output but may affect edges. Adjust as needed
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_SMOOTH = 1003,
    /// \~chinese 平滑度二,范围0-10， 默认1，越大越平滑， 对边缘也有影响,若边缘出现黑边异色可以将该值升高，若边缘出现内蚀可以该值适当降低 \~english Smoothness 2. Range 0-10, default 1. Increasing the value enhances smoothness, which also affects the edges. If there are black borders or color artifacts around the edges, you can increase this value. If the edges appear eroded, you can decrease this value to reduce the smoothness
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_SMOOTH2 = 1004,
    /// \~chinese 边界效果,范围10-60， 默认60，效果为若边缘出现黑边、异色，可以将该值降低，若边缘出现内蚀，可以适当增大 \~english Range 10-60, default 60. This parameter affects the appearance of the edges. If there are black borders or color artifacts along the edges, you can decrease this value. If the edges appear eroded, you can increase it slightly
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_EDGE = 1005,
    /// \~chinese 设置绿幕分割分割mask后处理的最大边长,单位分辨率，越小mask后处理性能越高，0表示按照原图大小处理，默认720分辨率 \~english The maximum edge length for post-processing after green screen segmentation, measured in resolution units. A smaller value leads to improved post-processing performance. A value of 0 indicates processing at the original image size. The default resolution is 720.
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_MAX_EDGE_LEN = 1006,
        /// \~chinese 调整绿幕分割色系范围, 范围0-10,默认5, 线性变化，数值越大会越扩充范围设定颜色的偏色范围
    ST_HUMAN_ACTION_PARAM_GREEN_SEGMENT_HSV_RANGE = 1007,

    //	\~chinese 手指参数 \~english Finger detection parameters
	/// \~chinese 设置检测到的最大手指的个数 \~english Set the maximum number of detected fingers. The valid range is [1, 32], and the returned value may be smaller than the input value.
	ST_HUMAN_ACTION_PARAM_FINGER_MAX_LIMIT = 1100,
	/// \~chinese 设置手指检测每多少帧进行一次 detect (默认30帧detect一次, 无手指10(30/3)帧detect一次). 值越大,cpu占用率越低, 但检测出新对象的时间越长. \~english Set the interval between finger detections in frames. The default interval is 30 frames for finger detection and 10 frames (30/3) for no finger detection. A larger value reduces CPU usage but increases the time to detect new objects
	ST_HUMAN_ACTION_PARAM_FINGER_DETECT_INTERVAL = 1101,

    // \~chinese 显著性分割参数 \~english Background/figure segmentation parameters
    /// \~chinese 输出的分割结果中长边的长度[10,长边长度](默认长边为模型内部处理的长边，若设置会做resize处理输出).值越大,背景分割的耗时越长,边缘部分效果越好.值为0还原为默认值. \~english Set the length of the longer side for the background segmentation result. The value should be in the range [10, length of the longer side]. By default, the longer side is determined by the internal processing of the model. If set, the output will be resized accordingly. A larger value will result in longer processing time and better edge effect on the segmented background. Setting the value to 0 will restore the default value
    ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_MAX_SIZE = 1201,
    /// \~chinese 分割羽化程度[0,1](默认值0.35),0 完全不羽化,1羽化程度最高,在strenth较小时,羽化程度基本不变.值越大,前景与背景之间的过度边缘部分越宽. \~english Set the degree of feathering for the background segmentation result. The value should be in the range [0, 1], where 0 means no feathering and 1 means the highest degree of feathering. When the strength is low, the degree of feathering remains almost unchanged. A higher value widens the transition edge between the foreground and background
    /// \~chinese 备注：只对1.5.0 人像分割模型有效 \~english Note: This parameter is only effective for the figure segmentation model v1.5.0
    ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_BLUR_STRENGTH = 1202,
    /// \~chinese 检测结果灰度图的方向是否需要旋转（0: 不旋转, 保持竖直; 1: 旋转, 方向和输入图片一致. 默认不旋转) \~english Set whether to rotate the grayscale background result image. The options are 0 (do not rotate, keep vertical) and 1 (rotate, align with the input image). By default, rotation is disabled
    ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_RESULT_ROTATE = 1203,
    /// \~chinese 分割边界区域上限阈值. 开启温度系数时设置无效 \~english Set the upper threshold for the boundary region of the background segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_MAX_THRESHOLD = 1204,
    /// \~chinese 分割边界区域下限阈值 开启温度系数时设置无效 \~english Set the lower threshold for the boundary region of the background segmentation result. This setting is invalid when the temperature coefficient is enabled
    ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_MIN_THRESHOLD = 1205,
//	ST_HUMAN_ACTION_PARAM_SEGMENT_KERNAL_TYPE = 406, \~chinese 已废弃 \~english (deprecated)
    /// \~chinese 分割检测结果边缘模糊程度 取值范围0-1, 视频版默认是0.5 图片版默认是1 \~english Set the degree of edge blur for the background segmentation result. The value should be in the range [0, 1]. The default value is 0.5 for video and 1 for images
    ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_SEGMENT_RESULT_BLUR = 1206,
	/// \~chinese 分割检测结果边缘模糊程度需保证开启温度系数，大于0.5为开启，小于0.5为关闭，关闭状态下使用卡阈值模式得到边界，默认状态为开启 \~english Set whether to use the temperature coefficient for the background segmentation result. The value should be greater than 0.5 to enable it and less than 0.5 to disable it. When disabled, min max threshold should be used for mask postprocess. By default, this parameter is enabled
	ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_SEGMENT_USE_TEMPERATURE = 1207,
    /// \~chinese 边缘移动参数，取值范围再[-2,2],默认值是1 \~english Set the edge shift parameter for the background segmentation. The value should be in the range [-2, 2], and the default value is 1
    ST_HUMAN_ACTION_PARAM_SALIENTOBJECT_SEGMENT_EDGESHIFT = 1209,

} st_human_action_param_type;

/// \~chinese
/// @brief 设置human_action参数
/// @param[in] handle 已初始化的human_action句柄
/// @param[in] type human_action参数关键字,例如ST_HUMAN_ACTION_PARAM_BACKGROUND_MAX_SIZE
/// @param[in] value 参数取值
/// @return 成功返回ST_OK,错误则返回错误码,错误码定义在st_mobile_common.h 中,如ST_E_FAIL等
/// \~english
/// @brief Set human_action parameters
/// @param[in] handle The initialized human action handle
/// @param[in] type The type of the human_action parameter, such as ST_HUMAN_ACTION_PARAM_BACKGROUND_MAX_SIZE
/// @param[in] value The value of the parameter
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_human_action_setparam(
    st_handle_t handle,
	st_human_action_param_type type,
    float value
);

/// \~chinese
/// @brief 获取human_action参数
/// @param[in] handle 已初始化的human_action句柄
/// @param[in] type human_action参数关键字,和setparam对应
/// @param[out] value 参数取值
/// @return 成功返回ST_OK,错误则返回错误码,错误码定义在st_mobile_common.h 中,如ST_E_FAIL等
/// \~english
/// @brief Get human_action parameters
/// @param[in] handle The initialized human action handle
/// @param[in] type The type of the human_action parameter, corresponding to the setparam function
/// @param[out] value The value of the parameter
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_human_action_getparam(
    st_handle_t handle,
    st_human_action_param_type type,
    float* value
);

/// \~chinese
//  以下为辅助函数
/// @brief 运行时检查是否有能力检测一些功能
/// @param[in] handle 已初始化的人体行为句柄
/// @param[in] detect_config 需要检测的人体行为,例如ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH | ST_MOBILE_HAND_LOVE | ST_MOBILE_SEG_BACKGROUND
/// @param[out] actual_detect_config 输入的检测config中实际可以检测出的检测项
/// @return 成功返回ST_OK,失败返回其他错误码,错误码常见license没有授权，对应模型没有加载等
/// \~english
//  The following are auxiliary functions
/// @brief Check at runtime whether certain functionalities can be detected
/// @param[in] handle The initialized human action handle
/// @param[in] detect_config The human actions to be detected, such as ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH | ST_MOBILE_HAND_LOVE | ST_MOBILE_SEG_BACKGROUND
/// @param[out] actual_detect_config The actual detection items that can be detected from the input detection configuration
/// @return Return ST_OK on success, or an error code on failure.  Common error codes include unauthorized license and unloaded models
ST_SDK_API
st_result_t st_mobile_human_action_can_detect(
	st_handle_t handle,
	unsigned long long input_detect_config,
	unsigned long long *actual_detect_config
);

/// \~chinese
/// @brief 获取图像检测需传入的人脸方向，需在加载人脸模型后使用
/// @param[in] handle 已初始化的人体行为句柄
/// @param[in] image 用于检测的图像数据
/// @param[in] pixel_format 用于检测的图像数据的像素格式. 检测人脸建议使用NV12、NV21、YUV420P(转灰度图较快),检测手势和前后背景建议使用BGR、BGRA、RGB、RGBA
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数；目前仅支持字节对齐的padding,不支持roi
/// @param[out] orientation 图像中人脸的方向
/// @return 成功返回ST_OK,失败返回其他错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Get the face orientation required for image detection, should be used after loading the face model
/// @param[in] handle Initialized body behavior handle
/// @param[in] image Image data for detection
/// @param[in] pixel_format Pixel format of the image data for detection
/// For face detection, it is recommended to use NV12, NV21, YUV420P (faster when converted to grayscale)
/// For gesture and background/foreground detection, it is recommended to use BGR, BGRA, RGB, RGBA
/// @param[in] image_width Width of the image for detection (in pixels)
/// @param[in] image_height Height of the image for detection (in pixels)
/// @param[in] image_stride Stride of the image for detection (in pixels), i.e., the number of bytes per row
/// Currently, only byte-aligned padding is supported, and ROI is not supported.
/// @param[out] orientation Orientation of the face in the image
/// @return Return ST_OK on success, or other error codes defined in st_mobile_common.h, such as ST_E_FAIL, on failure
ST_SDK_API st_result_t
st_mobile_get_detect_orientation(
    st_handle_t handle,
    const unsigned char *image,
    st_pixel_format pixel_format,
    int image_width,
    int image_height,
    int image_stride,
    st_rotate_type* orientation
    );

/// \~chinese
/// @brief 镜像human_action检测结果. 隔帧检测时, 需要将检测结果拷贝出来再镜像
/// @param[in] image_width 检测human_action的图像的宽度(以像素为单位)
/// @param[in,out] p_human_action 需要镜像的human_action检测结果
/// \~english
/// @brief Mirror the human action detection results. When detecting every other frame, the detection results need to be copied and mirrored
/// @param[in] image_width Width of the image used for human action detection (in pixels)
/// @param[in,out] p_human_action The human action detection results to be mirrored
ST_SDK_API void
st_mobile_human_action_mirror(
    int image_width,
    st_mobile_human_action_t *p_human_action
);

/// \~chinese
/// @brief 旋转human_action检测结果.
/// @param[in] image_width 检测human_action的图像的宽度(以像素为单位)
/// @param[in] image_height 检测human_action的图像的宽度(以像素为单位)
/// @param[in] orientation 顺时针旋转的角度
/// @param[in] b_rotate_image 是否旋转图片
/// @param[in,out] p_human_action 需要旋转的human_action检测结果
/// \~english
/// @brief Rotate the human action detection results
/// @param[in] image_width Width of the image used for human action detection (in pixels)
/// @param[in] image_height Height of the image (in pixels)
/// @param[in] orientation The angle of clockwise rotation
/// @param[in] b_rotate_image Whether to rotate the image
/// @param[in,out] p_human_action The human action detection results to be rotated
ST_SDK_API void
st_mobile_human_action_rotate(
	int image_width,
	int image_height,
	st_rotate_type orientation,
	bool b_rotate_image,
	st_mobile_human_action_t* p_human_action
);

/// \~chinese
/// @brief 放大/缩小human_action检测结果.背景图像不缩放
/// @param[in] scale 缩放的尺度
/// @param[in,out] p_human_action 需要缩放的human_action检测结果
/// \~english
/// @brief Scale up/down the human action detection results. The background image is not supported
/// @param[in] scale The scale factor for resizing
/// @param[in,out] p_human_action The human action detection results to be scaled
ST_SDK_API
void st_mobile_human_action_resize(
	float scale,
	st_mobile_human_action_t* p_human_action
);

/// \~chinese
/// @brief 拷贝human_action检测结果
/// @param[in] p_human_action_src 需要拷贝的human_action检测结果
/// @param[out] p_human_action_dst 需要拷贝的human_action检测结果.注意初始化成员,如果指针非空, 默认是已被分配过内存, 不会重新分配内存
/// \~english
/// @brief Copys the human action detection results
/// @param[in] p_human_action_src The source human action detection results to be copied
/// @param[out] p_human_action_dst The destination human action detection results to be copied to. Make sure to initialize the member variables. If the pointer is not null, it is assumed that memory has already been allocated and it will not reallocate memory
ST_SDK_API
void st_mobile_human_action_copy(
    const st_mobile_human_action_t * p_human_action_src,
    st_mobile_human_action_t * p_human_action_dst
);

/// \~chinese
/// @brief 删除human_action 结果, 只能删除st_mobile_human_action_copy的输出结果
/// @param[in] p_human_action 需要删除的human_action检测结果
/// \~english
/// @brief Delete the human action results. Only the output results from st_mobile_human_action_copy can be deleted
/// @param[in] p_human_action The human action detection results to be deleted
ST_SDK_API
void st_mobile_human_action_delete(
    st_mobile_human_action_t * p_human_action
);

/// \~chinese @brief 表情动作类型定义
/// \~english @brief Facial expression types
typedef enum{
	// \~chinese 脸部动作 \~english Facical expressions
	ST_MOBILE_EXPRESSION_EYE_BLINK = 1,     ///< \~chinese 眨眼 \~english Blink eye
	ST_MOBILE_EXPRESSION_MOUTH_AH = 2,      ///< \~chinese 嘴巴大张 \~english Open mouth wide
	ST_MOBILE_EXPRESSION_HEAD_YAW = 3,      ///< \~chinese 摇头 \~english Shake head
	ST_MOBILE_EXPRESSION_HEAD_PITCH = 4,    ///< \~chinese 点头 \~english Nod head
	ST_MOBILE_EXPRESSION_BROW_JUMP = 5,     ///< \~chinese 挑眉 \~english Raise eyebrow
	// \~chinese 手 \~english Hand
	ST_MOBILE_EXPRESSION_HAND_OK = 9,       ///< \~chinese OK手势 \~english OK hand
	ST_MOBILE_EXPRESSION_HAND_SCISSOR = 10, ///< \~chinese 剪刀手 \~english Scissor hand
	ST_MOBILE_EXPRESSION_HAND_GOOD = 11,    ///< \~chinese 大拇哥 \~english Thumbs Up
	ST_MOBILE_EXPRESSION_HAND_PALM = 12,    ///< \~chinese 手掌 \~english Palm hand
	ST_MOBILE_EXPRESSION_HAND_PISTOL = 13,  ///< \~chinese 手枪手势 \~english Pistol hand
	ST_MOBILE_EXPRESSION_HAND_LOVE = 14,    ///< \~chinese 爱心手势 \~english Love hand
	ST_MOBILE_EXPRESSION_HAND_HOLDUP = 15,  ///< \~chinese 托手手势 \~english Hold up hand
	ST_MOBILE_EXPRESSION_HAND_CONGRATULATE = 17,  ///< \~chinese 恭贺A（抱拳） \~english Congratulate A hand
	ST_MOBILE_EXPRESSION_HAND_FINGER_HEART = 18,  ///< \~chinese 单手比爱心 \~english Finger heart hand
	ST_MOBILE_EXPRESSION_HAND_FINGER_INDEX = 20,  ///< \~chinese 食指指尖 \~english Index finger point
	ST_MOBILE_EXPRESSION_HAND_FIST = 21,    ///< \~chinese 拳头 \~english Fist hand
	ST_MOBILE_EXPRESSION_HAND_666 = 22,     ///< \~chinese 666 \~english 666 hand
	ST_MOBILE_EXPRESSION_HAND_BLESS = 23,   ///< \~chinese 双手合十 \~english Bless hand
	ST_MOBILE_EXPRESSION_HAND_ILOVEYOU = 24,///< \~chinese 手势ILoveYou \~english I Love You hand
	ST_MOBILE_EXPRESSION_HAND_SSH = 25,     ///< \~chinese 嘘 \~english Shush hand
	ST_MOBILE_EXPRESSION_HAND_THREE = 26,   ///< \~chinese 手势3 \~english Three fingers gesture
	ST_MOBILE_EXPRESSION_HAND_FOUR = 27,    ///< \~chinese 手势4 \~english Four fingers gesture
	ST_MOBILE_EXPRESSION_HAND_THUMBS_DOWN = 28, ///< \~chinese 差评 \~english Thumbs down gesture
	ST_MOBILE_EXPRESSION_HAND_LOVEB = 29,       ///< \~chinese 比心B \~english Heart hand gesture
	ST_MOBILE_EXPRESSION_HAND_LOVEC = 30,       ///< \~chinese 比心C \~english Heart hand gesture
	ST_MOBILE_EXPRESSION_HAND_SEVEN = 31,       ///< \~chinese 七（卷心菜） \~english Seven gesture
	ST_MOBILE_EXPRESSION_HAND_FACE_PALMING = 32,///< \~chinese 捧脸 \~english Face plaming gesture
	ST_MOBILE_EXPRESSION_HAND_PRAY = 33,        ///< \~chinese 祈祷 \~english Pray gesture
	ST_MOBILE_EXPRESSION_HAND_NINE = 34,        ///< \~chinese 九 \~english Nine gesture
    ST_MOBILE_EXPRESSION_HAND_CONGRATULATE_B = 35,  ///< \~chinese 恭贺B（抱拳） \~english Congratulate B hand
    // \~chinese 头状态 \~english Head status
	ST_MOBILE_EXPRESSION_HEAD_NORMAL = 65,    ///< \~chinese 头正向 \~english Normal head
	ST_MOBILE_EXPRESSION_SIDE_FACE_LEFT = 66, ///< \~chinese 头向左侧偏 \~english Head facing left
	ST_MOBILE_EXPRESSION_SIDE_FACE_RIGHT = 67,  ///< \~chinese 头向右侧偏 \~english Head facing right
	ST_MOBILE_EXPRESSION_TILTED_FACE_LEFT = 68, ///< \~chinese 头向左侧倾斜 \~english Head tilted left
	ST_MOBILE_EXPRESSION_TILTED_FACE_RIGHT = 69,///< \~chinese 头向右侧倾斜 \~english Head tilted right
	ST_MOBILE_EXPRESSION_HEAD_RISE = 70,        ///< \~chinese 抬头 \~english Head raised
	ST_MOBILE_EXPRESSION_HEAD_LOWER = 71,       ///< \~chinese 低头 \~english Head lowered
	// \~chinese 眼状态 \~english Eye status
	ST_MOBILE_EXPRESSION_TWO_EYE_CLOSE = 85, ///< \~chinese 两眼都闭 \~english Both Eyes closed
	ST_MOBILE_EXPRESSION_TWO_EYE_OPEN = 86,  ///< \~chinese 两眼都睁 \~english Both Eyes open
	ST_MOBILE_EXPRESSION_LEFTEYE_OPEN_RIGHTEYE_CLOSE = 87, ///< \~chinese 左眼睁右眼闭 \~english Left eye open, right eye closed
	ST_MOBILE_EXPRESSION_LEFTEYE_CLOSE_RIGHTEYE_OPEN = 88, ///< \~chinese 左眼闭右眼睁 \~english Left eye closed, right eye open
	// \~chinese 嘴状态 \~english Mouth status
	ST_MOBILE_EXPRESSION_MOUTH_OPEN = 105,  ///< \~chinese 张嘴 \~english Open mouth
	ST_MOBILE_EXPRESSION_MOUTH_CLOSE = 106, ///< \~chinese 闭嘴 \~english Closed mouth
	ST_MOBILE_EXPRESSION_FACE_LIPS_UPWARD = 107, ///< \~chinese 嘴角上扬 \~english Lips Upward
	ST_MOBILE_EXPRESSION_FACE_LIPS_POUTED = 108, ///< \~chinese 嘟嘴 \~english Pouted lips
	ST_MOBILE_EXPRESSION_FACE_LIPS_CURL_LEFT = 109,    ///< \~chinese 左撇嘴 \~english Left curl lips
	ST_MOBILE_EXPRESSION_FACE_LIPS_CURL_RIGHT = 110,   ///< \~chinese 右撇嘴 \~english Right curl lips

	ST_MOBILE_EXPRESSION_COUNT = 128,

	// \~chinese 以下只能用于set_expression接口 \~english The following can only be used with the set_expression interface
	ST_MOBILE_EXPRESSION_FACE_ALL = 257,   ///< \~chinese 所有脸部动作 \~english All face expressions
	ST_MOBILE_EXPRESSION_HAND_ALL = 258    ///< \~chinese 所有手部动作 \~english All hand gestures
}ST_MOBILE_EXPRESSION;

/// \~chinese
/// @brief 根据human_action结果获取expression动作信息. 在st_mobile_human_action_detect之后调用
/// @param[in] human_action 输入human_action_detect结果
/// @param[in] orientation 人脸方向
/// @param[in] b_mirror 是否需要镜像expression结果
/// @param[out] expressions_result 用户分配内存，返回检测动作结果数组,动作有效true，无效false
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get expression action information based on human_action results. Should be called after calling st_mobile_human_action_detect
/// @param[in] human_action The input human_action_detect result
/// @param[in] orientation Orientation of the face
/// @param[in] b_mirror Whether to mirror the expression results
/// @param[out] expressions_result Array to store the detected action results. User-allocated memory. The expression action is considered valid if it is true
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_get_expression(
st_mobile_human_action_t* human_action,
st_rotate_type orientation, bool b_mirror,
bool expressions_result[ST_MOBILE_EXPRESSION_COUNT]
);

/// \~chinese
/// @brief 设置检测的roi参数，检测时只在roi范围内检测
/// @param[in] handle 输入human_action_detect检测的handle
/// @param[in] detect_config计算检测的3DPose 目前仅支持ST_HUMAN_ACTION_PARAM_WRIST_ROI和ST_HUMAN_ACTION_PARAM_WRIST_RENDER_ROI
/// @param[in] roi信息 设置检测的范围
/// \~english
/// @brief Set the ROI (Region of Interest) parameters for detection. Only targets within the specified ROI will be detected
/// @param[in] handle The initialized human action handle
/// @param[in] detect_config计算检测的3DPose The detection configuration for which to set the ROI. Currently, only supports ST_HUMAN_ACTION_PARAM_WRIST_ROI and ST_HUMAN_ACTION_PARAM_WRIST_RENDER_ROI
/// @param[in] roi信息 The ROI specifying the detection region
ST_SDK_API st_result_t
st_mobile_human_action_set_roi(
st_handle_t handle,
st_human_action_param_type type,
st_rect_t roi
);

/// \~chinese
/// @brief 设置expression动作阈值
/// @param[in] detect_expression 需要设置阈值的检测动作. 目前仅支持face相关的阈值，可以配置为ST_MOBILE_EXPRESSION_FACE_LIPS_POUTE等
/// @param[in] threshold 阈值数值[0,1]，阈值越大，误检越少，漏检越多
/// \~english
/// @brief Set the threshold for expression action detection
/// @param[in] detect_expression The detection action for which to set the threshold. Currently, only supports face-related thresholds, such as ST_MOBILE_EXPRESSION_FACE_LIPS_POUTE
/// @param[in] threshold The threshold value [0,1]. A higher threshold reduces false positives but may increase false negatives
ST_SDK_API st_result_t
st_mobile_set_expression_threshold(
ST_MOBILE_EXPRESSION detect_expression,
float threshold
);

/// \~chinese
/// @param[in] handle 已初始化的human_action句柄
/// @param[in] p_face 输入检测结果中的一个人脸结构指针
/// @param[in] orientation 人脸方向
/// @param[in] width 检测关键点时的图像宽度
/// @param[in] height 检测关键点时的图像高度
/// @param[in] fov 垂直fov参数，相机的内参，单位是角度，比如60
/// @param[out] p_distance 输出检测距离的结果,单位为米
/// @return 成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @param[in] handle The initialized human action handle
/// @param[in] p_face Face detection result from human action
/// @param[in] orientation Orientation of the face
/// @param[in] width Width of the image
/// @param[in] height Height of the image
/// @param[in] fov The vertical field of view (FOV) parameter, which represents the camera's intrinsic parameters in degrees (e.g., 60)
/// @param[out] p_distance The output result of the distance detection, in meters
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_human_action_calc_face_distance(
st_handle_t handle,
const st_mobile_face_t *p_face,
st_rotate_type orientation,
int width, int height, float fov,
float  *p_distance
);

/// \~chinese
/// @brief 设置Human action各个属性阈值
/// @param[in] handle 已初始化的human action句柄
/// @param[in] config 具体设置哪些阈值，为这几种：
///             ST_MOBILE_EYE_BLINK 眨眼动作
///             ST_MOBILE_MOUTH_AH 张嘴动作
///             ST_MOBILE_HEAD_YAW 摇头动作
///             ST_MOBILE_HEAD_PITCH 点头动作
///             ST_MOBILE_BROW_JUMP 挑眉毛动作
///             ST_MOBILE_FACE_LIPS_UPWARD 嘴角上扬动作
///             ST_MOBILE_FACE_LIPS_POUTED 嘟嘴动作
/// @param[in] threshold 设置config指定类型的阈值，置信度为[0,1], 默认阈值为0.5
/// @return  成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Set the threshold value for various attributes of Human Action
/// @param[in] handle The initialized human action handle
/// @param[in] config Specifies which thresholds to set, can be one of the following types:
///             ST_MOBILE_EYE_BLINK  Eye blink action
///             ST_MOBILE_MOUTH_AH   Mouth open action
///             ST_MOBILE_HEAD_YAW   Head shake action (yaw)
///             ST_MOBILE_HEAD_PITCH Head nod action (pitch)
///             ST_MOBILE_BROW_JUMP  Eyebrow raise action
///             ST_MOBILE_FACE_LIPS_UPWARD Corner of the mouth raised action
///             ST_MOBILE_FACE_LIPS_POUTED Pouted lips action
/// @param[in] threshold Threshold value for the specified config type, confidence range is [0,1], default threshold is 0.5
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_set_human_action_threshold(
    st_handle_t handle,
    unsigned long long config,
    float threshold
);

/// \~chinese
/// @brief 获取已经设置的Human action各个属性阈值
/// @param[in] handle  已初始化的human action句柄
/// @param[in] config 与st_mobile_set_human_action_threshold中config一致
/// @param[in] threshold 取得的阈值
/// @return  成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Get the threshold value of various attributes that have been set
/// @param[in] handle The initialized human action handle
/// @param[in] config Same as the config used in st_mobile_set_human_action_threshold
/// @param[in] threshold Retrieved threshold value
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_get_human_action_threshold(
    st_handle_t handle,
    unsigned long long config,
    float* threshold
);


// \~chinese 以下是mesh相关的使用函数，加载模型后需要调用相关接口获取相关信息 \~english The following functions are related to mesh usage. After loading the model, relevant interfaces need to be called to obtain the related information

/// \~chinese
/// @brief 获取mesh三角拓扑面片索引信息，在加载obj模型后调用一次来获取索引信息，或者在每次设置mesh模式后调用一次来更新索引信息
/// @param[in] handle 已初始化的human_action句柄
/// @param[in] mesh_type mesh类型： face mesh 或者 head mesh
/// @param[out] p_mesh_info mesh标准型信息，底层分配内存
/// @param[out] p_mouth_eye_mesh mesh嘴巴和眼睛面片索引结果，底层分配内存
/// @return 成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Get the triangle topology information of the mesh. Call this function once after loading the OBJ model to obtain the index information, or call it once after setting the mesh mode to update the index information
/// @param[in] handle The initialized human action handle
/// @param[in] mesh_type The type of mesh: face mesh or head mesh
/// @param[out] p_mesh_info Mesh standard type information, memory allocated internally
/// @param[out] p_mouth_eye_mesh Mesh standard type information, memory allocated internally
/// @return Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_human_action_get_mesh_info(
	st_handle_t handle,
	st_mobile_mesh_type mesh_type,
	st_mobile_mesh_info_t * p_mesh_info
);

/// \~chinese
/// @brief 加载mesh的标准人脸obj模型，用于内部的算法处理与mesh输出
/// @param[in] handle 已初始化的human_action句柄
/// @param[in] p_obj_path obj模型的文件路径
/// @param[in] mesh_type mesh类型： face mesh 或者 head mesh
/// @return  成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Load a standard face OBJ model for mesh processing
/// @param[in] handle The initialized human action handle
/// @param[in] p_obj_path File path to the OBJ model
/// @param[in] mesh_type The type of mesh: face mesh or head mesh
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_human_action_load_standard_mesh_obj(
	st_handle_t handle,
	const char* p_obj_path,
	st_mobile_mesh_type mesh_type
);

/// \~chinese
/// @brief 加载mesh的标准人脸obj模型，用于内部的算法处理与mesh输出
/// @param[in] handle 已初始化的human_action句柄
/// @param[in] p_buffer 已经加载到内存中的obj模型buffer
/// @param[in] buffer_len buffer的字节数
/// @param[in] mesh_type mesh类型： face mesh 或者 head mesh
/// @return  成功返回ST_OK, 失败返回其他错误码
/// \~english
/// @brief Load a standard face OBJ model for mesh processing
/// @param[in] handle The initialized human action handle
/// @param[in] p_buffer Buffer of the obj model loaded in memory
/// @param[in] buffer_len Length of the buffer in bytes
/// @param[in] mesh_type The type of mesh: face mesh or head mesh
/// @return  Return ST_OK on success, or an error code on failure
ST_SDK_API st_result_t
st_mobile_human_action_load_standard_mesh_obj_from_buffer(
	st_handle_t handle,
	const char* p_buffer,
	int buffer_len,
	st_mobile_mesh_type mesh_type
);


#endif  // INCLUDE_STMOBILE_ST_MOBILE_HUMAN_ACTION_H_
