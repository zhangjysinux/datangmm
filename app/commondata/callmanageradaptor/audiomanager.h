/*
 * \file audiomanager.h
 * Copyright of SyberOS . All rights reserved.
 * Contact: shenweizhong@syberos.com
 * \author Shen Weizhong
 *
 */
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#define AUDIOMANAGER_SERVICE "com.syberos.audiomanager"
#define AUDIOMANAGER_PATH "/com/syberos/audio/manager"
#define AUDIOMANAGER_INTERFACE "com.syberos.audiomanager"

#define AUDIOMANAGER_SIG_VOLUME_CHANGED "volumeChanged"
#define AUDIOMANAGER_SIG_MUTE_CHANGED "muteChanged"

/*!
 * \brief The AudioManager class provides a set of methods to do the communication between the application and the PulseAudio.
 */
class AudioManager{
public:
/*!
  * Type of role defined
  */
enum AM_ROLE_TYPE{
    //! Invalid role
    AM_ROLE_INVALID = 0,
    //! "ringtone"
    AM_ROLE_RINGTONE,
    //! "notification"
    AM_ROLE_NOTIFICATION,
    //! "media"
    AM_ROLE_MEDIA,
    //! "system"
    AM_ROLE_SYSTEM,
    //! "phone"
    AM_ROLE_PHONE,
    //! "alarm"
    AM_ROLE_ALARM,
    //! "keytone"
    AM_ROLE_KEYTONE,
    AM_ROLE_MAX
};

/*!
  * Adjust direction
  */
enum AM_ADJUST_DIRECTION{
    //! The flag to decrease the classification volume.
    ADJUST_LOWER = 1,
    //! The flag to increase the classification volume.
    ADJUST_RAISE,
    //! Maintain the previous classification volume. This may be useful when needing to show the volume toast without actually modifying the volume.
    ADJUST_SAME
};

/*!
  * Type of port to output voice
  */
enum AM_PORT_TYPE{
    //! Output IHF
    AM_PORT_OUTPUT_IHF = 0,
    //! Output Headphone
    AM_PORT_OUTPUT_HEADPHONE,
    //! Call on Earpiece
    AM_PORT_CALLON_EARPIECE,
    //! Call on Headphone
    AM_PORT_CALLON_HEADPHONE,
    //! Call on Speaker
    AM_PORT_CALLON_SPEAKER,
    //! Output IHF and Headphone
    AM_PORT_OUTPUT_IHFHEADPHONE,
    //! Call on Bluetooth
    AM_PORT_CALLON_BLUETOOTH,
    //! Input main Micphone
    AM_PORT_INPUT_MAIN_MIC,
    //! Invalid device
    AM_PORT_INVALID
};

/*!
  * Audio mode in communication
  */
enum AM_AUDIO_MODE{
    //! Normal
    AM_AUDIO_MODE_NORMAL = 0,
    //! SelfOrgnet
    AM_AUDIO_MODE_SELFORGNET,
    //! BaoTong
    AM_AUDIO_MODE_BAOTONG,
    //! ZhanHu
    AM_AUDIO_MODE_ZHANHU,
    //! TianTong
    AM_AUDIO_MODE_TIANTONG,
    //! LTE
    AM_AUDIO_MODE_LTE,
    //! Invalid Audio Mode
    AM_AUDIO_MODE_INVALID
};

/*!
  * Switch I2S mode
  */
enum AM_AUDIO_I2S_MODE{
    //! TianTong
    AM_AUDIO_I2S_MODE_TIANTONG= 0,
    //! HDMI
    AM_AUDIO_I2S_MODE_HDMI,
    //! Invalid I2S Mode
    AM_AUDIO_I2S_MODE_INVALID
};

/*!
  * Audio route for TianTong
  */
enum AM_TT_AUDIO_ROUTE{
    //! BT <-> LC1860 <-> TT
    AM_TT_AUDIO_ROUTE_BT_TT = 0,   //1---0
    //! BT <-> LC1860 <-> LTE      //
    AM_TT_AUDIO_ROUTE_BT_LTE,      //1---2
    //! TT <-> LC1860 <-> 1160
    AM_TT_AUDIO_ROUTE_TT,          //0---3
    //! Invalid Route
    AM_TT_AUDIO_ROUTE_INVALID
};


public:
    AudioManager();
    ~AudioManager();

public:

/*!
 * \brief Set the static volume or route volume to the role specified.
 *        There are two kinds of volumes in Ginkgo-P, one is static volume and the another is route volume.
 *        This method adjusts the route volume when the role specified is active, otherwise updates the static volume directly.
 *
 * \param role Specify the volume of which role will be changed.
 * \param volume The value will be set
 *
 * \note The range of the volume is from 0 to 100.
 */
void setVolume(AM_ROLE_TYPE role, int volume);

/*!
 * \brief Get the static volume or route volume to the role specified.
 *        There are two kinds of volumes in Ginkgo-P, one is static volume and the another is route volume.
 *        This method gets the route volume when the role specified is active, otherwise returns the static volume directly.
 *
 * \param role Specify the volume of which role will be returned.
 *
 * \return the volume of the specified role.
 *
 * \note The normal range of the volume is from 0 to 100.
 *       -1 will be returned when an invalid role is inputing.
 */
int getVolume(AM_ROLE_TYPE role);

/*!
 * \brief Get the active role
 *
 * \return Will be return one of the value defined in the enum AM_ROLE_TYPE.
 *
 * \note Return AM_ROLE_INVALID immediately if there is no active role.
 */
AM_ROLE_TYPE getActiveRole();

/*!
 * \brief Check whether the mute is on or off
 *
 * \return true if the mute is on, false if it's off.
 *
 */
bool isMute();

/*!
 * \brief Set the state of mute to on or off.
 *
 * \param state true indicates that the state of mute will be set to on, false will let the state of mute to off.
 *
 */
void setMute(bool state);

/*!
 * \brief Adjust the volume of a particular role by one step in a direction.
 *
 * \param role Specify the volume of which role will be changed.
 * \param direction The direction to adjust the volume, the value is one of ADJUST_LOWER, ADJUST_RAISE and ADJUST_SAME.
 *
 * \return The value after adjusting the volume.
 *
 * \note The range of the volume is from 0 to 12.
 */
int adjustVolume(AM_ROLE_TYPE role, AM_ADJUST_DIRECTION direction);

/*!
 * \brief Change the active port to output voice. Such as change the route from earpiece to speaker.
 *
 * \param port the expected port.
 *
 */
void setPort(AM_PORT_TYPE port);

/*!
 * \brief Get the active port.
 *
 * \return The active port used by current.
 *
 */
AM_PORT_TYPE getPort();

/*!
 * \brief Change the active audio mode to output voice in communication.
 *
 * \param mode the expected audio mode in communication.
 *
 */
void setAudioMode(AM_AUDIO_MODE mode);

/*!
 * \brief Get the active audio mode.
 *
 * \return The active audio mode used by current.
 *
 */
AM_AUDIO_MODE getAudioMode();

/*!
 * \brief Change the i2s mode for TIANTONG or HDMI.
 *
 * \param the i2s mode.
 *
 */
void setI2SMode(AM_AUDIO_I2S_MODE mode);

/*!
 * \brief Get the current i2s mode.
 *
 * \return the current i2s mode.
 *
 */
int getI2SMode();


/*!
 * \brief Start Changing the audio route to output voice in TianTong communication.
 *
 * \param the audio route the expected in TianTong communication.
 *
 */
void startTrans(AM_TT_AUDIO_ROUTE route);

/*!
 * \brief Stop TianTong communication.
 *
 */
void stopTrans();
};
#endif // AUDIOMANAGER_H
