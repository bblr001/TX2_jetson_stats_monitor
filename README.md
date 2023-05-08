#This project refer this job: https://github.com/zhangxianbing/jetson_stats
# TX2_jetson_stats_monitor
use tegrastats and TX2 INA3221 monitoring CPU, gpu, etc information

use step:
1. cd TX2_jetsion_stats_monitor
2. mkdir build
3. cmake ..
4. make
5. ./jetson_stats_spec_sample

and then TX2_stats.log will be logged under: /home/ubuntu/Documents/
the log will be like as follow:

2023-05-06 15:04:30 RAM 1645/7850MB (lfb 1312x4MB) SWAP 0/3925MB (cached 0MB) CPU [8%@2035,off,off,7%@2035,9%@2035,6%@2035] EMC_FREQ 0% GR3D_FREQ 0% PLL@40C MCPU@40C PMIC@50C Tboard@38C GPU@39C BCPU@40C thermal@39.6C Tdiode@39C CPV [ current:0mA power:0mV voltage:0.00mW ] TEMP [BCPU-therm: 40.50 GPU-therm: 39.00 MCPU-therm: 40.50 PLL-therm: 40.50 PMIC-Die: 50.00 Tboard_tegra: 38.00 Tdiode_tegra: 39.25 thermal-fan-est: 39.40 ]
2023-05-06 15:04:32 RAM 1645/7850MB (lfb 1312x4MB) SWAP 0/3925MB (cached 0MB) CPU [5%@2035,off,off,13%@2035,5%@2035,4%@2035] EMC_FREQ 0% GR3D_FREQ 0% PLL@40C MCPU@40C PMIC@50C Tboard@38C GPU@39C BCPU@40C thermal@39.9C Tdiode@39.25C CPV [ current:0mA power:0mV voltage:0.00mW ] TEMP [BCPU-therm: 41.00 GPU-therm: 39.00 MCPU-therm: 41.00 PLL-therm: 41.00 PMIC-Die: 50.00 Tboard_tegra: 38.00 Tdiode_tegra: 39.25 thermal-fan-est: 39.90 ]
2023-05-06 15:04:35 RAM 1645/7850MB (lfb 1312x4MB) SWAP 0/3925MB (cached 0MB) CPU [15%@2035,off,off,9%@2035,5%@2035,1%@2035] EMC_FREQ 0% GR3D_FREQ 0% PLL@40C MCPU@40C PMIC@50C Tboard@38C GPU@39C BCPU@40C thermal@39.6C Tdiode@39.25C CPV [ current:0mA power:0mV voltage:0.00mW ] TEMP [BCPU-therm: 40.50 GPU-therm: 39.00 MCPU-therm: 40.50 PLL-therm: 40.50 PMIC-Die: 50.00 Tboard_tegra: 38.00 Tdiode_tegra: 39.25 thermal-fan-est: 39.60 ]
2023-05-06 15:04:38 RAM 1645/7850MB (lfb 1312x4MB) SWAP 0/3925MB (cached 0MB) CPU [9%@2035,off,off,2%@2035,9%@2035,9%@2035] EMC_FREQ 0% GR3D_FREQ 0% PLL@41C MCPU@41C PMIC@50C Tboard@38C GPU@39C BCPU@41C thermal@39.7C Tdiode@39.25C CPV [ current:0mA power:0mV voltage:0.00mW ] TEMP [BCPU-therm: 41.00 GPU-therm: 39.00 MCPU-therm: 41.00 PLL-therm: 41.00 PMIC-Die: 50.00 Tboard_tegra: 38.00 Tdiode_tegra: 39.25 thermal-fan-est: 40.20 ]
