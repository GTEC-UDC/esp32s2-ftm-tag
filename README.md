# ESP32 S2 FTM Tag

This software is based on this example: [https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm](https://github.com/espressif/esp-idf/tree/master/examples/wifi/ftm).

The software performs Fine Time Measurement (FTM) with nearby compatible Access Points (APs or "anchors"). It scans for suitable FTM responders, then sequentially initiates FTM procedures with each detected anchor. 

After each FTM session with an anchor, the device:
1. Processes the FTM report, including raw RTT, estimated RTT, and driver-estimated distance.
2. Calculates its own distance estimation using an embedded machine learning model.
3. Stores the overall FTM results along with detailed data for each FTM frame exchanged (RTT, RSSI, and timestamps T1-T4).
4. Periodically (default: after 5 measurements), it connects to a pre-configured Wi-Fi network.
5. Publishes the collected FTM data in JSON format to a Zenoh topic (default: `ftm`).
6. Disconnects from the Wi-Fi network and resumes FTM measurements.

The JSON output published via Zenoh has the following structure:

```json
{
  "anchor_id": "7c:df:a1:05:87:c7",
  "rtt_est": 3000,
  "rtt_raw": 2850,
  "dist_est": 0.52,
  "own_est": 0.48,
  "num_frames": 16,
  "frames": [
    {
      "rssi": -35,
      "rtt": 2800,
      "t1": 123456789012,
      "t2": 123456789234,
      "t3": 123456790456,
      "t4": 123456790678
    },
    {
      "rssi": -36,
      "rtt": 2900,
      "t1": 123456889012,
      "t2": 123456889234,
      "t3": 123456890456,
      "t4": 123456890678
    }
    // ... more frames up to num_frames ...
  ]
}
```

Where:
- `anchor_id` (string): MAC address of the FTM responder AP.
- `rtt_est` (uint32): Estimated Round Trip Time in picoseconds, from the FTM report.
- `rtt_raw` (uint32): Raw Round Trip Time in picoseconds, from the FTM report.
- `dist_est` (float32): Estimated distance in meters, calculated by the ESP-IDF Wi-Fi driver.
- `own_est` (float32): Custom-calculated distance estimation in meters, from the local ML model.
- `num_frames` (int32): Number of FTM frames included in the `frames` array.
- `frames` (array of objects): Detailed information for each FTM frame exchanged.
  - `rssi` (int32): RSSI for the individual frame.
  - `rtt` (uint32): RTT for the individual frame in picoseconds.
  - `t1` (uint64): Timestamp T1 in picoseconds (departure from initiator).
  - `t2` (uint64): Timestamp T2 in picoseconds (arrival at responder).
  - `t3` (uint64): Timestamp T3 in picoseconds (departure from responder).
  - `t4` (uint64): Timestamp T4 in picoseconds (arrival at initiator).

By default, it uses 16 frames per FTM burst with a burst periodicity of 2 (see `frm_count` and `burst_period` in `start_ftm_session`).

This software is intended to be used in conjunction with the ESP32 S2 FTM ANCHOR, which is available here: [https://github.com/GTEC-UDC/esp32s2-ftm-anchor](https://github.com/GTEC-UDC/esp32s2-ftm-anchor)

It requires Eclipse Zenoh (specifically Zenoh-Pico for the embedded side) for the pub/sub communication. More information on Zenoh can be found here: [https://zenoh.io/](https://zenoh.io/)

## Use

Just compile a flash it in a ESP32 S2 module using the ESPRESSIF instructions: [https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#introduction)

## Cite

The code in this repository is related to the following work:

*V. Barral Vales, O. C. Fernández, T. Domínguez-Bolaño, C. J. Escudero and J. A. García-Naya, "Fine Time Measurement for the Internet of Things: A Practical Approach Using ESP32," in IEEE Internet of Things Journal, vol. 9, no. 19, pp. 18305-18318, 1 Oct.1, 2022, doi: 10.1109/JIOT.2022.3158701.* 

If you make use of this code, a citation is appreciated.
