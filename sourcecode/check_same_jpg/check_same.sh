for i in `ls jpg`; do echo $i;./check_same_using_opencv_perceptual_hash_algorithm $1 jpg/$i; done
