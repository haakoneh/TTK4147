busyTestSingleCore : busytest-singleCore.c
	gcc -o  busytestSingleCore busytest-singleCore.c ../io.c -D_GNU_SOURCE -lpthread -lrt -lcomedi -lm
	./busytestSingleCore

busytest : busytest.c
	gcc -o busytest busytest.c ../io.c -D_GNU_SOURCE -lpthread -lrt -lcomedi -lm
	./busytest
