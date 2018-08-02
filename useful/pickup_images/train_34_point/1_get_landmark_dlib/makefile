face_landmark_detection_ex_ml:face_landmark_detection_ex_ml.cpp
	g++ -std=c++11 $^ -o $@ -llapack -lgif -lblas -ldlib

test:
	./face_landmark_detection_ex_ml shape_predictor_68_face_landmarks.dat images.txt
	
clean:
	-rm face_landmark_detection_ex_ml
