objects = ReadCfg.o main.o
clibs = `pkg-config opencv --cflags opencv`
SIFT: $(objects)
	g++ $(clibs) $(objects) -o SIFT -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core 
main.o: main.cpp
	g++ $(clibs) -c main.cpp -std=c++11  -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab  -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann  -lopencv_core
ReadCfg.o: ReadCfg.cpp ReadCfg.h
	g++ -c ReadCfg.cpp -std=c++11


clean:
	rm *.o