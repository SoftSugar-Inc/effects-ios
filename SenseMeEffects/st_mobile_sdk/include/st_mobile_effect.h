/**
 * @file st_mobile_effect.h
 *
 * @brief
 * \~chinese 特效接口是多个功能的集合，主要包括美颜，美妆，滤镜，2D/3D贴纸, tryon等功能，其相关接口定义在st_mobile_effect.h文件中 \~english The effects interface is a collection of multiple functions, primarily including beautification, makeup, filters, 2D/3D stickers, tryon, and other features. The related interfaces are defined in the st_mobile_effect.h file.
 * \~chinese 目前该接口支持RGBA纹理格式或Buffer数据输入, 具体细节请参考接口文档或示例代码 \~english Currently, this interface supports input in RGBA texture format or Buffer data. For specific details, please refer to the interface documentation or sample code.
 * \~chinese 一般调用步骤：创建句柄->配置参数->开始渲染->销毁句柄 \~english General calling process: Create handle -> Configure parameters -> Start rendering -> Destroy handle
 *
 * \~chinese 调用示例：\~english Call example:
 * st_result_t ret = ST_OK;
 * st_handle_t handle = NULL;
 * ret = st_mobile_effect_create_handle(0x0, &handle);
 *
 * st_mobile_effect_set_beauty_strength(handle, EFFECT_BEAUTY_RESHAPE_NARROW_FACE, 1.0f);
 * st_mobile_effect_add_package(handle, "../makeup_lip.zip", nullptr);
 *
 * st_effect_texture_t effect_in_tex = { in_tex, width, height, ST_PIX_FMT_RGBA8888 };
 * st_effect_texture_t effect_out_tex = { out_tex, width, height, ST_PIX_FMT_RGBA8888 };
 *
 * st_effect_render_in_param_t in_param = {};
 * in_param.p_human = &human_action;
 * in_param.p_tex = &effect_in_tex;
 *
 * st_effect_render_out_param_t out_param = {};
 * out_param.p_human = &another_human_action;
 * out_param.p_tex = &effect_out_tex;
 *
 * st_mobile_effect_render(handle, &in_param, &out_param);
 *
 * st_mobile_effect_destroy_handle(handle);
 * handle = NULL;
 * */

#ifndef _ST_MOBILE_EFFECT_H_
#define _ST_MOBILE_EFFECT_H_

#include "st_mobile_common.h"
#include "st_mobile_human_action.h"
#include "st_mobile_animal.h"

#include <stdint.h>

#define EFFECT_MAX_NAME_LEN 256

/// \~chinese
/// @brief Effect handle配置模式
/// \~english
/// @brief Effect handle configuration mode
typedef enum {
    EFFECT_CONFIG_NONE              = 0x0,      ///< \~chinese 默认配置 \~english Default configuration
    EFFECT_CONFIG_IMAGE_MODE        = 0x2,      ///< \~chinese 图片模式, 添加素材后立即生效 \~english Image mode, effective immediately after adding material
    EFFECT_CONFIG_INNER_RENDER_CONTEXT  = 0x4,  ///< \~chinese 内置渲染context \~english Built-in rendering context
} st_effect_handle_config_t;

/// \~chinese
/// @brief 内存buffer数据块定义
/// \~english
/// @brief Memory buffer data block definition
typedef struct {
    char* data_ptr;     ///< \~chinese 数据的起始地址 \~english Starting address of data
    uint32_t data_len;  ///< \~chinese 数据的长度 \~english Length of data
} st_effect_buffer_t;

/// \~chinese
/// @brief 创建特效句柄
/// @param[in] config 素材生效模式
/// @param[out] handle 特效句柄指针
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Create an effect handle
/// @param[in] config Material activation mode
/// @param[out] handle Pointer to effect handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_create_handle(uint32_t config, st_handle_t* p_handle);

/// \~chinese
/// @brief 设备类型
/// \~english
/// @brief device type
typedef enum {
    EFFECT_DEVICE_TYPE_METAL    ///< \~chinese Metal后端 \~english Metal backend
} st_effect_device_type_t;

/// \~chinese
/// @brief 设备类型及相关的参数
/// \~english
/// @brief device type and the param
typedef struct {
    st_effect_device_type_t type;   ///< \~chinise 设备类型 \~english device type
    void* user_data;                ///< \~chinese iOS使用metal后端时command queue信息 \~english when using the metal backend on iOS, the currently actived command queue.
} st_effect_device_info_t;

/// \~chinese
/// @brief 创建特效句柄
/// @param[in] config 素材生效模式
/// @param[in] device_info 设备信息，目前仅支持iOS使用metal后端时传入MTLCommandQueue
/// @param[out] handle 特效句柄指针
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Create an effect handle
/// @param[in] config Material activation mode
/// @param[in] device_info for device information, currently, only iOS using the Metal backend is supported, where a MTLCommandQueue should be passed.
/// @param[out] handle Pointer to effect handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_create_handle_ex(uint32_t config, const st_effect_device_info_t* device_info, st_handle_t* p_handle);


/// \~chinese
/// @brief 销毁特效句柄, 需在OpenGL渲染线程中执行
/// @param[in] handle 已初始化的特效句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Destroy effect handle, must be executed in OpenGL rendering thread
/// @param[in] handle Initialized effect handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_destroy_handle(st_handle_t handle);


/// \~chinese
/// @brief Effect句柄级别的配置项, 影响整个句柄。需在OpenGL渲染线程中执行
/// \~english
/// @brief Effect handle level configuration options, affecting the entire handle. Must be executed in the OpenGL rendering thread
typedef enum {
    EFFECT_PARAM_MIN_FRAME_INTERVAL,        ///< \~chinese 贴纸前后两个序列帧切换的最小时间间隔，单位为毫秒。当两个相机帧处理的间隔小于这个值的时候，当前显示的贴纸序列帧会继续显示，直到显示的时间大于该设定值贴纸才会切换到下一阵，相机帧不受影响。 \~english The minimum time interval between two sequential frames of stickers, in milliseconds. When the interval between two camera frame processing is less than this value, the current displayed sticker sequential frame will continue to display until the displayed time is greater than this set value, and then the sticker will switch to the next formation, and the camera frame will not be affected.

    EFFECT_PARAM_MAX_MEMORY_BUDGET_MB,      ///< \~chinese 设置贴纸素材资源所占用的最大内存（MB），当估算内存超过这个值时，将不能再加载新的素材包 \~english Set the maximum memory (MB) occupied by sticker material resources. When the estimated memory exceeds this value, new material packages can no longer be loaded.

    EFFECT_PARAM_QUATERNION_SMOOTH_FRAME,   ///< \~chinese 设置相机姿态平滑参数，表示平滑多少帧, 越大延迟越高，抖动越微弱 \~english Set camera attitude smoothing parameters, indicating how many frames to smooth, the larger the delay, the weaker the jitter.

    EFFECT_PARAM_USE_INPUT_TIMESTAMP,       ///< \~chinese 设置贴纸是否使用外部时间戳更新 \~english Set whether the sticker uses an external timestamp update

    EFFECT_PARAM_PREFER_MEMORY_CACHE,       ///< \~chinese 倾向于空间换时间，传0的话，则尽可能清理内部缓存，保持内存最小。目前主要影响2D贴纸以及3D共享资源 \~english Prefer space for time. If 0 is passed, it will clean up the internal cache as much as possible to keep the memory at a minimum. Currently, it mainly affects 2D stickers and 3D shared resources.

    EFFECT_PARAM_DISABLE_BEAUTY_OVERLAP,    ///< \~chinese 传入大于0的值，禁用美颜Overlap逻辑（贴纸中的美颜会覆盖前面通过API或者贴纸生效的美颜效果，贴纸成组覆盖，API单个覆盖），默认启用Overlap \~english Enter a value greater than 0 to disable the beauty overlap logic (the beauty in the sticker will cover the beauty effect that has taken effect through the API or the sticker, the stickers are overlapped in groups, and the API is overlapped individually), overlap is enabled by default

    EFFECT_PARAM_DISABLE_MODULE_REORDER,    ///< \~chinese 传入大于0的值，强制按素材包添加顺序显示贴纸、美妆效果，后添加的在上层显示，该选项只会影响设置之后添加的素材。默认（0）会对v3.1之前的素材包重排module的渲染顺序。重新排序是为了在与美妆、风格素材包叠加时达到最佳效果 \~english Input a value greater than 0 to render modules strictly by add package order. stickers and makeups added late would render on top of others. This option will only affect materials added after setting. default value (0) will reorder the rendering order of modules from packages version less than v3.1. The reorder is to achieve the best effect when overlaying with beauty makeup and style material packages.

    EFFECT_PARAM_3D_POSE_SOLUTION,          ///< \~chinese 3DPose计算方案，传入0使用106旧模型方案，传1使用基于282模型优化的Pose方案，默认值为1 \~english 3DPose calculation solution, input 0 to use the old 106 model solution, input 1 to use the Pose solution optimized based on the 282 model, the default value is 1.

    EFFECT_PARAM_RENDER_DELAY_FRAME,        ///< \~chinese 设置未来帧帧数，默认值是0, 需要是大于等于0的值，0表示不开未来帧 \~english Set the number of future frames, the default value is 0, it needs to be a value greater than or equal to 0, 0 indicates that future frames are not opened.

    EFFECT_PARAM_GREEN_COLOR_BALANCE,       ///< \~chinese 设置去绿程度，0表示不去绿，1表示最大程度去绿，默认值为1 \~english Set the degree of green removal, 0 indicates no green removal, 1 indicates the maximum degree of green removal, the default value is 1.

    EFFECT_PARAM_GREEN_SPILL_BY_ALPHA,      ///< \~chinese 设置去绿色彩平衡, 和去绿程度配套使用, 平衡因去绿导致的主体颜色变化, 范围[0.0, 1.0], 默认0.5(不进行平衡) \~english Set green removal color balance, used in conjunction with green removal intensity to balance the color change caused by green removal. Range [0.0, 1.0], default 0.5 (no balance).

    EFFECT_PARAM_EXPLICIT_OUT_TEX_BLIT,     ///< \~chinese 设置Render接口的输出纹理是否做一次显示拷贝，在传入的输入纹理与D3D纹理绑定共享时需要传1弃用，默认0不做。 \~english set whether do an explicit texture copy for render output texture.  When output texture is bound with a D3D texture, this option needs turn on with 1, default is 0 (disabled).

    EFFECT_PARAM_GREEN_SEGMENT_COLOR,       /// \~chinese 设置绿幕分割颜色RGB值,默认为绿色,将颜色值（16进制数0xFFFFFF,按R、G、B排列）按float类型传入 \~english Set the RGB color value for green screen segmentation. The default color is green (hexadecimal value 0xFFFFFF, arranged as R, G, B). The color value should be passed as a float type

    EFFECT_PARAM_PLASTIC_FACE_OCCLUSION,    ///< \~chinese 微整形效果遮挡，目前支持白牙、亮眼，默认值为0, 表示没有效果遮挡，1表示开启效果遮挡 \~english MicroPlastic effect occlusion, and currently supports teeth whitening and eye brightening. default value is 0, means no effect occlusion, 1 means open effect occlusion.

    EFFECT_PARAM_MAKEUP_PERFORMANCE_HINT,   ///< \~chinese 设置美妆性能/效果优先级倾向，性能优先适用于多人脸的场景，引擎内部会根据设置调整渲染策略, 0表示效果优先， 1表示性能优先，默认值为0 \~english Configure the priority preference for makeup performance/effects. Performance priority is suitable for scenarios with multiple faces, and the engine internally will adjust the rendering strategy according to the setting. A value of 0 indicates a preference for effects, while a value of 1 indicates a preference for performance. The default value is 0.

    EFFECT_PARAM_DISABLE_TRIGER_OVERLAP,    ///< \~chinese 传入大于0的值，禁用贴纸触发逻辑对Overlap的影响（贴纸中的美颜触发会覆盖前面通过API或者贴纸生效的美颜效果），默认触发逻辑会对Overlap的影响 \~english Pass a value greater than 0 to disable the effect of sticker trigger logic on Overlap (beautification triggered by stickers will override the beautification effects applied earlier through API or stickers). By default, the trigger logic will affect Overlap.

} st_effect_param_t;

/// \~chinese
/// @brief 设置特效的参数
/// @param[in] handle 已初始化的贴纸句柄
/// @param[in] param 参数类型
/// @param[in] val 参数数值，具体范围参考参数类型说明
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Sets the parameters for the effect
/// @param[in] handle Initialized sticker handle
/// @param[in] param Parameter type
/// @param[in] val Parameter value, specific range refer to parameter type description
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_param(st_handle_t handle, st_effect_param_t param, float val);

/// \~chinese
/// @brief 获取特效的参数
/// @param[in] handle 已初始化的贴纸句柄
/// @param[in] param 参数类型
/// @param[out] val 参数数值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Gets the parameters for the effect
/// @param[in] handle Initialized sticker handle
/// @param[in] param Parameter type
/// @param[out] val Parameter value
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_param(st_handle_t handle, st_effect_param_t param, float* val);

/// \~chinese
/// @brief 自定义参数
/// \~english
/// @brief Custom parameters
typedef struct {
    st_quaternion_t camera_quat;    ///< \~chinese 相机姿态的四元数，默认四元数(0, 0, 0, 1)对应的姿态是相机水平向下，头向北。不需要传入该数据时，\~english Quaternion of the camera's posture, the default quaternion (0, 0, 0, 1) corresponds to the camera pointing downwards horizontally, heading north. When this data is not needed,
                                    ///< \~chinese st_effect_custom_param_t传入空指针即可，传空时，SDK内部的默认朝向是相机竖直朝向-Z轴，头向上。 \~english pass a null pointer to st_effect_custom_param_t, when null, the SDK's default direction is the camera vertically pointing to the -Z axis, head up.
    bool front_camera;              ///< \~chinese 是否为前置相机 \~english Whether it's the front camera
    int event;                      ///< \~chinese 用户定义的事件id \~english User-defined event id
} st_effect_custom_param_t;

/// \~chinese
/// @brief 自定义参数配置
/// \~english
/// @brief Custom parameter configuration
typedef enum {
    EFFECT_CUSTOM_NONE              = 0X0,  ///< \~chinese 不需要自定义参数 \~english No need for custom parameters
    EFFECT_CUSTOM_CAMERA_QUATION    = 0x1,  ///< \~chinese 需要相机姿态信息 \~english Camera posture information needed
    EFFECT_CUSTOM_CAMERA_FACING     = 0x2,  ///< \~chinese 需要前后相机信息 \~english Front and rear camera information needed
} st_effect_custom_param_config_t;

/// \~chinese
/// @brief 获取需要的检测配置选项
/// @param[in] handle 已初始化的特效句柄
/// @param[out] p_detect_config 返回检测配置选项, 每一位分别代表该位对应检测选项, 对应状态详见st_mobile_human_action.h中, 如ST_MOBILE_FACE_DETECT等
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Gets the required detection configuration options
/// @param[in] handle Initialized effect handle
/// @param[out] p_detect_config Returns detection configuration options. Each bit separately represents the detection option corresponding to that bit. Corresponding states are detailed in st_mobile_human_action.h, such as ST_MOBILE_FACE_DETECT etc.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_detect_config(st_handle_t handle, uint64_t* p_detect_config);

/// \~chinese
/// @brief 获取特定检测config对应的触发Action，目前主要是手势检测存在不同Action
/// @param[in] handle 已初始化的特效句柄
/// @param[in] config 需要查询触发动作的检测config，目前主要是手势（ST_MOBILE_DETECT_HAND_GESTURE）
/// @param[out] p_trigger_actions 返回当前贴纸需要的触发动作, 每一位分别代表该位对应动作选项, 对应状态详见st_mobile_human_action.h中, 如ST_HAND_ACTION_TYPE_OK等
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief get the trigger action of specific detect config. Currently, only hand gesture has various actions.
/// @param[in] handle Initialized effect handle
/// @param[in] config detect config needs to query trigger actions. currenlty ST_MOBILE_DETECT_HAND_GESTURE
/// @param[out] p_trigger_actions Returns trigger actions needs by current config, one example is ST_HAND_ACTION_TYPE_OK.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_human_trigger_actions(st_handle_t handle, uint64_t config, uint64_t *p_trigger_actions);

#define EFFECT_CUSTOM_INPUT_EVENT_SCREEN_TAP            0x10000
#define EFFECT_CUSTOM_INPUT_EVENT_SCREEN_DOUBLE_TAP     0x20000

/// \~chinese
/// @brief 获取需要的自定义事件选项
/// @param[in] handle 已初始化的特效句柄
/// @param[out] p_custom_event_config 返回自定义事件选项
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Gets the required custom event options
/// @param[in] handle Initialized effect handle
/// @param[out] p_custom_event_config Returns custom event options
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_custom_event_config(st_handle_t handle, uint64_t* p_custom_event_config);

/// \~chinese
/// @brief 获取前摄/后摄对应的默认手机姿态四元数，在处理图片、视频或者没有相应的手机姿态的情况下，需要传入默认的camera_quat
/// @param[in] handle 已初始化的特效句柄
/// @param[in] b_front_camera 指定当前获取的默认手机姿态四元数对应前摄像头还是后摄像头
/// @param[out] p_default_quat 返回的默认四元数
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Gets the default phone pose quaternion for the front/back camera. When processing images or videos, or when there is no corresponding phone pose, a default camera_quat needs to be passed in.
/// @param[in] handle Initialized effect handle
/// @param[in] b_front_camera Specifies whether the current default phone pose quaternion corresponds to the front camera or the rear camera
/// @param[out] p_default_quat Returns the default quaternion
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_default_camera_quaternion(st_handle_t handle, bool b_front_camera, st_quaternion_t *p_default_quat);

/// \~chinese
/// @brief 获取目前需要的动物检测类型
/// @param[in] handle 已初始化的特效句柄
/// @param[out] p_detect_config 返回的需要检测的类别，对应状态详见st_mobile_common.h中, 如ST_MOBILE_CAT_DETECT等
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Gets the current required animal detection type
/// @param[in] handle Initialized effect handle
/// @param[out] p_detect_config Returns the category that needs to be detected. Corresponding states are detailed in st_mobile_common.h, such as ST_MOBILE_CAT_DETECT etc.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_animal_detect_config(st_handle_t handle, uint64_t* p_detect_config);

/// \~chinese
/// @brief 获取自定义配置
/// @param[in] handle 已初始化的特效句柄
/// @param[out] p_custom_param_config 返回的自定义配置选项, 每一位分别代表该位的检测选项, 对应的状态详见st_effect_custom_param_config_t
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Gets custom configuration
/// @param[in] handle Initialized effect handle
/// @param[out] p_custom_param_config Returns custom configuration options. Each bit separately represents the detection option of that bit. The corresponding states are detailed in st_effect_custom_param_config_t.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_custom_param_config(st_handle_t handle, uint64_t* p_custom_param_config);

/// \~chinese
/// @brief 渲染输入的图片信息
/// \~english
/// @brief Render input image information
typedef struct {
    st_image_t image;                           ///< \~chinese 输入的图片信息 \~english Input image information
    st_rotate_type rotate;                      ///< \~chinese 输入图片的旋转信息（相对于输入的texture而言） \~english Rotation information of the input image (relative to the input texture)
    bool b_mirror;                              ///< \~chinese 输入图片是否水平镜像 \~english Whether the input image is horizontally mirrored
} st_effect_in_image_t;

/// \~chinese
/// @brief 渲染的输入参数
/// \~english
/// @brief Render input parameters
typedef struct {
    st_mobile_human_action_t* p_human;          ///< \~chinese 人脸检测结果 \~english Human face detection result

    st_mobile_animal_result_t* p_animal;        ///< \~chinese 动物脸检测结果 \~english Animal face detection result

    st_rotate_type rotate;                      ///< \~chinese 人脸朝向 \~english Human face orientation
    st_rotate_type front_rotate;                ///< \~chinese 前景渲染朝向 \~english Foreground rendering direction
    bool need_mirror;                           ///< \~chinese 是否需要镜像 \~english Need mirror or not
    st_effect_custom_param_t* p_custom_param;   ///< \~chinese 自定义参数配置 \~english Custom parameter configuration
    st_mobile_texture_t* p_tex;                 ///< \~chinese 输入的纹理信息 \~english Input texture information
    st_effect_in_image_t* p_image;              ///< \~chinese 输入的图片信息 \~english Input image information
    double time_stamp;                          ///< \~chinese 输入的时间戳 \~english Input timestamp
} st_effect_render_in_param_t;

/// \~chinese
/// @brief 渲染的输出参数
/// \~english
/// @brief Render output parameters
typedef struct {
    st_mobile_texture_t* p_tex;                 ///< \~chinese 输出的纹理信息 \~english Output texture information
    st_image_t* p_image;                        ///< \~chinese 输出的图片信息 \~english Output image information
    st_mobile_human_action_t* p_human;          ///< \~chinese 经过内部美颜、贴纸影响（反算）后的humanAction结果信息 \~english HumanAction result information after internal beautification and sticker effect (backward calculation)
} st_effect_render_out_param_t;

/// \~chinese
/// @brief 特效渲染, 必须在OpenGL渲染线程中执行
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_in_param 输入的渲染信息
/// @param[out] p_out_param 输出的渲染信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Effect rendering, must be executed in OpenGL rendering thread
/// @param[in] handle Initialized effect handle
/// @param[in] p_in_param Input rendering information
/// @param[out] p_out_param Output rendering information
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_render(st_handle_t handle, const st_effect_render_in_param_t* p_in_param, st_effect_render_out_param_t* p_out_param);


/// \~chinese @brief 美颜类型
/// \~english @brief Beauty type
typedef enum {
    // \~chinese 基础美颜 base
    // \~english Basic beauty group
    EFFECT_BEAUTY_BASE_WHITTEN                      = 101,  ///< \~chinese 美白, [0,1.0], 默认值0.30, 0.0不做美白 \~english Whitening, [0,1.0], default value 0.30, 0.0 means no whitening
    EFFECT_BEAUTY_BASE_REDDEN                       = 102,  ///< \~chinese 红润, [0,1.0], 默认值0.36, 0.0不做红润 \~english Redden, [0,1.0], default value 0.36, 0.0 means no reddening
    EFFECT_BEAUTY_BASE_FACE_SMOOTH                  = 103,  ///< \~chinese 磨皮, [0,1.0], 默认值0.74, 0.0不做磨皮 \~english Face smoothing, [0,1.0], default value 0.74, 0.0 means no face smoothing
    EFFECT_BEAUTY_BASE_SKIN_SMOOTH                  = 104,  ///< \~chinese 身体皮肤祛皱, [0,1.0],默认值0.0, 0.0不做身体皮肤祛皱 \~english Body skin wrinkle removal, [0, 1.0], default value 0.0, 0.0 disables wrinkle removal

    // \~chinese 美形 reshape
    // \~english Reshape beauty group
    EFFECT_BEAUTY_RESHAPE_SHRINK_FACE               = 201,  ///< \~chinese 瘦脸, [0,1.0], 默认值0.11, 0.0不做瘦脸效果 \~english Face slimming, [0,1.0], default value 0.11, 0.0 means no face slimming effect
    EFFECT_BEAUTY_RESHAPE_ENLARGE_EYE               = 202,  ///< \~chinese 大眼, [0,1.0], 默认值0.13, 0.0不做大眼效果 \~english Eye enlargement, [0,1.0], default value 0.13, 0.0 means no eye enlargement effect
    EFFECT_BEAUTY_RESHAPE_SHRINK_JAW                = 203,  ///< \~chinese 小脸, [0,1.0], 默认值0.10, 0.0不做小脸效果 \~english Jaw slimming, [0,1.0], default value 0.10, 0.0 means no jaw slimming effect
    EFFECT_BEAUTY_RESHAPE_NARROW_FACE               = 204,  ///< \~chinese 窄脸, [0,1.0], 默认值0.0, 0.0不做窄脸 \~english Narrow face, [0,1.0], default value 0.0, 0.0 means no narrow face effect
    EFFECT_BEAUTY_RESHAPE_ROUND_EYE                 = 205,  ///< \~chinese 圆眼, [0,1.0], 默认值0.0, 0.0不做圆眼 \~english Round eyes, [0,1.0], default value 0.0, 0.0 means no round eyes effect

    // \~chinese 微整形 plastic
    // \~english Micro Plastic group
    EFFECT_BEAUTY_PLASTIC_THINNER_HEAD              = 301,  ///< \~chinese 小头, [0, 1.0], 默认值0.0, 0.0不做小头效果 \~english Smaller head, [0, 1.0], default value 0.0, 0.0 means no smaller head effect
    EFFECT_BEAUTY_PLASTIC_THIN_FACE                 = 302,  ///< \~chinese 瘦脸型，[0,1.0], 默认值0.0, 0.0不做瘦脸型效果 \~english Thinner face shape, [0,1.0], default value 0.0, 0.0 means no thinner face shape effect
    EFFECT_BEAUTY_PLASTIC_CHIN_LENGTH               = 303,  ///< \~chinese 下巴，[-1, 1], 默认值为0.0，[-1, 0]为短下巴，[0, 1]为长下巴 \~english Chin length, [-1, 1], default value is 0.0, [-1, 0] means shorter chin, [0, 1] means longer chin
    EFFECT_BEAUTY_PLASTIC_HAIRLINE_HEIGHT           = 304,  ///< \~chinese 额头，[-1, 1], 默认值为0.0，[-1, 0]为低发际线，[0, 1]为高发际线 \~english Hairline height, [-1, 1], default value is 0.0, [-1, 0] means lower hairline, [0, 1] means higher hairline
    EFFECT_BEAUTY_PLASTIC_APPLE_MUSLE               = 305,  ///< \~chinese 苹果肌，[0, 1.0]，默认值为0.0，0.0不做苹果肌 \~english Apple muscle, [0, 1.0], default value is 0.0, 0.0 means no apple muscle effect
    EFFECT_BEAUTY_PLASTIC_NARROW_NOSE               = 306,  ///< \~chinese 瘦鼻翼，[0, 1.0], 默认值为0.0，0.0不做瘦鼻 \~english Narrow nose wings, [0, 1.0], default value is 0.0, 0.0 means no narrow nose wings effect
    EFFECT_BEAUTY_PLASTIC_NOSE_LENGTH               = 307,  ///< \~chinese 长鼻，[-1, 1], 默认值为0.0, [-1, 0]为短鼻，[0, 1]为长鼻 \~english Longer nose, [-1, 1], default value is 0.0, [-1, 0] means shorter nose, [0, 1] means longer nose
    EFFECT_BEAUTY_PLASTIC_PROFILE_RHINOPLASTY       = 308,  ///< \~chinese 侧脸隆鼻，[0, 1.0]，默认值为0.0，0.0不做侧脸隆鼻效果 \~english Profile rhinoplasty, [0, 1.0], default value is 0.0, 0.0 means no profile rhinoplasty effect
    EFFECT_BEAUTY_PLASTIC_MOUTH_SIZE                = 309,  ///< \~chinese 嘴型，[-1, 1]，默认值为0.0，[-1, 0]为放大嘴巴，[0, 1]为缩小嘴巴 \~english Mouth shape, [-1, 1], default value is 0.0, [-1, 0] means larger mouth, [0, 1] means smaller mouth
    EFFECT_BEAUTY_PLASTIC_PHILTRUM_LENGTH           = 310,  ///< \~chinese 缩人中，[-1, 1], 默认值为0.0，[-1, 0]为长人中，[0, 1]为短人中 \~english Philtrum length, [-1, 1], default value is 0.0, [-1, 0] means longer philtrum, [0, 1] means shorter philtrum
    EFFECT_BEAUTY_PLASTIC_EYE_DISTANCE              = 311,  ///< \~chinese 眼距，[-1, 1]，默认值为0.0，[-1, 0]为减小眼距，[0, 1]为增加眼距 \~english Eye distance, [-1, 1], default value is 0.0, [-1, 0] means reducing eye distance, [0, 1] means increasing eye distance
    EFFECT_BEAUTY_PLASTIC_EYE_ANGLE                 = 312,  ///< \~chinese 眼睛角度，[-1, 1]，默认值为0.0，[-1, 0]为左眼逆时针旋转，[0, 1]为左眼顺时针旋转，右眼与左眼相对 \~english Eye angle, [-1, 1], default value is 0.0, [-1, 0] means rotating left eye counterclockwise, [0, 1] means rotating left eye clockwise, relative to right eye
    EFFECT_BEAUTY_PLASTIC_OPEN_CANTHUS              = 313,  ///< \~chinese 开眼角，[0, 1.0]，默认值为0.0， 0.0不做开眼角 \~english Eye opening, [0, 1.0], default value is 0.0, 0.0 means no eye opening effect
    EFFECT_BEAUTY_PLASTIC_BRIGHT_EYE                = 314,  ///< \~chinese 亮眼，[0, 1.0]，默认值为0.0，0.0不做亮眼 \~english Brighten eyes, [0, 1.0], default value is 0.0, 0.0 means no eye brightening effect
    EFFECT_BEAUTY_PLASTIC_REMOVE_DARK_CIRCLES       = 315,  ///< \~chinese 祛黑眼圈，[0, 1.0]，默认值为0.0，0.0不做去黑眼圈 \~english Remove dark circles, [0, 1.0], default value is 0.0, 0.0 means no dark circles removal effect
    EFFECT_BEAUTY_PLASTIC_REMOVE_NASOLABIAL_FOLDS   = 316,  ///< \~chinese 祛法令纹，[0, 1.0]，默认值为0.0，0.0不做去法令纹 \~english Remove nasolabial folds, [0, 1.0], default value is 0.0, 0.0 means no nasolabial folds removal effect
    EFFECT_BEAUTY_PLASTIC_WHITE_TEETH               = 317,  ///< \~chinese 白牙，[0, 1.0]，默认值为0.0，0.0不做白牙 \~english White teeth, [0, 1.0], default value is 0.0, 0.0 means no teeth whitening effect
    EFFECT_BEAUTY_PLASTIC_SHRINK_CHEEKBONE          = 318,  ///< \~chinese 瘦颧骨， [0, 1.0], 默认值0.0， 0.0不做瘦颧骨 \~english Shrink cheekbones, [0, 1.0], default value is 0.0, 0.0 means no cheekbone shrinking effect
    EFFECT_BEAUTY_PLASTIC_OPEN_EXTERNAL_CANTHUS     = 319,  ///< \~chinese 开外眼角比例，[0, 1.0]，默认值为0.0， 0.0不做开外眼角 \~english Open external canthus, [0, 1.0], default value is 0.0, 0.0 means no opening of the outer canthus effect
    EFFECT_BEAUTY_PLASTIC_SHRINK_JAWBONE            = 320,  ///< \~chinese 瘦下颔，[0, 1.0], 默认值0.0， 0.0不做瘦下颔 \~english Shrink jawbone, [0, 1.0], default value is 0.0, 0.0 means no jawbone shrinking effect
    EFFECT_BEAUTY_PLASTIC_SHRINK_ROUND_FACE         = 321,  ///< \~chinese 圆脸瘦脸，[0, 1.0], 默认值0.0， 0.0不做瘦脸 \~english Shrink round face, [0, 1.0], default value is 0.0, 0.0 means no face slimming effect for round face
    EFFECT_BEAUTY_PLASTIC_SHRINK_LONG_FACE          = 322,  ///< \~chinese 长脸瘦脸，[0, 1.0], 默认值0.0， 0.0不做瘦脸 \~english Shrink long face, [0, 1.0], default value is 0.0, 0.0 means no face slimming effect for long face
    EFFECT_BEAUTY_PLASTIC_SHRINK_GODDESS_FACE       = 323,  ///< \~chinese 女神瘦脸，[0, 1.0], 默认值0.0， 0.0不做瘦脸 \~english Shrink goddess face, [0, 1.0], default value is 0.0, 0.0 means no face slimming effect for goddess face
    EFFECT_BEAUTY_PLASTIC_SHRINK_NATURAL_FACE       = 324,  ///< \~chinese 自然瘦脸，[0, 1.0], 默认值0.0， 0.0不做瘦脸 \~english Shrink natural face, [0, 1.0], default value is 0.0, 0.0 means no face slimming effect for natural face
    EFFECT_BEAUTY_PLASTIC_SHRINK_WHOLE_HEAD         = 325,  ///< \~chinese 整体缩放小头，[0, 1.0], 默认值0.0, 0.0不做整体缩放小头效果 \~english Shrink the whole head, [0, 1.0], default value is 0.0, 0.0 means no shrinking of the whole head effect
    EFFECT_BEAUTY_PLASTIC_EYE_HEIGHT                = 326,  ///< \~chinese 眼睛位置比例，[-1, 1]，默认值0.0, [-1, 0]向下移动眼睛，[0, 1]向上移动眼睛 \~english Eye position ratio, [-1, 1], default value is 0.0, [-1, 0] means moving eyes downward, [0, 1] means moving eyes upward
    EFFECT_BEAUTY_PLASTIC_MOUTH_CORNER              = 327,  ///< \~chinese 嘴角上移比例，[0, 1.0]，默认值0.0, 0.0不做嘴角调整 \~english Mouth corner lifting ratio, [0, 1.0], default value is 0.0, 0.0 means no mouth corner adjustment
    EFFECT_BEAUTY_PLASTIC_HAIRLINE                  = 328,  ///< \~chinese 新发际线高低比例，[-1, 1]，默认值0.0, [-1, 0]为低发际线，[0, 1]为高发际线 \~english Hairline height ratio, [-1, 1], default value is 0.0, [-1, 0] means lower hairline, [0, 1] means higher hairline
    EFFECT_BEAUTY_PLASTIC_FULLER_LIPS               = 329,  ///< \~chinese 丰唇，[-1, 1]，默认值0.0，[-1, 0]为嘴唇变薄，[0, 1]为丰唇 \~english Lip fullness adjustment, [-1, 1], default value is 0.0; [-1, 0] reduces lip fullness, [0, 1] increases lip fullness
    EFFECT_BEAUTY_PLASTIC_MOUTH_WIDTH               = 330,  ///< \~chinese 嘴巴宽度，[-1, 1]，默认值0.0，[-1, 0]为嘴巴变宽，[0, 1]为嘴巴变窄 \~english Mouth width adjustment, [-1, 1], default value is 0.0; [-1, 0] increases width, [0, 1] decreases width
    EFFECT_BEAUTY_PLASTIC_BROW_HEIGHT               = 331,  ///< \~chinese 眉毛高度，[-1, 1]，默认值0.0，[-1, 0]为眉毛上移，[0, 1]为眉毛下移 \~english Eyebrow height adjustment, [-1, 1], default value is 0.0; [-1, 0] raises brows, [0, 1] lowers brows
    EFFECT_BEAUTY_PLASTIC_BROW_THICKNESS            = 332,  ///< \~chinese 眉毛粗细，[-1, 1]，默认值0.0，[-1, 0]为眉毛变细，[0, 1]为眉毛增粗 \~english Eyebrow thickness adjustment, [-1, 1], default value is 0.0; [-1, 0] makes brows thinner, [0, 1] makes brows thicker
    EFFECT_BEAUTY_PLASTIC_BROW_DISTANCE             = 333,  ///< \~chinese 眉毛间距，[-1, 1]，默认值0.0，[-1, 0]为眉毛间距变大，[0, 1]为眉毛间距变小 \~english Eyebrow distance adjustment, [-1, 1], default value is 0.0; [-1, 0] increases distance between brows, [0, 1] decreases it
    EFFECT_BEAUTY_PLASTIC_FACE_V_SHAPE              = 334,  ///< \~chinese V脸，从下颌角到下巴的V脸效果，[0, 1.0]，默认值0.0, 0.0不做V脸 \~english V-shaped face effect (jawline to chin), [0, 1.0], default value is 0.0; 0.0 applies no V-shape effect
    EFFECT_BEAUTY_PLASTIC_FACE_FULL_V_SHAPE         = 335,  ///< \~chinese V脸，整脸的V脸效果，[0, 1.0]，默认值0.0, 0.0不做V脸 \~english Full V-shaped face effect, [0, 1.0], default value is 0.0; 0.0 applies no V-shape effect
    EFFECT_BEAUTY_PLASTIC_NOSE_TIP                  = 336,  ///< \~chinese 瘦鼻头，[0, 1.0]，默认值0.0, 0.0不做瘦鼻头 \~english Nose tip refinement, [0, 1.0], default value is 0.0; 0.0 applies no nose tip refinement
    EFFECT_BEAUTY_PLASTIC_NOSE_BRIDGE               = 337,  ///< \~chinese 鼻梁调整，[0, 1.0]，默认值0.0, 0.0不做鼻梁调整 \~english Nose bridge adjustment, [0, 1.0], default value is 0.0; 0.0 applies no nose bridge adjustment
    EFFECT_BEAUTY_PLASTIC_ENLARGE_PUPIL             = 338,  ///< \~chinese 放大瞳孔，[0, 1.0]，默认值0.0, 0.0不做瞳孔放大 \~english Pupil enlargement, [0, 1.0], default value is 0.0; 0.0 applies no pupil enlargement

    // \~chinese 调整 tone
    // \~english Tone adjustment group
    EFFECT_BEAUTY_TONE_CONTRAST                     = 601,  ///< \~chinese 对比度, [0,1.0], 默认值0.05, 0.0不做对比度处理 \~english Contrast, [0,1.0], default value 0.05, 0.0 means no contrast adjustment
    EFFECT_BEAUTY_TONE_SATURATION                   = 602,  ///< \~chinese 饱和度, [0,1.0], 默认值0.10, 0.0不做饱和度处理 \~english Saturation, [0,1.0], default value 0.10, 0.0 means no saturation adjustment
    EFFECT_BEAUTY_TONE_SHARPEN                      = 603,  ///< \~chinese 锐化, [0, 1.0], 默认值0.0, 0.0不做锐化 \~english Sharpening, [0, 1.0], default value 0.0, 0.0 means no sharpening
    EFFECT_BEAUTY_TONE_CLEAR                        = 604,  ///< \~chinese 清晰度, 清晰强度, [0,1.0], 默认值0.0, 0.0不做清晰 \~english Clarity, clarity strength, [0,1.0], default value 0.0, 0.0 means no clarity adjustment
    EFFECT_BEAUTY_TONE_DENOISING                    = 606,  ///< \~chinese 去噪，[0, 1.0], 默认值0.0, 0.0不做去噪处理 \~english denoising intensity, [0, 1.0], default value 0.0 means off
    EFFECT_BEAUTY_TONE_COLOR_TONE                   = 607,  ///< \~chinese 色调，[-1.0, 1.0], 默认值0.0, 0.0不做色调处理 \~english Color tone adjustment, [-1.0, 1.0], default value 0.0 means off
    EFFECT_BEAUTY_TONE_COLOR_TEMPERATURE            = 608,  ///< \~chinese 色温，[-1.0, 1.0], 默认值0.0, 0.0不做色温处理 \~english Color tempareture adjustment, [-1.0, 1.0], default value 0.0 means off

    // \~chinese 背景虚化，是单独的一个组，不与其他美颜组相互overlap
    // \~english bokeh group
    EFFECT_BEAUTY_TONE_BOKEH                        = 605,  ///< \~chinese 背景虚化强度, [0,1.0], 默认值0.0, 0.0不做背景虚化 \~english Bokeh intensity, [0,1.0], default value 0.0, 0.0 means no bokeh effect

    // \~chinese 美妆 makeup
    // \~english Makeup group
    EFFECT_BEAUTY_MAKEUP_HAIR_DYE                   = 401,  ///< \~chinese 染发 \~english Hair dye
    EFFECT_BEAUTY_MAKEUP_LIP                        = 402,  ///< \~chinese 口红 \~english Lipstick
    EFFECT_BEAUTY_MAKEUP_CHEEK                      = 403,  ///< \~chinese 腮红 \~english Blush
    EFFECT_BEAUTY_MAKEUP_NOSE                       = 404,  ///< \~chinese 修容 \~english Nose contouring
    EFFECT_BEAUTY_MAKEUP_EYE_BROW                   = 405,  ///< \~chinese 眉毛 \~english Eyebrow
    EFFECT_BEAUTY_MAKEUP_EYE_SHADOW                 = 406,  ///< \~chinese 眼影 \~english Eyeshadow
    EFFECT_BEAUTY_MAKEUP_EYE_LINE                   = 407,  ///< \~chinese 眼线 \~english Eyeliner
    EFFECT_BEAUTY_MAKEUP_EYE_LASH                   = 408,  ///< \~chinese 眼睫毛 \~english Eyelash
    EFFECT_BEAUTY_MAKEUP_EYE_BALL                   = 409,  ///< \~chinese 美瞳 \~english Eye color
    EFFECT_BEAUTY_MAKEUP_PACKED                     = 410,  ///< \~chinese 打包的美妆素材，可能包含一到多个单独的美妆模块，与其他单独美妆可以同时存在 \~english Packed makeup material, may contain one or more individual makeup modules, can coexist with other individual makeups
    EFFECT_BEAUTY_MAKEUP_EYE_PAINTING               = 411,  ///< \~chinese 眼妆 \~english Eye painting

    // \~chinese 滤镜 filter
    // \~english Filter group
    EFFECT_BEAUTY_FILTER                            = 501,  ///< \~chinese 滤镜 \~english Filter

    // \~chinese 试妆 tryon
    // \~english Makeup tryon
    EFFECT_BEAUTY_TRYON_HAIR_COLOR                  = 701,  ///< \~chinese 染发，可设置的参数包括：颜色，强度，明暗度，高光 \~english Hair dye, configurable parameters include: color, intensity, brightness, highlight
    EFFECT_BEAUTY_TRYON_LIPSTICK                    = 702,  ///< \~chinese 口红，可设置的参数包括：颜色，强度，高光(特定材质：水润、闪烁、金属)，质地类型 \~english Lipstick, configurable parameters include: color, intensity, highlight (specific textures: moisturizing, shimmering, metallic), texture type
    EFFECT_BEAUTY_TRYON_LIPLINE                     = 703,  ///< \~chinese 唇线，可设置的参数包括：颜色，强度，唇线线宽 \~english Lip line, configurable parameters include: color, intensity, lip line width
    EFFECT_BEAUTY_TRYON_BLUSH                       = 704,  ///< \~chinese 腮红，可设置的参数包括：颜色，强度 \~english Blush, configurable parameters include: color, intensity
    EFFECT_BEAUTY_TRYON_BROW                        = 705,  ///< \~chinese 眉毛，可设置的参数包括：颜色，强度 \~english Eyebrow, configurable parameters include: color, intensity
    EFFECT_BEAUTY_TRYON_FOUNDATION                  = 706,  ///< \~chinese 粉底，可设置的参数包括：颜色，强度 \~english Foundation, configurable parameters include: color, intensity
    EFFECT_BEAUTY_TRYON_CONTOUR                     = 707,  ///< \~chinese 修容，可设置的参数包括：强度（整体），区域信息（区域id，颜色，强度） \~english Contouring, configurable parameters include: intensity (overall), area information (area id, color, intensity)
    EFFECT_BEAUTY_TRYON_EYESHADOW                   = 708,  ///< \~chinese 眼影，可设置的参数包括：强度（整体），区域信息（区域id，颜色，强度） \~english Eyeshadow, configurable parameters include: intensity (overall), area information (area id, color, intensity)
    EFFECT_BEAUTY_TRYON_EYELINER                    = 709,  ///< \~chinese 眼线，可设置的参数包括：强度（整体），区域信息（区域id，颜色，强度） \~english Eyeliner, configurable parameters include: intensity (overall), area information (area id, color, intensity)
    EFFECT_BEAUTY_TRYON_EYELASH                     = 710,  ///< \~chinese 眼睫毛，可设置的参数包括：颜色，强度 \~english Eyelash, configurable parameters include: color, intensity
    EFFECT_BEAUTY_TRYON_STAMPLINER                  = 711,  ///< \~chinese 眼印，可设置的参数包括：颜色，强度 \~english Eye stamp, configurable parameters include: color, intensity

    // \~chinese 3D 微整形
    // \~english 3D micro plastic
    EFFECT_BEAUTY_3D_MICRO_PLASTIC                  = 801,
} st_effect_beauty_type_t;


/// \~chinese @brief 美颜分组信息, 各个分组包含具体内容请查看st_effect_beauty_type_t
/// \~english @brief Beauty group information, see st_effect_beauty_type_t for more detailed group information
typedef enum {
    EFFECT_BEAUTY_GROUP_BASE,           ///< \~chinese 基础美颜组 \~english Basic beauty group
    EFFECT_BEAUTY_GROUP_RESHAPE,        ///< \~chinese 美型组 \~english Reshape group
    EFFECT_BEAUTY_GROUP_PLASTIC,        ///< \~chinese 微整形组 \~english Micro Plastic group
    EFFECT_BEAUTY_GROUP_TONE,           ///< \~chinese 图像微调组 \~english Tone adjustment group
    EFFECT_BEAUTY_GROUP_BOKEH,          ///< \~chinese 背景模糊微调组 \~english bokeh group
    EFFECT_BEAUTY_GROUP_MAKEUP,         ///< \~chinese 美妆组 \~english Makeup group
    EFFECT_BEAUTY_GROUP_FILTER,         ///< \~chinese 滤镜组 \~english Filter group
} st_effect_beauty_group_t;

/// \~chinese
/// @brief 美颜信息
/// \~english
/// @brief Beauty information
typedef struct {
    st_effect_beauty_type_t type;       ///< \~chinese 美颜类型 \~english Beauty type
    float strength;                     ///< \~chinese 美颜强度 \~english Beauty strength
    char name[EFFECT_MAX_NAME_LEN];     ///< \~chinese 所属的素材包的名字 \~english Name of the material package it belongs to
    int mode;                           ///< \~chinese 美颜的模式 \~english Beauty mode
} st_effect_beauty_info_t;

/// \~chinese
/// @brief 3D美颜信息
/// \~english
/// @brief 3D beauty information
typedef struct {
    char name[EFFECT_MAX_NAME_LEN];
    int part_id;
    float strength;
    float strength_min;
    float strength_max;
} st_effect_3D_beauty_part_info_t;

/// \~chinese
/// @brief 口红质地类型
/// \~english
/// @brief Lipstick finish
typedef enum {
    EFFECT_LIPSTICK_CREAMY,             ///< \~chinese 自然 \~english creamy
    EFFECT_LIPSTICK_MATTE,              ///< \~chinese 哑光 \~english matte
    EFFECT_LIPSTICK_LUSTRE,             ///< \~chinese 水润 \~english lustre
    EFFECT_LIPSTICK_FROST,              ///< \~chinese 闪烁 \~english frost
    EFFECT_LIPSTICK_METAL,              ///< \~chinese 金属 \~english metal
    EFFECT_LIPSTICK_GLOSSY,             ///< \~chinese 光泽 \~english glossy
} st_effect_lipstick_finish_t;

/// \~chinese
/// @brief 试妆区域信息
/// \~english
/// @brief Tryon region information
typedef struct {
    int region_id;                      ///< \~chinese 区域id \~english Region id
    float strength;                     ///< \~chinese 颜色强度，[0, 1.0] \~english Color intensity, [0, 1.0]
    st_color_t color;                   ///< \~chinese 颜色, 每个通道的取值范围是[0, 255], 目前用到了 r, g, b 三个通道 \~english Color, each channel's value range is [0, 255], currently r, g, b channels are used
} st_effect_tryon_region_info_t;

#define REGION_COUNT 6

/// \~chinese
/// @brief 试妆信息
/// \~english
/// @brief Tryon information
typedef struct {
    st_color_t color;                                           ///< \~chinese 颜色, 每个通道的取值范围是[0, 255], 目前用到了 r, g, b 三个通道 \~english Color, each channel's value range is [0, 255], currently r, g, b channels are used
    float strength;                                             ///< \~chinese 颜色强度, [0, 1.0] \~english Color intensity, [0, 1.0]
    float line_width_ratio;                                     ///< \~chinese 唇线线宽比例， [0, 1.0] \~english  Lip line width ratio, [0, 1.0]
    float midtone;                                              ///< \~chinese 明暗度, [0, 1.0], 0.5表示不改变颜色的明暗度 \~english Brightness, [0, 1.0], 0.5 means no change in color brightness
    float highlight;                                            ///< \~chinese 高光, [0, 1.0], 0.0表示不加高光 \~english Highlight, [0, 1.0], 0.0 means no highlight added
    st_effect_lipstick_finish_t lip_finish_type;                ///< \~chinese 口红质地类型 \~english Lipstick finish type

    int region_count;                                           ///< \~chinese 当前效果的区域数量，目前只有修容、眼影、眼线支持多区域，其他试妆效果该变量均为0 \~english Number of regions for the current effect, currently only contouring, eyeshadow, and eyeliner support multiple regions, other tryon effects this variable is 0
    st_effect_tryon_region_info_t region_info[REGION_COUNT];    ///< \~chinese 区域信息，最多支持REGION_COUNT个区域 \~english Region information, supports up to REGION_COUNT regions
} st_effect_tryon_info_t;

/// \~chinese
/// @brief 获取覆盖生效的美颜的数量, 需要在st_mobile_effect_render接口后调用，因为overlap信息是在render之后更新的
/// @param[in] handle 已初始化的特效句柄
/// @param[out] p_info_num 变化的美颜的数量
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get the quantity of the effective overlapping beauty, needs to be called after the st_mobile_effect_render interface, because overlap information is updated after render.
/// @param[in] handle The initialized effects handle.
/// @param[out] p_info_num The number of changing beauty.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_overlapped_beauty_count(st_handle_t handle, int* p_info_num);

/// \~chinese
/// @brief 获取覆盖生效的美颜的信息, 需要在st_mobile_effect_render接口后调用，因为overlap信息是在render之后更新的
/// @param[in] handle 已初始化的特效句柄
/// @param[out] p_infos 美颜信息的起始地址
/// @param[in] info_num 起始地址可以容纳美颜信息的数量
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get the information of the effective overlapping beauty, needs to be called after the st_mobile_effect_render interface, because overlap information is updated after render.
/// @param[in] handle The initialized effects handle.
/// @param[out] p_infos The starting address of beauty information.
/// @param[in] info_num The number of beauty information that the starting address can accommodate.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_overlapped_beauty(st_handle_t handle, st_effect_beauty_info_t* p_infos, int info_num);

/// \~chinese
/// @brief 设置美颜的强度
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 美颜类型
/// @param[in] val 强度
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set the intensity of beauty.
/// @param[in] handle The initialized effects handle.
/// @param[in] param Beauty type.
/// @param[in] val Intensity.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_beauty_strength(st_handle_t handle, st_effect_beauty_type_t param, float val);

/// \~chinese
/// @brief 获取美颜的强度
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 美颜类型
/// @param[out] val 强度
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get the intensity of beauty.
/// @param[in] handle The initialized effects handle.
/// @param[in] param Beauty type.
/// @param[out] val Intensity.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_beauty_strength(st_handle_t handle, st_effect_beauty_type_t param, float* val);

/// \~chinese
/// @brief 磨皮模式
/// \~english
/// @brief smooth mode supported
typedef enum {
    EFFECT_SMOOTH_FACE_ONLY     = 0,      ///< \~chinese 只对人脸做磨皮处理 \~english smooth only on face
    EFFECT_SMOOTH_FULL_IMAGE    = 1,      ///< \~chinese 对全图做磨皮处理 \~english whole image smooth
    EFFECT_SMOOTH_FACE_DETAILED = 2,      ///< \~chinese 对脸部做精细化磨皮处理 \~english high quality smooth on face
    EFFECT_SMOOTH_FACE_REFINE   = 3,      ///< \~chinese 对脸部皮肤细化和改善质感的磨皮处理 \~english refining and improving the texture of facial skin
    EFFECT_SMOOTH_FACE_EVEN     = 4,      ///< \~chinest 针对脸部皮肤平整的匀肤磨皮处理 \~english for the treatment of evening out and smoothing facial skin texture
    EFFECT_SMOOTH_FACE_GAN      = 100,    ///< \~chinese gan模型对脸部皮肤进行磨皮处理，需要通过 st_mobile_effect_set_beauty 接口设置gan模型 \~english The gan model performs skin smoothing on facial skin and requires setting up the gan model through the st_mobile_effect_set_beauty interface.
} st_effect_smooth_mode;

/// \~chinese
/// @brief 美白模式，主要有 3 种不同的美白效果，同时兼容了老的美白效果
/// \~english
/// @brief whiten mode, four modes supported
typedef enum {
    EFFECT_WHITEN_1      = 0,      ///< \~chinese 美白1 \~english mode 1
    EFFECT_WHITEN_2      = 1,      ///< \~chinese 美白2 \~english mode 2
    EFFECT_WHITEN_3      = 2,      ///< \~chinese 美白3 \~english mode 3
    EFFECT_WHITEN_LEGACY = 3,      ///< \~chinese 老美白效果 \~english legacy mode.
} st_effect_whiten_mode;

/// \~chinese
/// @brief 背景虚化模式，主要有 2 种不同的背景虚化效果
/// \~english
/// @brief bokeh mode, two modes supported
typedef enum {
    EFFECT_BOKEH_1       = 0,      ///< \~chinese 背景虚化1 \~english bokeh mode 1
    EFFECT_BOKEH_2       = 1,      ///< \~chinese 背景虚化2 \~english bokeh mode 2
} st_effect_bokeh_mode;

/// \~chinese
/// @brief 设置美颜的模式, 目前仅对磨皮、美白、背景虚化有效，具体支持的模式参考前面枚举值定义
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 美颜类型, 目前支持磨皮、美白、背景虚化
/// @param[in] mode 模式，目前可选择 st_effect_smooth_mode, st_effect_whiten_mode, st_effect_bokeh_mode
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set the mode of beauty, currently only effective for skin smoothing, whitening, and background blurring, the supported valid modes are three values: [0, 1, 2] (background blurring supports two values: [0, 1]).
/// @param[in] handle The initialized effects handle.
/// @param[in] param Beauty type.
/// @param[in] mode Mode, the supported valid modes are three values: [0, 1, 2].
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_beauty_mode(st_handle_t handle, st_effect_beauty_type_t param, int mode);

/// \~chinese
/// @brief 获取美颜的模式, 目前仅对磨皮和美白有效
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 美颜类型
/// @param[out] mode 模式
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get the mode of beauty, currently only effective for skin smoothing and whitening.
/// @param[in] handle The initialized effects handle.
/// @param[in] param Beauty type.
/// @param[out] mode Mode.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_beauty_mode(st_handle_t handle, st_effect_beauty_type_t param, int* mode);

/// \~chinese
/// @brief Effect 美颜配置项，影响美颜功能，需要在OpenGL渲染线程中执行
/// \~english
/// @brief Beauty configuration options for effect, affecting the beauty function, and needs to be executed in the OpenGL rendering thread.
typedef enum {
    EFFECT_BEAUTY_PARAM_ENABLE_WHITEN_SKIN_MASK,         ///< \~chinese 是否为美白开启皮肤分割, 默认为不启用. 0 表示不启用， 非0表示启用 \~english Whether to enable skin segmentation for whitening. Disabled by default. 0 means disabled, non-zero means enabled.
} st_effect_beauty_param_t;

/// \~chinese
/// @brief 设置美颜相关配置项
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 配置项类型
/// @param[in] val 配置项参数值，具体范围参考配置项说明
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set beauty related configuration items.
/// @param[in] handle The initialized effects handle.
/// @param[in] param Configuration item type.
/// @param[in] val Configuration item parameter value, specific range refer to configuration item description.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_beauty_param(st_handle_t handle, st_effect_beauty_param_t param, float val);

/// \~chinese
/// @brief 获取美颜相关配置项
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 配置项类型
/// @param[out] val 配置项值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get beauty related configuration items.
/// @param[in] handle The initialized effects handle.
/// @param[in] param Configuration item type.
/// @param[out] val Configuration item value.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_beauty_param(st_handle_t handle, st_effect_beauty_param_t param, float* val);

/// \~chinese
/// @brief 设置试妆相关参数
/// @param[in] handle 已初始化的特效句柄
/// @param[in] tryon_type 试妆类型
/// @param[in] p_info 需要设置的试妆信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set tryon makeup related parameters.
/// @param[in] handle The initialized effects handle.
/// @param[in] tryon_type Tryon makeup type.
/// @param[in] p_info The tryon makeup information to be set.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_tryon_param(st_handle_t handle, st_effect_beauty_type_t tryon_type, const st_effect_tryon_info_t* p_info);

/// \~chinese
/// @brief 获取试妆相关参数
/// @param[in] handle 已初始化的特效句柄
/// @param[in] tryon_type 试妆类型
/// @param[out] p_info 需要返回的试妆信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get tryon makeup related parameters.
/// @param[in] handle The initialized effects handle.
/// @param[in] tryon_type Tryon makeup type.
/// @param[out] p_info The tryon makeup information to be returned.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_tryon_param(st_handle_t handle, st_effect_beauty_type_t tryon_type, st_effect_tryon_info_t* p_info);

/// \~chinese
/// @brief 加载美颜素材，可以通过将path参数置为nullptr，清空之前类型设置的对应素材（如美颜、美妆、滤镜素材）
///        目前对美颜支持设置美白、红润两种自定义的素材
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 美颜类型
/// @param[in] path 待添加的素材文件路径
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Load beauty material, can clear previous corresponding material (like beauty, makeup, filter materials) by setting the path parameter to nullptr.
///        Currently, it supports setting whitten and redden, two kinds of custom materials for beauty.
/// @param[in] handle The initialized effects handle.
/// @param[in] param Beauty type.
/// @param[in] path The file path of the material to be added.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_beauty(st_handle_t handle, st_effect_beauty_type_t param, const char* path);

/// \~chinese
/// @brief 加载美颜素材
/// @param[in] handle 已初始化的特效句柄
/// @param[in] param 美颜类型
/// @param[in] buffer 待添加的素材缓存信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Load beauty material.
/// @param[in] handle The initialized effects handle.
/// @param[in] param Beauty type.
/// @param[in] buffer The cache information of the material to be added.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_beauty_from_buffer(st_handle_t handle, st_effect_beauty_type_t param, const st_effect_buffer_t* buffer);

/// \~chinese
/// @brief 添加素材包
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_package_path 待添加的素材包文件路径
/// @param[out] p_package_id 素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Add a material package.
/// @param[in] handle The initialized effects handle.
/// @param[in] p_package_path The file path of the material package to be added.
/// @param[out] p_package_id Material package ID.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_add_package(st_handle_t handle, const char* p_package_path, int* p_package_id);

/// \~chinese
/// @brief 添加缓存中的素材包
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_package_buffer 待添加的素材包缓存信息
/// @param[out] p_package_id 素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Add the material package to the cache.
/// @param[in] handle The initialized effects handle.
/// @param[in] p_package_buffer The cache information of the material package to be added.
/// @param[out] p_package_id Material package ID.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_add_package_from_buffer(st_handle_t handle, const st_effect_buffer_t* p_package_buffer, int* p_package_id);

/// \~chinese
/// @brief 更换缓存中的素材包 (删除已有的素材包)
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_package_path 待更换的素材包文件路径
/// @param[out] package_id 素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Replace the material package in the cache (delete the existing material package).
/// @param[in] handle The initialized effects handle.
/// @param[in] p_package_path The file path of the material package to be used as the replacement.
/// @param[out] package_id Material package ID.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_change_package(st_handle_t handle, const char* p_package_path, int* p_package_id);

/// \~chinese
/// @brief 更换缓存中的素材包 (删除已有的素材包)
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_package_buffer 待更换的素材包缓存信息
/// @param[out] p_package_id 素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Replace the material package in the cache (delete the existing material package).
/// @param[in] handle The initialized effects handle.
/// @param[in] p_package_buffer The buffer information of the material package to be used as the replacement.
/// @param[out] p_package_id Material package ID.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_change_package_from_buffer(st_handle_t handle, const st_effect_buffer_t* p_package_buffer, int* p_package_id);

/// \~chinese
/// @brief 替换缓存中的素材包 (删除package id的素材包)
/// @param[in] handle 已初始化的特效句柄
/// @param[in] old_package_id 将被替换的素材包package id
/// @param[in] p_package_path 待更换的素材包文件路径
/// @param[out] p_new_package_id 新素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Replace the material package in the cache (delete the material package with the package id).
/// @param[in] handle The initialized effects handle.
/// @param[in] old_package_id The package ID of the material package to be replaced.
/// @param[in] p_package_path The file path of the material package to be used.
/// @param[out] p_new_package_id New material package ID.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_replace_package(st_handle_t handle, int old_package_id, const char* p_package_path, int* p_new_package_id);

/// \~chinese
/// @brief 替换缓存中的素材包 (删除package id的素材包)
/// @param[in] handle 已初始化的特效句柄
/// @param[in] old_package_id 将被替换的素材包package id
/// @param[in] p_package_buffer 待更换的素材包缓存信息
/// @param[out] p_new_package_id 素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Replace the material package in the cache (delete the material package with the package id).
/// @param[in] handle The initialized effects handle.
/// @param[in] old_package_id The package ID of the material package to be replaced.
/// @param[in] p_package_buffer The buffer information of the material package to be used.
/// @param[out] p_new_package_id New material package ID.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_replace_package_from_buffer(st_handle_t handle, int old_package_id, const st_effect_buffer_t* p_package_buffer, int *p_new_package_id);

/// \~chinese
/// @brief 删除指定素材包
/// @param[in] handle 已初始化的特效句柄
/// @param[in] package_id 待删除的素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Delete the specified material package.
/// @param[in] handle The initialized effects handle.
/// @param[in] package_id The material package ID to be deleted.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_remove_package(st_handle_t handle, int package_id);

/// \~chinese
/// @brief 清空所有素材包
/// @param[in] handle 已初始化的特效句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Clear all material packages.
/// @param[in] handle The initialized effects handle.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_clear_packages(st_handle_t handle);

/// \~chinese
/// @brief 重新播放制定素材包中的素材
/// @param[in] handle 已初始化的特效句柄
/// @param[in] package_id 素材包ID
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Replay the material in the specified material package.
/// @param[in] handle The initialized effects handle.
/// @param[in] package_id Material package ID.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_replay_package(st_handle_t handle, int package_id);

/// \~chinese
/// @brief 素材包的播放状态类型
/// \~english
/// @brief Material package play state type
typedef enum {
    EFFECT_PACKAGE_BEGIN,           ///< \~chinese 素材包开始(素材包中的任一素材开始播放认为素材包开始) \~english Material package starts (when any material in the package starts playing, it is considered that the material package starts)
    EFFECT_PACKAGE_END,             ///< \~chinese 素材包结束(素材包中开始播放后被切换或取消,或者已经开始的素材都完成播放循环认为是素材包结束播放) \~english Material package ends (when the material package has started playing and is switched or cancelled, or when all the started materials have completed the play cycle, it is considered that the material package has ended playing)
    EFFECT_PACKAGE_TERMINATED,      ///< \~chinese 素材包被终止(素材包中已经开始播放的素材都因为对应检测结果消失导致停止播放认为是素材包被终止) \~english Material package terminated (when all the materials that have started playing in the material package stop playing due to the disappearance of corresponding detection results, it is considered that the material package is terminated)
} st_effect_package_state_t;

/// \~chinese
/// @brief 素材包中Module（子特效）的播放状态类型
/// \~english
/// @brief Play state type of Module (sub-effect) in the material package
typedef enum {
    EFFECT_MODULE_LOADED,               ///< \~chinese 已加载 \~english Loaded
    EFFECT_MODULE_PAUSED_FIRST_FRAME,   ///< \~chinese 暂停到第一帧 \~english Paused to the first frame
    EFFECT_MODULE_PLAYING,              ///< \~chinese 正在播放 \~english Playing
    EFFECT_MODULE_PAUSED,               ///< \~chinese 暂停 \~english Paused
    EFFECT_MODULE_PAUSED_LAST_FRAME,    ///< \~chinese 暂停到最后一帧 \~english Paused to the last frame
    EFFECT_MODULE_INVISIBLE,            ///< \~chinese 不可见 \~english Invisible
    EFFECT_MODULE_RESUMED,              ///< \~chinese 唤醒, 下一帧开始执行 \~english Wake up, the next frame starts to execute
    EFFECT_MODULE_UNLOADED,             ///< \~chinese 已被销毁（卸载） \~english Destroyed (unloaded)
} st_effect_module_state_t;

/// \~chinese
/// @brief 素材包信息
/// \~english
/// @brief Material package information
typedef struct {
    int package_id;                     ///< \~chinese 素材包的ID \~english The ID of the material package
    char name[EFFECT_MAX_NAME_LEN];     ///< \~chinese 素材包的名字 \~english The name of the material package
    st_effect_package_state_t state;    ///< \~chinese 素材包当前的状态 \~english The current state of the material package
    int module_count;                   ///< \~chinese 素材包拥有的特效数量 \~english The number of effects owned by the material package
    int displayed_frames;               ///< \~chinese 当前播放的帧数 \~english The number of frames currently playing
    void* reserved;                     ///< \~chinese 额外的数据 \~english Additional data
} st_effect_package_info_t;

/// \~chinese
/// @brief 特效类型
/// \~english
/// @brief Effects types
typedef enum {
    EFFECT_MODULE_STICKER_2D = 0,       ///< \~chinese 2D贴纸模块 \~english 2D sticker module
    EFFECT_MODULE_SOUND = 1,            ///< \~chinese 音乐模块 \~english Music module
    EFFECT_MODULE_BEAUTIFY = 2,         ///< \~chinese 美颜模块 \~english Beautify module
    EFFECT_MODULE_FILTER = 3,           ///< \~chinese 滤镜模块 \~english Filter module
    EFFECT_MODULE_DEFORMAITON = 4,      ///< \~chinese 脸部变形模块 \~english Face deformation module
    EFFECT_MODULE_MAKEUP = 5,           ///< \~chinese 美妆模块 \~english Makeup module
    EFFECT_MODULE_BACKGROUND_EDGE = 6,  ///< \~chinese 背景描边模块 \~english Background edge module
    EFFECT_MODULE_STICKER_3D = 7,       ///< \~chinese 3D贴纸模块 \~english 3D sticker module
    EFFECT_MODULE_PARTICLE = 8,         ///< \~chinese 粒子模块 \~english Particle module
    EFFECT_MODULE_AVATAR = 9,           ///< \~chinese Avatar模块 \~english Avatar module
    EFFECT_MODULE_FACE_EXCHANGE = 10,   ///< \~chinese 多人换脸模块 \~english Multi-person face swap module
    EFFECT_MODULE_FACE_MATTING = 11,    ///< \~chinese 扣脸模块 \~english Face matting module
    EFFECT_MODULE_SKYBOX = 12,          ///< \~chinese 天空盒模块 \~english Skybox module
    EFFECT_MODULE_FACE_STRETCH = 14,    ///< \~chinese 人脸拖拽模块 \~english Face stretch module
    EFFECT_MODULE_DOUBLEGANGER = 15,    ///< \~chinese 影分身 \~english Doubleganger
    EFFECT_MODULE_MASK_FILL = 16,       ///< \~chinese 染发模块 \~english Hair dyeing module
    EFFECT_MODULE_HEAD_ANIMATION = 17,  ///< \~chinese 大头模块 \~english Big head module
    EFFECT_MODULE_3D_FACE_MATTE = 18,   ///< \~chinese 3D人脸抠图 \~english 3D face matte
    EFFECT_MODULE_TRYON = 19,           ///< \~chinese tryon试妆模块 \~english Tryon makeup module
    EFFECT_MODULE_GAN = 20,             ///< \~chinese Gan模块 \~english Gan module
    EFFECT_MODULE_SEGMENT = 21,         ///< \~chinese Segment模块 \~english Segment module
    EFFECT_MODULE_3D_MICRO_PLASTIC =22,     ///< \~chinese 3D微整形模块 \~english 3D micro plastic module
    EFFECT_MODULE_FACEMESH_EFFECTS = 23,    ///< \~chinese 3Dfacemesh特效模块 \~english 3D facemesh effects module
    EFFECT_MODULE_3D_CAT_STICKER = 24,      ///< \~chinese 3D猫脸贴纸 \~english 3D cat face sticker
    EFFECT_MODULE_3D_DOG_STICKER = 25,      ///< \~chinese 3D狗脸贴纸 \~english 3D dog face sticker
    EFFECT_MODULE_3D_SHOES_STICKER = 26,    ///< \~chinese 3D鞋子贴纸 \~english 3D shoe sticker
    EFFECT_MODULE_3D_WRIST = 27,            ///< \~chinese 3D手饰贴纸 \~english 3D hand ornament sticker
    EFFECT_MODULE_SPARKLE_FILTER = 28,      ///< \~chinese 闪烁滤镜 \~english Sparkle filter
    EFFECT_MODULE_GAN_IMAGE = 29,       ///< \~chinese Gan Image模块 \~english Gan Image module
    EFFECT_MODULE_3D_NAIL = 30,         ///< \~chinese 3D美甲贴纸 \~english 3D nail sticker
    EFFECT_MODULE_FACE_INSET = 31,      ///< \~chinese 抠五官模块 \~english Facial Feature Extraction module
    EFFECT_MODULE_3D_RING =32,          ///< \~chinese 3D戒指贴纸 \~english 3D ring sticker
} st_effect_module_type_t;

typedef struct st_file_buffer_t {
    char file_name[EFFECT_MAX_NAME_LEN];    ///< \~chinese 文件名称 \~english File name
    unsigned char* file_buffer;             ///< \~chinese 文件内容 \~english File content
    unsigned int file_len;                  ///< \~chinese 文件内容长度 \~english File content length
} st_file_buffer_t;


typedef struct st_gan_request_t {
    char category[EFFECT_MAX_NAME_LEN];     ///< \~chinese GAN类型名称（大类别）。目前支持AttrEdit,GanSkin,GanHair,GanAllStyle 和部署的服务器支持功能相关 \~english GAN type name (major category). Currently supports AttrEdit, GanSkin, GanHair, GanAllStyle and functions related to the deployed server
    char function[EFFECT_MAX_NAME_LEN];     ///< \~chinese 服务器上对应的function, 表示GAN类型名称(小类别) \~english The corresponding function on the server, representing the GAN type name (minor category)
    st_file_buffer_t* pzips;                ///< \~chinese 多个zip文件（变性别会有2个zip包，变小孩会传出1个zip包） \~english Multiple zip files (changing gender will have 2 zip packages, changing to a child will pass out 1 zip package)
    int zip_count;                          ///< \~chinese zip 文件个数 \~english Number of zip files
    st_image_t* in_image;                   ///< \~chinese 输入图像， sdk内部分配、管理内存 \~english Input image, sdk allocates and manages memory internally
    st_mobile_human_action_t* p_human;      ///< \~chinese 所使用图像的检测结果 \~english Detection results of the image used
} st_gan_request_t;

typedef struct st_gan_return_t {
    st_image_t* out_image;                  ///< \~chinese 输出图像，上层分配内存 \~english Output image, the upper layer allocates memory
} st_gan_return_t;

/// \~chinese @brief Module Info中的保留数据类型，根据特定case，保留数据的类型会不同
/// \~english @brief The reserved data types in Module Info, the type of reserved data varies according to specific cases
typedef enum
{
    EFFECT_RESERVED_UNKNOWN = 0,            ///< \~chinese 未知（无意义）保留数据 \~english Unknown (meaningless) reserved data
    EFFECT_RESERVED_SOUND_DATA = 1,         ///< \~chinese 声音数据的buffer，类型是st_effect_buffer_t \~english Buffer of sound data, the type is st_effect_buffer_t
    EFFECT_RESERVED_LOOP_NUM = 2,           ///< \~chinese module的loop次数，类型是int \~english The loop count of the module, the type is int
    EFFECT_RESERVED_IMAGE = 3,              ///< \~chinese 图像数据的buffer，类型是st_image_t \~english Buffer of image data, the type is st_image_t
    EFFECT_RESERVED_GAN_SERVER = 4,         ///< \~chinese ganserver request请求所需数据，类型是st_gan_in \~english Data required for ganserver request, the type is st_gan_in
    EFFECT_RESERVED_SEGMENT_BASECOLOR = 5,  ///< \~chinese 返回绿幕分割基色, 类型是uint32_t \~english Return the base color of green screen segmentation, the type is uint32_t
    EFFECT_RESERVED_TEXTURE = 6,            ///< \~chinese 纹理信息，类型是st_mobile_texture_t（仅支持RGBA格式的纹理, 如果指定该选项，st_mobile_effect_set_module_info接口需要在与render相同的GL context中调用） \~english Texture information, the type is st_mobile_texture_t (only supports RGBA format textures, if this option is specified, st_mobile_effect_set_module_info interface needs to be called in the same GL context with render)
} st_effect_reserved_t;


/// \~chinese @brief Module Info结构体，可以通过st_mobile_effect_get_modules_in_package，st_mobile_effect_set_module_state_change_callback等API获取
///                  通过st_mobile_effect_set_module_info API设置module数据
/// \~english @brief Module Info structure, can be obtained through st_mobile_effect_get_modules_in_package, st_mobile_effect_set_module_state_change_callback and other APIs
///                  Set module data through st_mobile_effect_set_module_info API
typedef struct {
    st_effect_module_type_t type;       ///< \~chinese 贴纸的类型 \~english The type of sticker
    int module_id;                      ///< \~chinese 贴纸的ID \~english The ID of the sticker
    int package_id;                     ///< \~chinese 贴纸所属素材包的ID \~english The ID of the material package to which the sticker belongs
    char name[EFFECT_MAX_NAME_LEN];     ///< \~chinese 贴纸的名字 \~english The name of the sticker
    float strength;                     ///< \~chinese 贴纸的强度 \~english The strength of the sticker
    int instance_id;                    ///< \~chinese 贴纸对应的position id, 即st_mobile_human_action_t结果中不同类型结果中的id \~english The position id corresponding to the sticker, that is, the id in the different types of results in st_mobile_human_action_t
    st_effect_module_state_t state;     ///< \~chinese 贴纸的播放状态 \~english The playing state of the sticker
    int current_frame;                  ///< \~chinese 当前播放的帧数 \~english The number of frames currently playing
    uint64_t position_type;             ///< \~chinese 贴纸对应的position种类, 见st_mobile_human_action_t中的动作类型 \~english The type of position corresponding to the sticker, see the action type in st_mobile_human_action_t
    st_effect_reserved_t rsv_type;      ///< \~chinese 额外数据（reserved）的类型，在特定case下需要强转为特定类型，参考st_effect_reserved_type定义 \~english The type of additional data (reserved), in specific cases, it needs to be cast to a specific type, refer to the definition of st_effect_reserved_type
    void* reserved;                     ///< \~chinese 额外的数据，在声音EFFECT_MODULE_LOADED回调中指向st_effect_buffer_t类型的声音文件buffer地址(该地址只在回调函数调用期间有效)，在声音的EFFECT_MODULE_PLAYING回调中对应的是声音循环次数（强转为int） \~english Additional data, points to the address of the sound file buffer of type st_effect_buffer_t in the sound EFFECT_MODULE_LOADED callback (this address is only valid during the callback function call), and corresponds to the sound loop count in the sound EFFECT_MODULE_PLAYING callback (cast to int)
} st_effect_module_info_t;

/// \~chinese
/// @brief 获取素材信息
/// @param[in] handle 已初始化的特效句柄
/// @param[in] package_id 素材包ID
/// @param[out] p_package_info 素材信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get material information.
/// @param[in] handle The initialized effects handle.
/// @param[in] package_id Material package ID.
/// @param[out] p_package_info Material information.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_package_info(st_handle_t handle, int package_id, st_effect_package_info_t* p_package_info);

/// \~chinese
/// @brief 获取素材的贴纸信息
/// @param[in] handle 已初始化的特效句柄
/// @param[in] package_id 素材包ID
/// @param[in] module_num 贴纸信息地址能容纳的贴纸的数量
/// @param[out] p_modules 贴纸信息的起始地址
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get sticker information of the material.
/// @param[in] handle The initialized effects handle.
/// @param[in] package_id Material package ID.
/// @param[in] module_num The number of stickers that the sticker information address can hold.
/// @param[out] p_modules The starting address of the sticker information.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_modules_in_package(st_handle_t handle, int package_id, st_effect_module_info_t* p_modules, int module_num);

/// \~chinese
/// @brief 获取贴纸信息
/// @param[in] handle 已初始化的特效句柄
/// @param[in] module_id 贴纸ID
/// @param[out] p_module_info 贴纸信息的起始地址
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Get sticker information.
/// @param[in] handle The initialized effects handle.
/// @param[in] module_id Sticker ID.
/// @param[out] p_module_info The starting address of the sticker information.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_get_module_info(st_handle_t handle, int module_id, st_effect_module_info_t* p_module_info);

/// \~chinese
/// @brief 设置贴纸信息
/// @param[in] handle 已初始化的特效句柄
/// @param[in] module_id 贴纸ID
/// @param[in] p_module_info 贴纸信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set sticker information.
/// @param[in] handle The initialized effects handle.
/// @param[in] module_id Sticker ID.
/// @param[in] p_module_info Sticker information.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_module_info(st_handle_t handle, const st_effect_module_info_t* p_module_info);

/// \~chinese
/// @brief 设置贴纸素材包内部美颜组合的强度，强度范围[0.0, 1.0]
/// @param[in] handle 已初始化的特效句柄
/// @param[in] group 美颜组合类型
/// @param[in] strength 强度值
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set the intensity of the beauty combination inside the sticker material package, with the intensity range [0.0, 1.0].
/// @param[in] handle The initialized effects handle.
/// @param[in] group The type of beauty combination, currently only supports setting the intensity of makeup and filter combinations.
/// @param[in] strength Intensity value.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_package_beauty_group_strength(st_handle_t handle, int pkg_id, st_effect_beauty_group_t group, float strength);

/// \~chinese
/// @brief 贴纸播放状态发生改变引起的回调函数
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_module_info 贴纸信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Callback function triggered by changes in sticker playback status.
/// @param[in] handle The initialized effects handle.
/// @param[in] p_module_info Sticker information.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
typedef st_result_t(*st_effect_module_state_change_callback)(st_handle_t handle, const st_effect_module_info_t* p_module_info);

/// \~chinese
/// @brief 设置贴纸播放状态改变的回调函数指针
/// @param[in] handle 已初始化的特效句柄
/// @param[in] callback 回调函数指针
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set the callback function pointer for changes in sticker playback status.
/// @param[in] handle The initialized effects handle.
/// @param[in] callback Callback function pointer.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_module_state_change_callback(st_handle_t handle, st_effect_module_state_change_callback callback);

/// \~chinese
/// @brief 素材包播放状态发生改变的引起的回调函数
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_package_info 素材包信息
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Callback function triggered by changes in material package playback status.
/// @param[in] handle The initialized effects handle.
/// @param[in] p_package_info Material package information.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
typedef st_result_t(*st_effect_package_state_change_callback)(st_handle_t handle, const st_effect_package_info_t* p_package_info);

/// \~chinese
/// @brief 设置素材包播放状态改变的回调函数指针
/// @param[in] handle 已初始化的特效句柄
/// @param[in] callback 回调函数指针
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Set the callback function pointer for changes in material package playback status.
/// @param[in] handle The initialized effects handle.
/// @param[in] callback Callback function pointer.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_set_packaged_state_change_callback(st_handle_t handle, st_effect_package_state_change_callback callback);

/// \~chinese
/// @brief 释放内部缓存的资源，目前主要是GL相关的渲染资源，需要在GL context中调用
/// @param[in] handle 已初始化的特效句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Release the resources cached internally, mainly GL-related rendering resources, and needs to be invoked in GL context.
/// @param[in] handle The initialized effects handle.
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_release_cached_resource(st_handle_t handle);

/// \~chinese
/// @brief 重置render接口输出buffer时，PC平台内部的双缓冲。一般在PC平台上层有一段时间没有用render接口时，防止闪一帧前面画面时调用。
/// @param[in] handle 已初始化的特效句柄
/// @return 成功返回ST_OK, 失败返回其他错误码, 错误码定义在st_mobile_common.h中, 如ST_E_FAIL等
/// \~english
/// @brief Reset the internal double buffering of the PC platform when the render interface output buffer. Generally called when the upper layer of the PC platform has not used the render interface for a period of time to prevent flashing a frame of the previous screen.
/// @param[in] handle An initialized effect handle
/// @return Returns ST_OK if successful, otherwise returns other error codes. Error code definitions can be found in st_mobile_common.h, such as ST_E_FAIL etc.
ST_SDK_API st_result_t
st_mobile_effect_reset_output_buffer_cache(st_handle_t handle);


/// \~chinese
/// @brief 在调用st_mobile_effect_set_beauty函数加载了3D微整形素材包之后调用。获取到素材包中所有的blendshape的数量
/// @param[in] handle 已初始化的特效句柄
/// @param[out] parts_count 输出的数组长度
/// \~english
/// @brief This is called after st_mobile_effect_set_beauty function is used to load a 3D micro shaping material package. It gets the quantity of all blendshapes in the material package.
/// @param[in] handle An initialized effect handle
/// @param[out] parts_count Output array length.
ST_SDK_API st_result_t
st_moobile_effect_get_3d_beauty_parts_count(st_handle_t handle, int* parts_count);

/// \~chinese
/// @brief 在调用st_mobile_effect_set_beauty函数加载了3D微整形素材包之后调用。获取到素材包中所有的blendshape名称、index和当前强度[0, 1]
/// @param[in] handle 已初始化的特效句柄
/// @param[out] parts 输出的blend shape信息数组。由外部分配内存。
/// @param[in] parts_count 输入的数组长度,应小于等于获取的数量
/// \~english
/// @brief This is called after st_mobile_effect_set_beauty function is used to load a 3D micro shaping material package. It gets all the blendshape names, index, and current intensity [0, 1] in the material package.
/// @param[in] handle An initialized effect handle
/// @param[out] parts Output blend shape information array. Memory is allocated externally.
/// @param[in] parts_count Input array length, should be less than or equal to the obtained quantity.
ST_SDK_API st_result_t
st_mobile_effect_get_3d_beauty_parts(st_handle_t handle, st_effect_3D_beauty_part_info_t* parts, int parts_count);

/// \~chinese
/// @brief 用于输入human action的face mesh list信息。
/// @param[in] handle 已初始化的特效句柄
/// @param[in] parts 从human action中获取的face_mesh_list信息指针
/// \~english
/// @brief Input the face mesh list information for human action.
/// @param[in] handle The initialized effects handle.
/// @param[in] parts Pointer to the face_mesh_list information obtained from human action.
ST_SDK_API st_result_t
st_mobile_effect_set_face_mesh_list(st_handle_t handle, st_mobile_face_mesh_list_t* face_mesh_list);

/// \~chinese
/// @brief 在调用st_mobile_effect_set_beauty函数加载了3D微整形素材包之后调用。在获取blendshape数组之后，可以依据起信息修改权重[0, 1]，设置给渲染引擎产生效果。
/// @param[in] handle 已初始化的特效句柄
/// @param[in] parts 输入的blend shape信息数组。
/// @param[in] partSize 输入的数组长度
/// \~english
/// @brief This is called after st_mobile_effect_set_beauty function is used to load a 3D micro shaping material package. After getting the blendshape array, you can modify the weight [0, 1] based on its information and set it to the rendering engine to produce an effect.
/// @param[in] handle An initialized effect handle
/// @param[in] parts Input blend shape information array
/// @param[in] partSize Length of the input array
ST_SDK_API st_result_t
st_mobile_effect_set_3d_beauty_parts_strength(st_handle_t handle, st_effect_3D_beauty_part_info_t* parts, int partSize);


/// \~chinese
/// @brief 安全区类型，目前支持矩形、椭圆形两种类型。
/// \~english
/// @brief Safe area type, currently supporting rectangle and ellipse types.
typedef enum region_type
{
    EFFECT_REGION_RECT,
    EFFECT_REGION_ELLIPSE,
} st_effect_region_t;

/// \~chinese
/// @brief 安全区数据结构，首先指定类型，然后根据类型指定具体的区域参数。
/// \~english
/// @brief Safe area data structure, first specify the type, then specify the specific area
typedef struct safe_region
{
    st_effect_region_t type;    // \~chinese 支持矩形、椭圆两种类型 \~english Supports rectangle and ellipse types
    st_pointi_t param1;         // \~chinese 根据类型不同，语义不同，矩形指左上角坐标，椭圆指中心点坐标 \~english Different semantics based on type, rectangle refers to the top left corner coordinate, ellipse refers to the center point coordinate
    st_pointi_t param2;         // \~chinese 根据类型不同，语义不同，矩形指右下角坐标，椭圆指[x: 横轴半径，y: 竖轴半径] \~english Different semantics based on type, rectangle refers to the bottom right corner coordinate, ellipse refers to [x: horizontal axis radius, y: vertical axis radius]
} st_effect_safe_region_t;

/// \~chinese
/// @brief 设置绿幕分割安全区，安全区中的像素将不进行绿幕分割，暴露原图像素
/// @param[in] handle 已初始化的特效句柄
/// @param[in] p_regions 安全区数组，传入空指针意味着清除所有安全区
/// @param[in] num 安全区数组长度
/// \~english
/// @brief Set the green screen segmentation safety zone. The pixels in the safety zone will not undergo green screen segmentation and expose the original pixel.
/// @param[in] handle An initialized effect handle
/// @param[in] p_regions Array of safety zones, passing a null pointer means clearing all safety zones.
/// @param[in] num Length of the safety zone array
ST_SDK_API st_result_t
st_mobile_effect_set_greenmask_safe_regions(st_handle_t handle, const st_effect_safe_region_t *p_regions, int num);


#endif // _ST_MOBILE_EFFECT_H_
