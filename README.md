# ESP32 S2 FTM Tag

This software is based on this example: [https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm](https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm).

The software starts by scanning for APs with which to initiate FTM communication. After scanning, it starts to perform FTM communications with each of the detected APs, outputting a JSON with the following data via serial port:

```json
{"id":"7c:df:a1:05:87:c7", "tof":3, "d":52, "rss":-37}
```

where:
- *id*: MAC of the receiver AP
- *tof*: Time of flight.
- *d*: Estimate distance in cm.
- *rss*: Average rssi of all frames.

By default, it uses 32 frames with an periodicity of 200ms.

This software is intended to be used in conjunction with the ESP32 S2 FTM ANCHOR, which is available here: [https://github.com/GTEC-UDC/esp32s2-ftm-anchor](https://github.com/GTEC-UDC/esp32s2-ftm-anchor)

## Use

Just compile a flash it in a ESP32 S2 module using the ESPRESSIF instructions: [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction)

## Cite

The code in this repository is related to the following work:

*V. Barral Vales, O. C. Fernández, T. Domínguez-Bolaño, C. J. Escudero and J. A. García-Naya, "Fine Time Measurement for the Internet of Things: A Practical Approach Using ESP32," in IEEE Internet of Things Journal, vol. 9, no. 19, pp. 18305-18318, 1 Oct.1, 2022, doi: 10.1109/JIOT.2022.3158701.* 

If you make use of this code, a citation is appreciated.
