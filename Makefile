cc = msp430-elf-gcc
master_device = msp430g2553
slave_device  = msp430g2553

obj_dir = obj
src_dir = src

# Source files
master = master/adc_test.c master/input.c ssbl.c
slave = slave/adc_test.c ssbl.c

raw_obj_m = $(patsubst %.c, %_$(master_device).o, $(master))
obj_m = $(patsubst %, $(obj_dir)/%, $(raw_obj_m))

raw_obj_s = $(patsubst %.c, %_$(slave_device).o, $(slave))
obj_s = $(patsubst %, $(obj_dir)/%, $(raw_obj_s))

all: master slave

master: mkdir $(obj_m)
	$(cc) $(obj_m) -mmcu=$(master_device) -o bin/$@.msp

slave: mkdir $(obj_s)
	$(cc) $(obj_s) -mmcu=$(slave_device) -o bin/$@.msp

$(obj_dir)/%_$(master_device).o: $(src_dir)/%.c
	$(cc) -mmcu=$(master_device) -c $< -o $@
$(obj_dir)/%_$(slave_device).o: $(src_dir)/%.c
	$(cc) -mmcu=$(slave_device) -c $< -o $@

.PHONY: clean
clean:
	rm -rf bin/* $(obj_dir)

.PHONY: mkdir
mkdir:
	mkdir -pv bin $(obj_dir) $(obj_dir)/master $(obj_dir)/slave
