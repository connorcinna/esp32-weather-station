#define TRIGGER_TEMP_MEASURE_NOHOLD 0xF3
#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#include "htu21d.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"


void work()
{
    for (;;)
    {
        float temp_out = ht21d_read_temperature();
        float humidity_out = ht21d_read_humidity();
        printf("Temperature: %.2f", temp_out);
        printf("Humidity: %.2f", humidity_out);
    }
}
void app_main() 
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret); 

    //initialize HTU21D params and set it up with i2c
    ret = htu21d_init(I2C_NUM_0, SDA_PIN, SCL_PIN, GPIO_PULLUP_ENABLE, GPIO_PULLUP_ENABLE);
    if (ret != HTU21D_ERR_OK)
    {
        printf("Error initializing htu21d temperature sensor");
        exit(-1);
    }

    //main loop - constantly read from the HTU21D sensor
    work();

}