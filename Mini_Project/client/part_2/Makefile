TOOL_CHAIN= ~/Desktop/TTK4147_21/TTK4147_exercises/Mini_Project/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin
CROSS=avr32-linux-

simple_test : PI_regulator.c
	$(TOOL_CHAIN)/$(CROSS)gcc PI_regulator.c miniproject.c udp_conn.c -o simple_test -lrt -pthread	
	cp simple_test /export/nfs/home/SW
	
simple_test_gcc : PI_regulator.c
	gcc PI_regulator.c miniproject.c udp_conn.c -o simple_test_gcc -lrt -pthread	

