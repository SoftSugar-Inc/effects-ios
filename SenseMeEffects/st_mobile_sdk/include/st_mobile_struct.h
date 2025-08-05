#ifndef ST_MOBILE_STRUCT_H
#define ST_MOBILE_STRUCT_H

#include "st_mobile_common.h"
#include <stdint.h>

typedef float st_rmat_t[3][3];

/// \~chinese @brief human 3d pose 信息
/// \~english @brief Human 3D pose information
typedef struct {
	uint64_t position;                             ///< \~chinese 检测的config,ST_MOBILE_WRIST_DETECT或ST_MOBILE_DETECT_FOOT，可用于区分获取的3dPose \~english Detection configuration used to differentiate the obtained 3D pose information, ST_MOBILE_WRIST_DETECT for wrist, ST_MOBILE_DETECT_FOOT for foot
	int id;                                        ///< \~chinese 检测id \~english Detection ID
	st_mobile_transform_t pose;                    ///< \~chinese 输出的pose信息 \~english Pose information
	bool isvalid;                                  ///< \~chinese 输出pose信息是否有效 \~english Indicates whether the output pose information is valid
}st_human_pose_t;


/// \~chinese @brief 静态手势类型
/// \~english @brief Hand action gesture types
#define ST_HAND_ACTION_TYPE_OK				0x00000200              ///< \~chinese OK手势 \~english OK hand gesture
#define ST_HAND_ACTION_TYPE_SCISSOR			0x00000400              ///< \~chinese 剪刀手 \~english Scissor hand gesture
#define ST_HAND_ACTION_TYPE_GOOD			0x00000800              ///< \~chinese 大拇哥 \~english Thumbs up hand gesture
#define ST_HAND_ACTION_TYPE_PALM			0x00001000              ///< \~chinese 手掌 \~english Palm gesture
#define ST_HAND_ACTION_TYPE_PISTOL			0x00002000              ///< \~chinese 手枪手势 \~english Pistol hand gesture
#define ST_HAND_ACTION_TYPE_LOVE			0x00004000              ///< \~chinese 爱心手势 \~english Heart hand gesture
#define ST_HAND_ACTION_TYPE_HOLDUP			0x00008000              ///< \~chinese 托手手势 \~english Hold up hand gesture
#define ST_HAND_ACTION_TYPE_FIST			0x00200000              ///< \~chinese 拳头手势 \~english Fist hand Agesture
#define ST_HAND_ACTION_TYPE_CONGRATULATE	0x00020000              ///< \~chinese 恭贺A（抱拳）\~english Fist-palm A salute
#define ST_HAND_ACTION_TYPE_FINGER_HEART	0x00040000              ///< \~chinese 单手比爱心 \~english Finger heart gesture
#define ST_HAND_ACTION_TYPE_FINGER_INDEX	0x00100000              ///< \~chinese 食指指尖 \~english Finger point gesture
#define ST_HAND_ACTION_TYPE_666				0x00400000              ///< \~chinese 666 \~english 666 hand gesture
#define ST_HAND_ACTION_TYPE_BLESS			0x00800000              ///< \~chinese 双手合十 \~english Bless hand gesture
#define ST_HAND_ACTION_TYPE_ILOVEYOU		0x010000000000          ///< \~chinese 手势ILoveYou \~english I love you hand gesture
#define ST_HAND_ACTION_TYPE_THREE			0x10000000000000        ///< \~chinese 三根手指 \~english Three fingers
#define ST_HAND_ACTION_TYPE_FOUR			0x20000000000000        ///< \~chinese 四根手指 \~english Four fingers
#define ST_HAND_ACTION_TYPE_THUMBS_DOWN		0x01                    ///< \~chinese 差评 \~english Thumbs down gesture
#define ST_HAND_ACTION_TYPE_LOVEB			0x02                    ///< \~chinese 比心B \~english Heart hand B gesture
#define ST_HAND_ACTION_TYPE_LOVEC			0x04                    ///< \~chinese 比心C \~english Heart hand C gesture
#define ST_HAND_ACTION_TYPE_SEVEN			0x08                    ///< \~chinese 七（卷心菜） \~english Seven gesture
#define ST_HAND_ACTION_TYPE_FACE_PALMING	0x10                    ///< \~chinese 捧脸 \~english Face plaming gesture
#define ST_HAND_ACTION_TYPE_PRAY			0x20                    ///< \~chinese 祈祷 \~english Pray gesture
#define ST_HAND_ACTION_TYPE_NINE			0x40                    ///< \~chinese 九 \~english Nine gesture
#define ST_HAND_ACTION_TYPE_CONGRATULATE_B  0x80                    ///< \~chinese 恭贺B（抱拳）\~english Fist-palm B salute
#define ST_HAND_ACTION_TYPE_SSH				0x400000000000          ///< \~chinese 手势嘘,依赖于手势检测和106点检测 \~english Shush hand. This detection relies on gesture detection and face 106-point detection
#define ST_HAND_ACTION_TYPE_OTHER			0                       ///< \~chinese 其他手势 \~english Other gesture

/// \~chinese @brief 动态手势类型
/// \~english @brief Dynamic hand gesture types
typedef enum st_hand_dynamic_gesture_type_t {
	ST_DYNAMIC_GESTURE_TYPE_INVALID = -1,                      ///< \~chinese 无效的动态手势 \~english Invalid dynamic hand gesture
	ST_DYNAMIC_GESTURE_TYPE_HOLD_ON,                           ///< \~chinese 静止 \~english Hold on gesture
	ST_DYNAMIC_GESTURE_TYPE_FOREFINGER_CLICK,                  ///< \~chinese 食指点击 \~english Forefinger click gesture
	ST_DYNAMIC_GESTURE_TYPE_FOREFINGER_ROTATION_CLOCKWISE,     ///< \~chinese 食指顺时针旋转 \~english Forefinger clockwise rotation gesture
	ST_DYNAMIC_GESTURE_TYPE_FOREFINGER_ROTATION_ANTICLOCKWISE, ///< \~chinese 食指逆时针旋转 \~english Forefinger anticlockwise rotation gesture
	ST_DYNAMIC_GESTURE_TYPE_PALM_FAN,                          ///< \~chinese 手掌扇风（废弃） \~english Palm fan gesture (deprecated)
	ST_DYNAMIC_GESTURE_TYPE_PALM_MOVING_LEFT_AND_RIGHT,        ///< \~chinese 手掌左右平移 \~english Palm moving left and right gesture
	ST_DYNAMIC_GESTURE_TYPE_PALM_MOVING_UP_AND_DOWN,           ///< \~chinese 手掌上下平移 \~english Palm moving up and down gesture
	ST_DYNAMIC_GESTURE_TYPE_MAX_NUM                            ///< \~chinese 目前支持的动态手势个数 \~english Maximum number of supported dynamic hand gestures
} st_hand_dynamic_gesture_type_t;
/// \~chinese @brief 动态手势结果
/// \~english @brief Dynamic hand gesture result
typedef struct st_hand_dynamic_gesture_t {
	int has_dynamic_gesture;                        ///< \~chinese 是否有动态手势：0表示没有，1表示有 \~english Indicates whether there is a dynamic hand gesture: 0 for no, 1 for yes
	st_hand_dynamic_gesture_type_t dynamic_gesture; ///< \~chinese 动态手势类别 \~english Dynamic hand gesture type
	float score;                                    ///< \~chinese 动态手势得分 \~english Dynamic hand gesture score
} st_hand_dynamic_gesture_t;
/// \~chinese @brief 手势检测结果
/// \~english @brief Hand detection result
typedef struct st_mobile_hand_t {
	int id;                                         ///< \~chinese 手的id \~english Hand ID
	st_rect_t rect;                                 ///< \~chinese 手部矩形框 \~english Hand bounding rectangle
	int left_right;                                 ///< 0 unknown ,1 left, 2 right \~chinese 左手/右手 只在3d骨骼点开启后有结果 \~english Indicates left hand or right hand, only available when ST_MOBILE_DETECT_HAND_SKELETON_KEYPOINTS_3D option is enabled
	st_pointf_t *p_key_points;                      ///< \~chinese 手部关键点 \~english Hand key points
	int key_points_count;                           ///< \~chinese 手部关键点个数 \~english Number of hand key points
	uint64_t hand_action;                           ///< \~chinese 静态手势 \~english Hand action
	float score;                                    ///< \~chinese 静态手势置信度 \~english Confidence score of hand action
	st_pointf_t *p_skeleton_keypoints;              ///< \~chinese 手部骨骼点 \~english Hand skeleton points
	int skeleton_keypoints_count;                   ///< \~chinese 手部骨骼点个数 一般是0/20 \~english Number of hand skeleton points, usually 0/20
	st_point3f_t *p_skeleton_3d_keypoints;          ///< \~chinese 手部3d骨骼点 \~english 3D hand skeleton points
	int skeleton_3d_keypoints_count;                ///< \~chinese 手部3d骨骼点的个数，一般是0/21 \~english Number of 3D hand skeleton points, usually 0/21
	st_hand_dynamic_gesture_t hand_dynamic_gesture; ///< \~chinese 动态手势 \~english Dynamic hand gesture
	st_pointf_t *p_dynamic_gesture_keypoints;       ///< \~chinese 动态手势关键点 \~english Dynamic hand gesture key points
	int dynamic_gesture_keypoints_count;            ///< \~chinese 动态手势关键点的个数 \~english Number of dynamic hand gesture key points
} st_mobile_hand_t, *p_st_mobile_hand_t;

/// \~chinese @brief 每个指甲检测结果
/// \~english @brief Nail detection result
typedef struct st_mobile_nail_t {
	int id;											///< \~chinese 指甲id \~english Nail ID
	int label;										///< \~chinese 指甲label，取值0-4(大拇指，食指，中指，无名指和小拇指) \~english Nail label, ranging from 0 to 4 (thumb, index finger, middle finger, ring finger, little finger)
	float score;									///< \~chinese 置信度 \~english Confidence score
	st_rect_t rect;									///< \~chinese 包络矩形框 \~english Bounding rectangle
	st_pointf_t *p_key_points;						///< \~chinese 关键点数组 \~english Nail key points
	int points_count; 								///< \~chinese 关键点数目,一般是16 \~english Number of key points, usually 16
	st_3dpose_t* p_pose3d;						    ///< \~chinese pose指甲3d位置信息,方向与人正立方向一致，暂不支持resize mirror rotate后处理 \~english 3D nail position information, aligned with the upright direction of the person, not supported for resizing, mirroring, or rotating currently
}st_mobile_nail_t, *p_st_mobile_nail_t;

typedef struct st_mobile_wrist_t {
	int id;											///< \~chinese 手腕id \~english Wrist ID
	float score;									///< \~chinese 置信度 \~english Confidence score
	st_rect_t rect;									///< \~chinese 包络矩形框 \~english Bounding rectangle
	st_pointf_t *p_key_points;						///< \~chinese 关键点数组 \~english Wrist key points
	int points_count; 								///< \~chinese 关键点数目,一般是4 \~english Number of key points, usually 4
	st_left_or_right_t label;						///< \~chinese 左右手分类 \~english Left or right hand
	st_3dpose_t* p_pose3d;						    ///< \~chinese pose手腕3d位置信息,方向与人正立方向一致，暂不支持resize mirror rotate后处理 \~english 3D wrist position information, aligned with the upright direction of the person, not supported for resizing, mirroring, or rotating currently
}st_mobile_wrist_t, *p_st_mobile_wrist_t;

/// \~chinese @brief 手指类型
/// \~english @brief Finger type
typedef enum {
	ST_FINGER_THUMB = 0,          ///< \~chinese 大拇指 \~english Thumb
	ST_FINGER_INDEX = 1,          ///< \~chinese 食指 \~english Index finger
	ST_FINGER_MIDDLE = 2,         ///< \~chinese 中指 \~english Middle finger
	ST_FINGER_RING = 3,           ///< \~chinese 无名指 \~english Ring finger
	ST_FINGER_PINKY = 4,          ///< \~chinese 小拇指 \~english Little finger
	ST_FINGER_OTHER = 5,          ///< \~chinese 其他手指 \~english Other finger
}st_finger_type_t;

/// \~chinese @brief 单根手指检测结果
/// \~english @brief Single finger detection result
typedef struct {
	int id;                         ///< \~chinese 手部id(整个手的id) \~english Finger ID
	st_finger_type_t finger_type;   ///< \~chinese 手指类型 0-大拇指 1-食指 2-中指 3-无名指 4-小拇指，目前只检测无名指 \~english Finger type: 0-Thumb, 1-Index finger, 2-Middle finger, 3-Ring finger, 4-Little finger
	float score;                    ///< \~chinese 置信度 \~english Confidence score
	st_pointf_t* p_key_points;      ///< \~chinese 手指关键点数组 \~english Array of finger keypoints
	int points_count;               ///< \~chinese 关键点数量 \~english Number of keypoints
	st_left_or_right_t label;       ///< \~chinese 左右手分类 \~english Left or right hand classification
	st_3dpose_t* p_pose3d;          ///< \~chinese pose信息 \~english Pose information in 3D
	float rotate_rect[5];           ///< \~chinese 旋转框 0-angle 1-x 2-y 3-width 4-height \~english Rotation rectangle: 0-angle, 1-x, 2-y, 3-width, 4-height
}st_mobile_finger_t;


/// \~chinese @brief 肢体检测结果
/// \~english @brief Body detection result
typedef struct st_mobile_body_t {
	int id;                         ///< \~chinese 肢体 id \~english Body ID
	st_pointf_t *p_key_points;      ///< \~chinese 肢体关键点 \~english Body key points
	float * p_key_points_score;     ///< \~chinese 肢体关键点的置信度[0,1] 值越大，置信度越高.建议用户使用0.15作为置信度阈值. \~english Confidence scores of body key points [0,1]. It is recommended to use 0.15 as the confidence threshold
	int key_points_count;           ///< \~chinese 肢体关键点个数 目前为0/4/14 \~english Number of body key points, currently 0/4/14
	st_pointf_t *p_contour_points;  ///< \~chinese 肢体轮廓点 \~english Body contour points
	float * p_contour_points_score; ///< \~chinese 肢体轮廓点的置信度[0,1] 值越大，置信度越高.建议用户使用0.15作为置信度阈值. \~english Confidence scores of body contour points [0,1]. It is recommended to use 0.15 as the confidence threshold
	int contour_points_count;       ///< \~chinese 肢体轮廓点个数 目前为0/63 \~english Number of body contour points, currently 0/63
	st_point3f_t *p_key_points_3d;	///< \~chinese 肢体3d点 \~english Body 3D points
	float * p_key_points_3d_score;  ///< \~chinese 肢体3d点置信度 \~english Confidence scores of body 3D points
	int key_points_3d_count;		///< \~chinese 肢体3d点个数(0或17） \~english Number of body 3D points, currently 0/17
	int label;						///< \~chinese 肢体label 0表示正常 1表示双手抱胸 2表示双手抱头 \~english Body label. 0 for normal, 1 for crossed arms, 2 for hands on head
	int hand_valid[2];              ///< \~chinese 半身肢体中左右手信息是否有效，仅用于半身avatar模型，用于调试看中间结果 \~english Indicates whether left and right hand information is valid in half-body body model, used for debugging intermediate results
} st_mobile_body_t, *p_st_mobile_body_t;

/// \~chinese @brief 分割检测结果
/// \~english @brief Segmentation detection result
typedef struct st_mobile_segment_t {
	st_image_t *p_segment;      ///< \~chinese 前后背景分割结果图片信息,前景为0,背景为255,边缘部分有模糊(0-255之间),输出图像大小可以调节，mask大小不能超过图像的大小． \~english Segmentation result image information. Foreground is represented by 0, background is represented by 255, and the edge has blur (between 0 and 255). The output image size can be adjusted, but the mask size cannot exceed the image size
	float score;		        ///< \~chinese 前后背景分割置信度 \~english Segmentation confidence score
	///min_threshold 和max_threshold值都是0.0或分别为0.0和1.0 上层则不需要卡阈值. \~english If min_threshold and max_threshold are both 0.0 or either 0.0 and 1.0. post-processing is not required externally.
	float min_threshold;	    ///< \~chinese 前后背景最小阈值，与模型相关，取值范围时0.0f-1.0f，当min_threshold和max_threshold同时大于0时，需要外部做后处理，当原始图像尺寸大于320，mask大小是320; 当原始图像尺寸小于320，mask输出大小是原图大小 \~english Minimum threshold for segmentation, configured by the model, ranges from 0.0f to 1.0f. If both min_threshold and max_threshold are greater than 0, post-processing is required externally. When the original image size is larger than 320, the mask size is 320; when the original image size is smaller than 320, the mask output size is the same as the original image size
	float max_threshold;	    ///< \~chinese 前后背景最大阈值，同上 \~english Maximum threshold for segmentation, same as above
	st_pointf_t offset;         ///< \~chinese 分割结果位于原图的左上角坐标，一般是（0,0），只有嘴唇分割结果不同 \~english The coordinates of the segmentation result in the upper left corner of the original image, usually (0, 0), except for lip segmentation results
	st_pointf_t scale;          ///< \~chinese 分割结果的缩放比例。p_segment->width(height)*scale为和原图对应的分割结果像素大小。 \~english The scaling ratio of the segmentation result. p_segment->width(height) * scale represents the pixel size of the corresponding segmentation result with the original image
	st_rotate_type rotate;      ///< \~chinese 分割结果朝向, 除深度估计和腿部分割结果除外, 其他分割结果默认朝上 \~english Orientation of the segmentation result, defaults to upward except for depth estimation and leg segmentation results
	int face_id;                ///< \~chinese 如果-1表示是整幅图的结果; 如果>=0, 与st_mobile_face_t 中的id对应. 目前嘴部分割和头部特殊模型分割支持id \~english If -1, it indicates the result of the entire image; if >=0, it corresponds to the id in st_mobile_face_t. Currently, the mouth segmentation and special head model segmentation support id
	unsigned char* p_extra_info_buffer;  ///< \~chinese 额外输出的中间信息， 仅供sdk中的渲染接口使用， 目前仅天空和绿幕分割会输出 \~english Additional intermediate information output, only used for rendering interfaces in the SDK. Currently, only sky and green screen segmentation will output
	int extra_info_length;      ///< \~chinese 额外输出中间信息buffer长度 \~english Length of the buffer for additional intermediate information output
} st_mobile_segment_t, *p_st_mobile_segment_t;

/// \~chinese @brief 人头检测结果信息
/// \~english @brief Head detection result
typedef struct st_mobile_head_result_t {
	st_rect_t rect;             ///< \~chinese 目标框，用于表示此目标在当前帧上的位置 \~english Bounding rectangle representing the position of the target in the current frame
	int id;                     ///< \~chinese 目标ID, 用于表示在目标跟踪中的相同目标在不同帧多次出现 \~english Target ID, indicate the same target appearing multiple times in different frames in target tracking
	float score;                ///< \~chinese 目标置信度，用于表示此目标在当前帧中的置信度 (0 - 10) \~english Target confidence score, indicate the confidence of the target in the current frame (0 - 10)
	float angle;                ///< \~chinese 目标roll角，用于表示此目标在当前帧中的旋转信息，原图中目标逆时针旋转angle度后，目标会是正方向 (-180 - 180) \~english Roll angle of the target, used to represent the rotation information of the target in the current frame. After rotating the target counterclockwise by the angle in the original image, the target will be in the upright orientation (-180 - 180)
} st_mobile_head_result_t, *p_st_mobile_head_result_t;

/// \~chinese @brief 人头检测结果
/// \~english @brief Head detection result
typedef struct st_mobile_head_t {
	st_mobile_head_result_t * p_head_result; ///< \~chinese 人头检测结果信息 \~english Head detection result information
	st_mobile_face_mesh_t * p_head_mesh;     ///< \~chinese head mesh信息，包括head mesh关键点及个数 \~english Head mesh information, including head mesh keypoints and count
} st_mobile_head_t, *p_st_mobile_head_t;

typedef struct st_mobile_bodymesh_t {
	int id;
	st_rect_t rect; ///< \~chinese 矩形框 \~english bodymesh rect
	float score;	///< 置信度
	st_rmat_t *p_body_poses;            ///< body pose pram  N*3*3 body mesh和衣物的姿态控制参数，做衣物驱动需要用到。
	int body_pose_dim;          //< body pose dimension
	float *p_betas;                ///< body shape param body mesh和衣物的体形控制参数，做衣物驱动需要用到
	int beta_dim;                ///< body shape dimension
	st_mobile_face_mesh_t* p_body_mesh;     ///< \~chinese head mesh信息，包括head mesh关键点及个数 \~english Head mesh information, including head mesh keypoints and count
} st_mobile_bodymesh_t, *p_st_mobile_bodymesh_t;

/// \~chinese @brief 脚部检测结果
/// \~english @brief Foot detection result
typedef struct st_mobile_foot_t {
	int id;						///< \~chinese 脚的ID \~english Foot ID
	float score;				///< \~chinese 脚的置信度 \~english Foot confidence score
	st_rect_t rect;				///< \~chinese 矩形框 \~english Bounding rectangle
	st_pointf_t *p_key_points;  ///< \~chinese 脚部关键点 \~english Foot key points
	int key_points_count;		///< \~chinese 脚部关键点个数 \~english Number of foot key points
	st_left_or_right_t label;	///< \~chinese 脚部类型(左脚还是右脚) \~english Foot type (left or right)
	st_3dpose_t* p_pose3d;		///< \~chinese pose foot 3d位置信息,方向与人正立方向一致，暂不支持resize mirror rotate后处理 \~english Foot 3D position information, consistent with the direction of the person's upright position, not supported for resizing, mirroring, or rotation post-processing, currently
} st_mobile_foot_t, *p_st_mobile_foot_t;

/// \~chinese @brief 所有human_action 的分割结果
/// \~english @brief All segmentation results of human_action
typedef struct st_mobile_human_action_segments_t {
	st_mobile_segment_t *p_figure;           ///< \~chinese 检测到人像分割信息 \~english Detected figure segmentation information
	int figure_count;                        ///　目前支持多人人像分割，分割face_id与p_faces的id对应
	st_mobile_segment_t *p_hair;             ///< \~chinese 检测到头发分割信息 \~english Detected hair segmentation information
	st_mobile_segment_t *p_multi;            ///< \~chinese 检测到多类分割信息 \~english Detected multi-class segmentation information
	st_mobile_segment_t *p_sky;				 ///< \~chinese 检测到天空分割信息 \~english Detected sky segmentation information
	st_mobile_segment_t *p_depth;			 ///< \~chinese 检测到深度估计信息 \~english Detected depth estimation information
	st_mobile_segment_t *p_green;            ///< \~chinese 检测到绿幕分割信息 \~english Detected green screen segmentation information
	st_mobile_segment_t *p_skin;             ///< \~chinese 检测到皮肤分割信息 \~english Detected skin segmentation information
	int skin_count;                          ///< \~chinese 目前支持多人皮肤分割，分割face_id与p_faces的id对应 \~english Currently supports multi-person skin segmentation, where the segmented face_id corresponds to the id in p_faces
	st_mobile_segment_t *p_mouth_parse;      ///< \~chinese 检测到的嘴唇遮挡分割信息 \~english Detected mouth occlusion segmentation information
	int mouth_parse_count;                   ///< \~chinese 检测到的嘴唇数目 与p_faces的id对应 \~english Number of detected mouths corresponding to the id in p_faces
	st_mobile_segment_t *p_head;             ///< \~chinese 检测到头部分割信息 \~english Detected head segmentation information
	int head_count;                          ///< \~chinese 目前支持多人头，分割face_id与p_faces的id对应 \~english Currently supports multi-person heads, where the segmented face_id corresponds to the id in p_faces
	st_mobile_segment_t *p_face_occlusion;   ///< \~chinese 检测到人脸遮挡分割信息 \~english Detected face occlusion segmentation information
	int face_occlusion_count;                ///< \~chinese 目前只支持1个 \~english Currently only supports 1
	st_mobile_segment_t *p_trouser_leg;	     ///< \~chinese 检测到裤腿分割信息 \~english Detected trouser leg segmentation information
	int trouser_leg_count;                   ///< \~chinese 目前只支持1个	 \~english Currently only supports 1
	st_mobile_segment_t* p_cloth;            ///< \~chinese 检测到的衣物分割信息	 \~english Detected clothing segmentation information
	int cloth_count;                         ///< \~chinese 目前只支持1个 \~english Currently only supports 1
	st_mobile_segment_t *p_salientobject;       ///<  显著性(前景)分割
	int salientobject_count;                    ///<　目前只支持1个
} st_mobile_human_action_segments_t;

/// \~chinese @brief human_action检测结果
/// \~english @brief Human action detection result
typedef struct st_mobile_human_action_t {
	st_mobile_face_t *p_faces;               ///< \~chinese 检测到的人脸信息 \~english Detected face information
	int face_count;                          ///< \~chinese 检测到的人脸数目 \~english Number of detected faces
	st_mobile_head_t *p_heads;               ///< \~chinese 检测到的人头信息 \~english Detected head information
	int head_count;                          ///< \~chinese 检测到的人头数目 \~english Number of detected heads
	st_mobile_hand_t *p_hands;               ///< \~chinese 检测到的手的信息 \~english Detected hand information
	int hand_count;                          ///< \~chinese 检测到的手的数目 \~english Number of detected hands
	st_mobile_body_t *p_bodys;               ///< \~chinese 检测到的肢体信息 \~english Detected body information
	int body_count;                          ///< \~chinese 检测到的肢体的数目 \~english Number of detected bodies
	float camera_motion_score;               ///< \~chinese 摄像头运动状态置信度 无效 \~english Camera motion score (not valid)
	st_mobile_human_action_segments_t * p_segments;  ///< \~chinese 所有分割结果 \~english All segmentation results
	st_mobile_nail_t *p_nail;				 ///< \~chinese 检测到的指甲的信息 \~english Detected nail information
	int nail_count;							 ///< \~chinese 检测到的指甲个数 \~english Number of detected nails
	st_mobile_foot_t *p_feet;				 ///< \~chinese 检测到的脚的信息 \~english Detected foot information
	int foot_count;							 ///< \~chinese 检测到的脚的个数 \~english Number of detected feet
	st_mobile_wrist_t *p_wrist;				 ///< \~chinese 检测到的手腕的信息 \~english Detected wrist information
	int wrist_count;						 ///< \~chinese 检测到的手腕的个数 \~english Number of detected wrists
	st_mobile_finger_t *p_finger;			 ///< \~chinese 检测到的手指的信息 \~english Detected finger information
	int finger_count;						 ///< \~chinese 检测到的手指的个数 \~english Number of detected fingers
	st_mobile_bodymesh_t * p_body_meshs;
	int bodymesh_count;
	unsigned char* p_extra_buffer;           ///< \~chinese 额外检测信息 \~english Additional detection information
	int extra_buffer_length;
} st_mobile_human_action_t, *p_st_mobile_human_action_t;



/// \~chinese @brief mesh类型
/// \~english @brief Mesh types
typedef enum {
	ST_MOBILE_FACE_MESH = 1,  ///< \~chinese face mesh 类型 \~english Face mesh type
	ST_MOBILE_HEAD_MESH = 2,   ///< \~chinese 360度mesh 类型 \~english Head mesh type
	ST_MOBILE_BODY_MESH = 3      ///< \~chinese body mesh 类型 \~english BODY mesh type
} st_mobile_mesh_type;


/// \~chinese @brief 三角面片的顶点索引
/// \~english @brief Vertex indices of a triangle face
typedef struct st_face_mesh_index_t {
	int v1;
	int v2;
	int v3;
}st_face_mesh_index_t, *p_st_face_mesh_index_t;

/// \~chinese @brief 3d mesh 三角面片索引结果
/// \~english @brief 3D mesh triangle face index result
typedef struct st_mobile_face_mesh_list_t {
	st_face_mesh_index_t* p_face_mesh_index;
	int face_mesh_list_count;
}st_mobile_face_mesh_list_t, *p_st_mobile_face_mesh_list_t;

/// \~chinese @brief 标准obj mesh 信息
/// \~english @brief Standard OBJ mesh information
typedef struct st_mobile_mesh_info_t {
	st_mobile_face_mesh_list_t* p_mesh;             ///< \~chinese mesh面片索引结果 \~english Mesh triangle face index result
	st_mobile_face_mesh_list_t* p_mouth_eye_mesh;   ///< \~chinese mesh嘴巴和眼睛面片索引结果 \~english Mesh triangle face index result for mouth and eyes
	st_mobile_mesh_part_uv_t * p_part_uv_standard;  ///< \~chinese 五官默认纹理坐标， 目前仅支持facemesh的眉毛 \~english Default texture coordinates for facial features, currently only supports eyebrows in facemesh
	int part_uv_standard_cnt;                       ///< \~chinese p_part_uv_standard长度 \~english Length of p_part_uv_standard
	st_mobile_mesh_part_uv_t * p_part_uv_safe_line_standard;  ///< \~chinese 五官默认区域， 目前仅支持facemesh的眉毛 \~english Default region for facial features, currently only supports eyebrows in facemesh
	int part_uv_safe_line_standard_count;           ///< \~chinese 带uv的五官部位个数p_part_uv_safe_line_standard 的长度 \~english Length of p_part_uv_safe_line_standard with UV coordinates for facial features
}st_mobile_mesh_info_t;



// \~chinese @brief body鍥涘厓鏁扮粨鏋
/// \~english @brief Body quaternion result
typedef struct st_mobile_body_avatar_t {
	st_quaternion_t *p_body_quat_array;     ///< \~chinese 浜轰綋鍥涘厓鏁版暟缁 \~english Array of body quaternions
	int body_quat_count;                    ///< \~chinese 浜轰綋鍥涘厓鏁扮粍涓ª鏁°(0 鎴 79) \~english Number of body quaternions (0 or 79)
	bool is_idle;                           ///< \~chinese 鏄¯鍚︽槸idle鐘舵€ \~english Whether it is in idle state
} st_mobile_body_avatar_t, *p_st_mobile_body_avatar_t;

#endif
