/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_types.h"
#include "esp_err.h"

#include "driver/i2c.h"

/* ES8311 address: CE pin low - 0x18, CE pin high - 0x19 */
#define ES8311_ADDRRES_0 0x18u
#define ES8311_ADDRESS_1 0x19u

#ifdef __cplusplus
extern "C" {
#endif

typedef void *es8311_handle_t;

typedef enum {
    ES8311_MIC_GAIN_MIN = -1,
    ES8311_MIC_GAIN_0DB,
    ES8311_MIC_GAIN_6DB,
    ES8311_MIC_GAIN_12DB,
    ES8311_MIC_GAIN_18DB,
    ES8311_MIC_GAIN_24DB,
    ES8311_MIC_GAIN_30DB,
    ES8311_MIC_GAIN_36DB,
    ES8311_MIC_GAIN_42DB,
    ES8311_MIC_GAIN_MAX
} es8311_mic_gain_t;

typedef enum es8311_resolution_t {
    ES8311_RESOLUTION_16 = 16,
    ES8311_RESOLUTION_18 = 18,
    ES8311_RESOLUTION_20 = 20,
    ES8311_RESOLUTION_24 = 24,
    ES8311_RESOLUTION_32 = 32
} es8311_resolution_t;

typedef struct es8311_clock_config_t {
    bool mclk_inverted;
    bool sclk_inverted;
    bool mclk_from_mclk_pin; // true: from MCLK pin (pin no. 2), false: from SCLK pin (pin no. 6)
    int  sample_frequency;   // in Hz
} es8311_clock_config_t;

/**
 * @brief Initialize ES8311
 *
 * @param dev ES8311 handle
 * @param[in] clk_cfg Clock configuration
 * @param[in] res_in  Input serial port resolution
 * @param[in] res_out Output serial port resolution
 * @return
 *     - ESP_OK success
 *     - ESP_ERR_INVALID_ARG Sample frequency or resolution invalid
 *     - Else fail
 */
esp_err_t es8311_init(es8311_handle_t dev, const es8311_clock_config_t *const clk_cfg, const es8311_resolution_t res_in,
                      const es8311_resolution_t res_out);

/**
 * @brief Set output volume
 *
 * Volume paramter out of <0, 100> interval will be truncated.
 *
 * @param dev ES8311 handle
 * @param[in] volume Set volume (0 ~ 100)
 * @param[out] volume_set Volume that was set. Same as volume, unless volume is outside of <0, 100> interval.
 *                        This parameter can be set to NULL, if user does not need this information.
 *
 * @return
 *     - ESP_OK success
 *     - Else fail
 */
esp_err_t es8311_voice_volume_set(es8311_handle_t dev, int volume, int *volume_set);

/**
 * @brief Get output volume
 *
 * @param dev ES8311 handle
 * @param[out] volume get volume (0 ~ 100)
 *
 * @return
 *     - ESP_OK success
 *     - Else fail
 */
esp_err_t es8311_voice_volume_get(es8311_handle_t dev, int *volume);

/**
 * @brief Print out ES8311 register content
 *
 * @param dev ES8311 handle
 */
void es8311_register_dump(es8311_handle_t dev);

/**
 * @brief Mute ES8311 output
 *
 * @param dev ES8311 handle
 * @param[in] enable true: mute, false: don't mute
 * @return
 *     - ESP_OK success
 *     - Else fail
 */
esp_err_t es8311_voice_mute(es8311_handle_t dev, bool enable);

/**
 * @brief Set Microphone gain
 *
 * @param dev ES8311 handle
 * @param[in] gain_db Microphone gain
 * @return
 *     - ESP_OK success
 *     - Else fail
 */
esp_err_t es8311_microphone_gain_set(es8311_handle_t dev, es8311_mic_gain_t gain_db);

/**
 * @brief Configure microphone
 *
 * @param dev ES8311 handle
 * @param[in] digital_mic Set to true for digital microphone
 * @return
 *     - ESP_OK success
 *     - Else fail
 */
esp_err_t es8311_microphone_config(es8311_handle_t dev, bool digital_mic);

/**
 * @brief Configure sampling frequency
 *
 * @note This function is called by es8311_init().
 *       Call this function explicitly only if you want to change sample frequency during runtime.
 * @param dev ES8311 handle
 * @param[in] mclk_frequency   MCLK frequency in Hz on MCLK or SCLK pin, depending bit register01[7].
 * @param[in] sample_frequency Required sample frequency in Hz, e.g. 44100, 22050...
 * @return
 *     - ESP_OK success
 *     - ESP_ERR_INVALID_ARG cannot set clock dividers for given MCLK and sampling frequency
 *     - Else I2C read/write error
 */
esp_err_t es8311_sample_frequency_config(es8311_handle_t dev, int mclk_frequency, int sample_frequency);

/**
 * @brief Create ES8311 object and return its handle
 *
 * @param[in] port     I2C port number
 * @param[in] dev_addr I2C device address of ES8311
 *
 * @return
 *     - NULL Fail
 *     - Others Success
 */
es8311_handle_t es8311_create(const i2c_port_t port, const uint16_t dev_addr);

/**
 * @brief Delete ES8311 object
 *
 * @param dev ES8311 handle
 */
void es8311_delete(es8311_handle_t dev);

#ifdef __cplusplus
}
#endif
