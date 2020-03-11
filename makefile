clean:
	rm -f bin/*

ios: ios/test_connect.cpp
	g++ -o ./bin/ios_test ios/test_connect.cpp src/socketaddress.cpp
linux: linux/test_connect.cpp
	g++ -o ./bin/linux_test linux/test_connect.cpp src/socketaddress.cpp

android:
	/sbin/.magisk/mirror/data/data/com.n0n3m4.droidc/files/gcc/bin/aarch64-linux-android-g++ -o ./bin/android_test android/test_connect.cpp src/socketaddress.cpp -pie -fPIE --sysroot=$SYSROOT
