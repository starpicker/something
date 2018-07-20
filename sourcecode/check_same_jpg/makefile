target := check_same_using_opencv_perceptual_hash_algorithm

$(target):$(target).cpp
	g++ $^ -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -o $@

clean:
	-rm $(target)
