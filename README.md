# kppd
Configure post-processing settings of Qualcomm MDSS  

##Building
export CROSS_COMPILE=\<path\>  
export SYSROOT=\<path\>  
make  

##Clean
make clean  

##Installing on Android
adb push ./out/kppd /data/local/tmp/  
adb push ./template/postproc.conf /data/local/tmp/  
adb shell  
chmod 0755 /data/local/tmp/kppd  
./data/local/tmp/kppd /data/local/tmp/postproc.conf  
