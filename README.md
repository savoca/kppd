# kcal
Configure post-processing settings of Qualcomm MDSS

##Building
export CROSS_COMPILE=\<path\>  
export SYSROOT=\<path\>  
make  

##Clean
make clean  

##Installing on Android
adb push ./out/kcal /data/local/tmp/  
adb shell  
chmod 0755 /data/local/tmp/kcal  
./data/local/tmp/kcal  

##Usage
kcal pcc 256 256 256 - (red, green, blue)  
kcal pa 0 256 256 256 - (hue, saturation, value, contrast)  
kcal pa_v2 0 256 256 256 - (Use pa_v2 for SoCs apq8084 and newer)  
kcal igc - (invert)  
kcal lut 255 255 255 - (red, green, blue - MDP3 HW only)
