/**
*@file st_mobile_face_attribute.h

* \~chinese
* 提供人脸特征检测功能，并可根据人脸特征，识别年龄、性别、肤色、五官、表情等
*
*@attention
* 该文件中的API不保证线程安全.多线程调用时,需要确保安全调用.
* 例如在 detect/reset 没有执行完就执行 destroy 可能造成crash;
* 在 detect 执行过程中调用 reset 函数可能会造成crash.

* 一般调用步骤：创建句柄->检测特征->销毁句柄

* \~english
* Provides facial feature detection functionality and the ability to recognize age, gender, skin color, facial features, and expressions based on facial features.
*
*@attention
* The APIs in this file do not guarantee thread safety. When calling them in a multi-threaded environment, it is important to ensure safe usage.
* For example, calling detect or reset functions while destroy is still executing may result in a crash.
* Calling the reset function during the execution of detect may also lead to a crash.

* General pipeline: Create a handle -> Add models -> Detect features -> Destroy the handle.

* st_mobile_face_attribute_create

* st_mobile_face_attribute_detect

* st_mobile_face_attribute_destroy

**/
#ifndef INCLUDE_STMOBILE_ST_MOBILE_FACE_ATTRIBUTE_H_
#define INCLUDE_STMOBILE_ST_MOBILE_FACE_ATTRIBUTE_H_

#include "st_mobile_common.h"

/// \~chinese
/// @brief 创建人脸属性检测句柄
/// @param[in] model_path 模型文件的绝对路径或相对路径
/// @param[out] handle 人脸属性检测句柄
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Create a handle for face attribute detection
/// @param[in] model_path Absolute or relative path to the model file
/// @param[out] handle The handle for face attribute detection, or NULL if creation fails
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_face_attribute_create(
    const char *model_path,
    st_handle_t *handle
);
/// \~chinese
/// @brief 创建人脸属性检测句柄
/// @param[in] buffer 模型缓存起始地址
/// @param[in] buffer_size 模型缓存大小
/// @param[out] handle 人脸属性检测句柄
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Create a handle for face attribute detection
/// @param[in] buffer Pointer to the model buffer
/// @param[in] buffer_size Length of the model buffer
/// @param[out] handle The face attribute handle
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_face_attribute_create_from_buffer(
const unsigned char* buffer,
unsigned int buffer_size,
st_handle_t * handle);

/// \~chinese
/// @brief 销毁已初始化的人脸属性检测句柄
/// @param[in] handle 已初始化的人脸属性检测句柄
/// \~english
/// @brief Destroy the initialized face attribute detection handle
/// @param[in] handle The initialized face attribute detection handle
ST_SDK_API void
st_mobile_face_attribute_destroy(
    st_handle_t handle
);
#ifndef ST_MOBILE_MAX_ATTR_STRING_LEN
#define ST_MOBILE_MAX_ATTR_STRING_LEN 32
#endif

/// \~chinese @brief 单个属性
/// \~english @brief Single attribute of a face
typedef struct st_mobile_attribute_t {
    char category[ST_MOBILE_MAX_ATTR_STRING_LEN];   ///< \~chinese 属性描述, 例如 "age", "gender" 或 "attractive" \~english Attribute category, such as "age", "gender", or "attractiveness"
    char label[ST_MOBILE_MAX_ATTR_STRING_LEN];      ///< \~chinese 属性标签描述, 例如 "male", "female", "21" 等 \~english Attribute label description, such as "male", "female", "21", etc
    float score;                                    ///< \~chinese 该属性标签的置信度 \~english Confidence level of the attribute label
} st_mobile_attribute_t;

/// \~chinese @brief 一个人脸的所有属性
/// \~english @brief Total attributes of a face
typedef struct st_mobile_attributes_t {
    st_mobile_attribute_t *p_attributes;    ///< \~chinese 属性数组 \~english Attribute array
    int attribute_count;                    ///< \~chinese 属性个数 \~english Number of attributes
} st_mobile_attributes_t;

/// \~chinese
/// @brief 人脸属性检测
/// @param[in] handle 已初始化的人脸属性检测句柄
/// @param[in] image 用于检测的图像数据,推荐BGR格式
/// @param[in] pixel_format 用于检测的图像数据的像素格式, 支持所有彩色图像格式,推荐ST_PIX_FMT_BGR888,不建议使用ST_PIX_FMT_GRAY8（结果不准确）
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像中每一行的跨度(以像素为单位)
/// @param[in] p_face_array 输入待处理的人脸信息,需要包括关键点信息
/// @param[in] face_count 输入的人脸个数
/// @param[out] p_attributes_array 检测到的人脸属性结果数组,包含face_count个st_mobile_attributes_t
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief  Perform face attribute detection
/// @param[in] handle The initialized face attribute detection handle
/// @param[in] image The image data used for face attribute detection, BGR888 format is recommended
/// @param[in] pixel_format The pixel format of the image data, supporting all color image formats. It is recommended to use ST_PIX_FMT_BGR888, and ST_PIX_FMT_GRAY8 is not recommended as it may result in inaccurate results
/// @param[in] image_width Width of the image used for detection, measured in pixels
/// @param[in] image_height Height of the image, in pixels
/// @param[in] image_stride Stride of each row in the image, in pixels
/// @param[in] p_face_array The input face detection results for processing, face 106 keypoints should be included
/// @param[in] face_count The number of input faces
/// @param[out] p_attributes_array The array of detected face attribute results, including face_count instances of st_mobile_attributes_t
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_face_attribute_detect(
    st_handle_t handle,
    const unsigned char *image,
    st_pixel_format pixel_format,
    int image_width,
    int image_height,
    int image_stride,
    const st_mobile_106_t *p_face_array,
    int face_count,
    st_mobile_attributes_t** p_attributes_array
);

/// \~chinese
/// @brief  人脸属性检测
/// @param[in] handle  已初始化的人脸属性检测句柄
/// @param[in] image  用于检测的图像数据,推荐BGR888格式
/// @param[in] pixel_format  用于检测的图像数据的像素格式, 支持所有彩色图像格式,推荐ST_PIX_FMT_BGR888,不建议使用ST_PIX_FMT_GRAY8（结果不准确）
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height  用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像中每一行的跨度(以像素为单位)
/// @param[in] p_face_array  输入待处理的人脸信息,需要包括关键点信息
/// @param[in] face_count 输入的人脸个数
/// @param[out] p_attributes_array 检测到的人脸属性结果数组,包含face_count个st_mobile_attributes_t
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief  Perform face attribute detection
/// @param[in] handle The initialized face attribute detection handle
/// @param[in] image The image data used for face attribute detection, BGR888 format is recommended
/// @param[in] pixel_format The pixel format of the image data, supporting all color image formats. It is recommended to use ST_PIX_FMT_BGR888, and ST_PIX_FMT_GRAY8 is not recommended as it may result in inaccurate results
/// @param[in] image_width Width of the image used for detection, measured in pixels
/// @param[in] image_height Height of the image, in pixels
/// @param[in] image_stride Stride of each row in the image data, in pixels
/// @param[in] p_face_array The input face detection results for processing, face 106 keypoints should be included
/// @param[in] face_count The number of input face
/// @param[out] p_attributes_array The array of detected face attribute results, including face_count instances of st_mobile_attributes_t
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_face_attribute_detect2(
    st_handle_t handle,
    const unsigned char *image,
    st_pixel_format pixel_format,
    int image_width,
    int image_height,
    int image_stride,
    const st_mobile_face_t *p_face_array,
    int face_count,
    st_mobile_attributes_t** p_attributes_array
);


/// \~chinese @brief 属性顺序标号
/// \~english @brief Attribute enumeration
typedef enum{
    ST_ATTR_AGE = 0,                ///< \~chinese 年龄 (0-100)      \~english Confidence (0-100) for age attribute
    ST_ATTR_GENDER_MALE = 1,        ///< \~chinese 性别为男性的置信度(0-1) \~english Confidence (0-1) for male gender
    ST_ATTR_ATTRACTIVE = 2,         ///< \~chinese 魅力0-100         \~english Confidence (0-1) for attractiveness
    ST_ATTR_EYEGLASS = 3,           ///< \~chinese 眼镜置信度(0-1)    \~english Confidence (0-1) for wearing eyeclasses
    ST_ATTR_SUNGLASS = 4,           ///< \~chinese 太阳镜置信度(0-1)  \~english Confidence (0-1) for wearing sun classes
    ST_ATTR_SMILE = 5,              ///< \~chinese 微笑置信度(0-1)    \~english Confidence (0-1) for smiling
    ST_ATTR_MASK = 6,               ///< \~chinese 面具置信度(0-1)    \~english Confidence (0-1) for wearing a mask
    ST_ATTR_EYE_OPEN = 7,           ///< \~chinese 眼睛睁开置信度(0-1) \~english Confidence (0-1) for eyes being open
    ST_ATTR_MOUTH_OPEN = 8,         ///< \~chinese 嘴巴张开置信度(0-1) \~english Confidence (0-1) for mouth being open
    ST_ATTR_BEARD = 9,              ///< \~chinese 有胡子置信度(0-1)   \~english Confidence (0-1) for having a beard
    ST_ATTR_R_A = 10,       		///< \~chinese 置信度(0-1)        \~english Confidence (0-1)
    ST_ATTR_R_B = 11,       		///< \~chinese 置信度(0-1)        \~english Confidence (0-1)
    ST_ATTR_R_C = 12,        		///< \~chinese 置信度(0-1)        \~english Confidence (0-1)
    ST_ATTR_EMOTION_ANGRY = 13,     ///< \~chinese 愤怒置信度(0-1)     \~english Confidence (0-1) for the expression of anger
    ST_ATTR_EMOTION_CALM = 14,      ///< \~chinese 平静置信度(0-1)     \~english Confidence (0-1) for calm
    ST_ATTR_EMOTION_CONFUSED = 15,  ///< \~chinese 困惑置信度(0-1)     \~english Confidence (0-1) for the expression of confusion
    ST_ATTR_EMOTION_DISGUST = 16,   ///< \~chinese 厌恶置信度(0-1)     \~english Confidence (0-1) for disgust
    ST_ATTR_EMOTION_HAPPY = 17,     ///< \~chinese 高兴置信度(0-1)     \~english Confidence (0-1) for the expression of happiness
    ST_ATTR_EMOTION_SAD = 18,       ///< \~chinese 悲伤置信度(0-1)     \~english Confidence (0-1) for the expression of sadness
    ST_ATTR_EMOTION_SCARED = 19,    ///< \~chinese 惊恐置信度(0-1)     \~english Confidence (0-1) for the expression of being scared
    ST_ATTR_EMOTION_SURPRISED = 20, ///< \~chinese 诧异置信度(0-1)     \~english Confidence (0-1) for the expression of being surprised
    ST_ATTR_EMOTION_SQUINT = 21,    ///< \~chinese 斜视置信度(0-1)     \~english Confidence (0-1) for the expression of having a squint
    ST_ATTR_EMOTION_SCREAM = 22,    ///< \~chinese 尖叫置信度(0-1)     \~english Confidence (0-1) for the expression of screaming
    ST_ATTR_LENGTH = 32             ///< \~chinese 属性特征长度        \~english The length of the attribute feature buffer
}st_attribute_index;

/// \~chinese
/// @brief 人脸属性检测, 输出所有特征的置信度
/// @param[in] handle 已初始化的人脸属性检测句柄
/// @param[in] image 用于检测的图像数据,推荐BGR888格式
/// @param[in] pixel_format 用于检测的图像数据的像素格式, 支持所有彩色图像格式,推荐ST_PIX_FMT_BGR888,不建议使用ST_PIX_FMT_GRAY8（结果不准确）
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像中每一行的跨度(以像素为单位)
/// @param[in] p_face_array 输入待处理的人脸信息,需要包括关键点信息
/// @param[in] face_count 输入的人脸个数
/// @param[out] p_attribute_array 检测到的人脸属性结果数组,包含face_count* ST_ATTR_LENGTH 个float 数据
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief Perform face attribute detection
/// @param[in] handle The initialized face attribute detection handle
/// @param[in] image The image data used for face attribute detection, BGR888 format is recommended
/// @param[in] pixel_format The pixel format of the image data, supporting all color image formats. It is recommended to use ST_PIX_FMT_BGR888, and ST_PIX_FMT_GRAY8 is not recommended as it may result in inaccurate results
/// @param[in] image_width Width of the image data used for detection, measured in pixels
/// @param[in] image_height Height of the image, in pixels
/// @param[in] image_stride Stride of each row in the image, in pixels
/// @param[in] p_face_array The input face detection results for processing, face keypoints 106 should be included
/// @param[in] face_count The number of input face
/// @param[out] p_attribute_array The array of detected face attribute results
/// @return  Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_face_attribute_detect_ext(
    st_handle_t handle,
    const unsigned char *image,
    st_pixel_format pixel_format,
    int image_width,
    int image_height,
    int image_stride,
    const st_mobile_106_t *p_face_array,
    int face_count,
    float** p_attribute_array
);

/// \~chinese
/// @brief 人脸属性检测, 输出所有特征的置信度, 只有输入和前一个接口不同
/// @param[in] handle 已初始化的人脸属性检测句柄
/// @param[in] image 用于检测的图像数据,推荐BGR888格式
/// @param[in] pixel_format 用于检测的图像数据的像素格式, 支持所有彩色图像格式,推荐ST_PIX_FMT_BGR888,不建议使用ST_PIX_FMT_GRAY8（结果不准确)
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像中每一行的跨度(以像素为单位)
/// @param[in] p_face_array 输入待处理的人脸信息,需要包括关键点信息
/// @param[in] face_count 输入的人脸个数
/// @param[out] p_attribute_array 检测到的人脸属性结果数组,包含face_count* ST_ATTR_LENGTH 个float 数据
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h中,如ST_E_FAIL等
/// \~english
/// @brief  Perform face attribute detection. This API has the same functionality as the previous one, but with different input parameter
/// @param[in] handle The initialized face attribute detection handle
/// @param[in] image The image data used for face attribute detection, BGR888 format is recommended
/// @param[in] pixel_format The pixel format of the image data, supporting all color image formats. It is recommended to use ST_PIX_FMT_BGR888, and ST_PIX_FMT_GRAY8 is not recommended as it may result in inaccurate results
/// @param[in] image_width Width of the image data used for detection, measured in pixels
/// @param[in] image_height Height of the image, in pixels
/// @param[in] image_stride Stride of each row in the image, in pixels
/// @param[in] p_face_array The input face detection results for processing, face keypoints 106 should be included
/// @param[in] face_count The number of input face
/// @param[out] p_attribute_array The array of detected face attribute results
/// @return Return ST_OK on success, or an error code on failure. Error codes are defined in st_mobile_common.h, such as ST_E_FAIL, etc
ST_SDK_API st_result_t
st_mobile_face_attribute_detect_ext2(
st_handle_t handle,
const unsigned char *image,
st_pixel_format pixel_format,
int image_width,
int image_height,
int image_stride,
const st_mobile_face_t *p_face_array,
int face_count,
float** p_attribute_array
);

#endif // INCLUDE_STMOBILE_ST_MOBILE_FACE_ATTRIBUTE_H_
