clean:
	rm -f bin/*

ios: ios/test_connect.cpp
	g++ -o ./bin/ios_test ios/test_connect.cpp src/socketaddress.cpp