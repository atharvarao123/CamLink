    #include <Arduino.h>
    #include <WiFi.h>
    #include <HTTPClient.h>
    #include <esp_camera.h>
    #include <WebServer.h>
    #define CAMERA_MODEL_WROVER_KIT
    #include "camera_pins.h"

    const char *serverUrl = "http://10.0.0.43:5061/videos/";
    const char *ssid_Router     = "wifi name";
    const char *password_Router = "wifi password";
    camera_config_t config;
    TaskHandle_t takePicture = NULL;
    TaskHandle_t receiveSignalTask = NULL;
    #define PICTURE 2
    #define yesPin 12
    #define noPin 15
    void camera_init();
    WebServer Server;
    void receiveSignal();
    void clickPicture(void* parameters);
    void setup() {
        pinMode(PICTURE,INPUT_PULLUP);
        pinMode(yesPin,OUTPUT);
        pinMode(noPin,OUTPUT);
        Serial.begin(9600);
        camera_init();
        WiFi.begin(ssid_Router, password_Router);
        Serial.print("Connecting to Wifi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }

        Server.on("/data/",HTTP_POST,receiveSignal);
        Server.begin(80);

        Serial.println("\nConnected to WiFi");
        xTaskCreate(clickPicture,"takes a picture",2048,NULL,1,&takePicture);
    }

    void clickPicture(void* parameters){
        vTaskDelay(2000/portTICK_PERIOD_MS);
        for(;;){
            Serial.print(digitalRead(PICTURE));
            if(digitalRead(PICTURE) == LOW){
                
                camera_fb_t *cam = esp_camera_fb_get();
                if(!cam){
                    delay(1000);
                    return;
                }
                HTTPClient http;
                http.begin(serverUrl);
                int response = http.POST(cam->buf,cam->len);
                Serial.printf("%d",response);
                http.end();
                esp_camera_fb_return(cam);
                vTaskDelay(2000/portTICK_PERIOD_MS);
            }
            vTaskDelay(2000/portTICK_PERIOD_MS);
        }
    }

    void receiveSignal(){
    if(Server.hasArg("plain")){
        String text = Server.arg("plain");
        Server.send(200,"text/plain","received thanks");
        if(text == "yes"){  
           
            digitalWrite(yesPin,HIGH);
            vTaskDelay(5000/portTICK_PERIOD_MS);
            digitalWrite(yesPin,LOW);
        }
        else{
          
            digitalWrite(noPin,HIGH);
            vTaskDelay(5000/portTICK_PERIOD_MS);
            digitalWrite(noPin,LOW);
        }
    }
    else{
        Server.send(400,"text/plain","no");
    }
    }
    void loop() {
    Server.handleClient();
    vTaskDelay(500/portTICK_PERIOD_MS);
    }

    void camera_init() {
        config.ledc_channel = LEDC_CHANNEL_0;
        config.ledc_timer = LEDC_TIMER_0;
        config.pin_d0 = Y2_GPIO_NUM;
        config.pin_d1 = Y3_GPIO_NUM;
        config.pin_d2 = Y4_GPIO_NUM;
        config.pin_d3 = Y5_GPIO_NUM;
        config.pin_d4 = Y6_GPIO_NUM;
        config.pin_d5 = Y7_GPIO_NUM;
        config.pin_d6 = Y8_GPIO_NUM;
        config.pin_d7 = Y9_GPIO_NUM;
        config.pin_xclk = XCLK_GPIO_NUM;
        config.pin_pclk = PCLK_GPIO_NUM;
        config.pin_vsync = VSYNC_GPIO_NUM;
        config.pin_href = HREF_GPIO_NUM;
        config.pin_sccb_sda = SIOD_GPIO_NUM;
        config.pin_sccb_scl = SIOC_GPIO_NUM;
        config.pin_pwdn = PWDN_GPIO_NUM;
        config.pin_reset = RESET_GPIO_NUM;
        config.xclk_freq_hz = 10000000;
        config.pixel_format = PIXFORMAT_JPEG;
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;

        esp_err_t err = esp_camera_init(&config);
        if (err != ESP_OK) {
            Serial.printf("Camera init failed with error 0x%x\n", err);
            while (true) {
                delay(1000);
            }
        }
    }
