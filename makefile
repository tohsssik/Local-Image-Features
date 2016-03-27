objects = ReadCfg.o main.o Sift.o
clibs = `pkg-config opencv --cflags opencv`
librarys = -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core 
SIFT: $(objects)
	g++ $(clibs) $(objects) -o SIFT $(librarys)
main.o: main.cpp
	g++ $(clibs) -c main.cpp -std=c++11  $(librarys)
ReadCfg.o: ReadCfg.cpp ReadCfg.h
	g++ -c ReadCfg.cpp -std=c++11
Sift.o: Sift.cpp Sift.h
	g++ -c  $(clibs)  Sift.cpp -std=c++11 $(librarys)
    
test: test.cpp
	g++ test.cpp -o test -std=c++11
clean:
	rm *.o test

#Windows
# objects = ReadCfg.o main.o
# includePath = -I$(OPENCV)\build\include -I$(OPENCV)\build\include\opencv2 -I$(OPENCV)\build\include\opencv
# clibs = -L$(OPENCV)\build\x86\vc12\staticlib\ -lIlmImf
# SIFT: $(objects)
# 	g++  $(objects) -o SIFT $(clibs) 
# main.o: main.cpp
# 	g++ $(clibs) -c main.cpp -std=c++11 $(includePath) $(clibs) 
# ReadCfg.o: ReadCfg.cpp ReadCfg.h
# 	g++ -c ReadCfg.cpp -std=c++11
# 
# 
# clean:
# 	erase *.o